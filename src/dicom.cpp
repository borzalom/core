// XtraBYtes - The Proof of Signature Blocktech Superchain - http://xtrabytes.global
// Copyright (c) 2017, Zoltan Szabo. All rights reserved.
// Copyright (c) 2017, XtraBYtes Founders and Developers ( see AUTHORS )
// Licensed under GNU General Public License Version 3 or later (the "GPL")

// DICOM - Distributed Command Message


#include <boost/lexical_cast.hpp>
#include <boost/thread/thread.hpp>
#include <boost/asio/buffer.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <fstream>
#include <sstream>
#include "dicom.h"
#include "modules.h"

MODULE_SIGNAL GetSignalByName(std::string SignalName) {
  for (int signal = unknown_signal_first; signal != unknown_signal_last; ++signal) {
    std::string TmpSignalName = GetSignalName((MODULE_SIGNAL)signal); 
    if(TmpSignalName.compare(SignalName)==0) return (MODULE_SIGNAL)signal;
  }
  return unknown_signal_last;
}

std::string GetSignalName(MODULE_SIGNAL signal) {
   switch (signal) {
     case unknown_signal_first			:return "unknown-signal-first";	
     case system_blocks_newblock			:return "system-blocks-newblock";
     case view_page_overview_content  	:return "view-page-overview-content";
     default:
        return "unknown-signal";
   }
}

DICOMserver::DICOMserver(const std::string& address, const std::string& port, std::size_t thread_pool_size)
  			: thread_pool_size_(thread_pool_size), signals_(io_service_), acceptor_(io_service_), new_DICOMconn_() {

  signals_.add(SIGINT);
  signals_.add(SIGTERM);
  #if defined(SIGQUIT)
    signals_.add(SIGQUIT);
  #endif // defined(SIGQUIT)
  signals_.async_wait(boost::bind(&DICOMserver::handle_stop, this));

  boost::asio::ip::tcp::resolver resolver(io_service_);
  boost::asio::ip::tcp::resolver::query query(address, port);
  boost::asio::ip::tcp::endpoint endpoint = *resolver.resolve(query);
  acceptor_.open(endpoint.protocol());
  acceptor_.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
  acceptor_.bind(endpoint);
  acceptor_.listen();

  start_accept();
}

void DICOMserver::start()  {
	
  std::vector<boost::shared_ptr<boost::thread> > threads;
  for (std::size_t i = 0; i < thread_pool_size_; ++i)
  {
    boost::shared_ptr<boost::thread> thread(new boost::thread(
          boost::bind(&boost::asio::io_service::run, &io_service_)));
    threads.push_back(thread);
  }

  for (std::size_t i = 0; i < threads.size(); ++i)
    threads[i]->join();
    
}

void DICOMserver::start_accept()  {
  new_DICOMconn_.reset(new DICOMconn(io_service_, request_handler_));
  acceptor_.async_accept(new_DICOMconn_->socket(),
      boost::bind(&DICOMserver::handle_accept, this,
        boost::asio::placeholders::error));
}

void DICOMserver::handle_accept(const boost::system::error_code& e) {
  
  if (!e)  {  new_DICOMconn_->start();  }
  start_accept();
}

void DICOMserver::handle_stop() {
  
  io_service_.stop();
}

request_handler::request_handler() {}

void request_handler::handle_request(const HTTPrequest& req, reply& rep) {
  
    bool error = false;   
    
    if (req.uri.compare("/dicom") != 0) {
       rep.status = reply::bad_request;
       return;    
    }  
    
    std::stringstream ss;
    ss << req.data;
    boost::property_tree::ptree JSONrequest;
    boost::property_tree::read_json(ss, JSONrequest);
    std::string DICOM = JSONrequest.get<std::string>("DICOM");
    
    bool DICOM_command_recognised = false;
    if(!DICOM.compare("Module-Registration")) {
    	 DICOM_command_recognised = true;
       LogPrint(LL_LOG,"DICOM request: Module-Registration ("+JSONrequest.get<std::string>("module-id")+")");
       boost::property_tree::ptree JSONreply = XBY_Modules.ModuleRegistration(JSONrequest);
       std::stringstream ss;
       boost::property_tree::json_parser::write_json(ss, JSONreply);       
       rep.content=ss.str();       
    } 
    
    if (!DICOM_command_recognised) {
       rep.content="{\"DICOM\":\"Bad command.\"}";
    } 
  
    rep.status = reply::ok;
        
    rep.headers.resize(2);
    rep.headers[0].key = "Content-Length";
    rep.headers[0].value = boost::lexical_cast<std::string>(rep.content.size());
    rep.headers[1].key = "Content-Type";
    rep.headers[1].value = "application/json";
}


DICOMconn::DICOMconn(boost::asio::io_service& io_service, request_handler& handler)
    : strand_(io_service), socket_(io_service), request_handler_(handler) { }

boost::asio::ip::tcp::socket& DICOMconn::socket() {

  return socket_;
}

void DICOMconn::start() {
  socket_.async_read_some(boost::asio::buffer(buffer_),
      strand_.wrap( boost::bind(&DICOMconn::handle_read, shared_from_this(),
                    boost::asio::placeholders::error,
                    boost::asio::placeholders::bytes_transferred)));
}

void DICOMconn::handle_read(const boost::system::error_code& e, std::size_t bytes_transferred) {

  if (!e)  {
    boost::tribool result;
        
    boost::tie(result, boost::tuples::ignore) = request_parser_.parse(
        request_, buffer_.data(), buffer_.data() + bytes_transferred);
      
    if (result) {    	
      request_handler_.handle_request(request_, reply_);
    	      
      boost::asio::async_write(socket_, reply_.to_buffers(),
          strand_.wrap(
            boost::bind(&DICOMconn::handle_write, shared_from_this(),
              boost::asio::placeholders::error)));
    } else if (!result) {      
      reply_.status = reply::bad_request;
      boost::asio::async_write(socket_, reply_.to_buffers(),
          strand_.wrap(
            boost::bind(&DICOMconn::handle_write, shared_from_this(),
              boost::asio::placeholders::error)));
    } else {
      socket_.async_read_some(boost::asio::buffer(buffer_),
          strand_.wrap(
            boost::bind(&DICOMconn::handle_read, shared_from_this(),
              boost::asio::placeholders::error,
              boost::asio::placeholders::bytes_transferred)));
    }
  }

}

void DICOMconn::handle_write(const boost::system::error_code& e) {
  if (!e) {  	 
    boost::system::error_code ignored_ec;
    socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ignored_ec);
  }
}


std::vector<boost::asio::const_buffer> reply::to_buffers()
{
  std::vector<boost::asio::const_buffer> buffers;

  switch (status) {
	  case reply::ok:  
	      buffers.push_back(boost::asio::buffer(HTTP_STATUS_STR_OK)); break;  
	  case reply::bad_request:  
	      buffers.push_back(boost::asio::buffer(HTTP_STATUS_STR_BAD_REQUEST)); break;  
	  default:
	      buffers.push_back(boost::asio::buffer(HTTP_STATUS_STR_SERVER_ERROR));
  }

  for (std::size_t i = 0; i < headers.size(); ++i)
  {
    KeyValue& h = headers[i];
    buffers.push_back(boost::asio::buffer(h.key));
    buffers.push_back(boost::asio::buffer(KEY_VALUE_SEPARATOR_CHARS));
    buffers.push_back(boost::asio::buffer(h.value));
    buffers.push_back(boost::asio::buffer(CRLF_CHARS));
  }
  buffers.push_back(boost::asio::buffer(CRLF_CHARS));
  buffers.push_back(boost::asio::buffer(content));
  return buffers;
}


DICOMclient::DICOMclient( boost::asio::io_service& s ) :ios(s),sock(s) {
     dc = new DICOMclient(s,this);   
}

DICOMclient::DICOMclient( boost::asio::io_service& s, DICOMclient *c ) :ios(s),sock(s) {}

DICOMclient::~DICOMclient() {}

boost::property_tree::ptree DICOMclient::request( const std::string& json )
    {
    	  
        boost::system::error_code error = boost::asio::error::host_not_found;
        sock.close();
        sock.connect( ep, error );
        if( error ) {                	
            throw boost::system::system_error(error);
        }
        
        boost::asio::streambuf request_buf;
        std::ostream request_info(&request_buf);
        request_info << "POST /dicom HTTP/1.1\n";
        request_info << "Host: 127.0.0.1\n";
        request_info << "Content-Type: application/json-rpc\n";
        request_info << "Content-Length: "<<json.size() << "\n\n";

        request_info << json;

        boost::asio::write( sock, request_buf );
        
        std::string str_response;

        boost::system::error_code ec;
        
        // FIXMEE !!!
        char buf[1024];
        size_t bytes_transferred = sock.receive(boost::asio::buffer(buf), {}, ec);
        if (!ec) str_response.append(buf, buf + bytes_transferred);

        std::stringstream resp(str_response);        
        std::string line;

        // read header lines          
        while (std::getline(resp, line) && line != "\r"){}
        std::getline(resp, line); // read data line           
        
        std::stringstream req;
        req << line;
        using boost::property_tree::ptree;
        ptree  pt;
        std::stringstream rtnss(req.str());        
        // FIXMEE error checkings         
        boost::property_tree::json_parser::read_json( rtnss, pt );
        return pt;

}

void DICOMclient::HeartbeatEchoRequest(XBYmodule* module) {
			 	 
	 tcp::resolver resolver(dc->ios);
    tcp::resolver::query q("127.0.0.1",NumberToString(module->DICOMport));
    tcp::resolver::iterator epi = resolver.resolve(q);
    tcp::resolver::iterator end;

    boost::system::error_code error = boost::asio::error::host_not_found;
    int tries = 0;
    while( error && epi != end )
    {   
        if (tries++ > 1) return;
        dc->sock.close();
        dc->sock.connect( *epi, error );
    }
    if( error )  {  return;  }
    dc->ep = *epi;
        	 	 	
	 boost::property_tree::ptree pt = dc->request("{\"DICOM\": \"Heartbeat-Echo-Request\" }");

    module->LastHeartbeat = time(NULL);    
}


boost::property_tree::ptree DICOMclient::ModuleRequest(XBYmodule* module, boost::property_tree::ptree *req) {
			 	 
	 boost::property_tree::ptree reply;		 	 
	 tcp::resolver resolver(dc->ios);
    tcp::resolver::query q("127.0.0.1",NumberToString(module->DICOMport));
    tcp::resolver::iterator epi = resolver.resolve(q);
    tcp::resolver::iterator end;

    boost::system::error_code error = boost::asio::error::host_not_found;
    int tries = 0;
    while( error && epi != end )
    {   
        if (tries++ > 1) return reply;
        dc->sock.close();
        dc->sock.connect( *epi, error );
    }
    if( error )  {  return reply;  }
    dc->ep = *epi;
    
    std::stringstream ss;
    write_json(ss, *req);
    reply = dc->request(ss.str());
           	 	 	
	 return reply;   
}