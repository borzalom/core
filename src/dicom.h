// XtraBYtes - The Proof of Signature Blocktech Superchain - http://xtrabytes.global
// Copyright (c) 2017, Zoltan Szabo. All rights reserved.
// Copyright (c) 2017, XtraBYtes Founders and Developers ( see AUTHORS )
// Licensed under GNU General Public License Version 3 or later (the "GPL")

// DICOM - Distributed Command Message

#ifndef XBY_DICOM_H
#define XBY_DICOM_H

#ifdef _WIN32
  #include <winsock2.h>
#endif  
#include <boost/asio.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/bind.hpp>
#include <boost/array.hpp>
#include <boost/property_tree/json_parser.hpp>

#include <utility>

#include "util.h"

class XBYmodule;

using boost::asio::ip::tcp;

enum MODULE_SIGNAL {
	unknown_signal_first,
   system_blocks_newblock,
   view_page_overview_content,
   unknown_signal_last
};

MODULE_SIGNAL GetSignalByName(std::string SignalName);
std::string GetSignalName(MODULE_SIGNAL signal);

struct reply {

  enum status_type { ok = 200, bad_request = 400, internal_server_error = 500 } status;

  std::vector<KeyValue> headers;
  std::string content;
  std::vector<boost::asio::const_buffer> to_buffers();

};


class request_handler : private boost::noncopyable {

  public:  
    explicit request_handler();
    void handle_request(const HTTPrequest& req, reply& rep);

};

class DICOMconn : public boost::enable_shared_from_this<DICOMconn>, private boost::noncopyable {
  
	public:

  		explicit DICOMconn(boost::asio::io_service& io_service, request_handler& handler);
  		boost::asio::ip::tcp::socket& socket();
  		void start();

	private:

		void handle_read(const boost::system::error_code& e, std::size_t bytes_transferred);
  		void handle_write(const boost::system::error_code& e);

		boost::asio::io_service::strand strand_;
  		boost::asio::ip::tcp::socket socket_;
  		request_handler& request_handler_;
  		boost::array<char, 8192> buffer_;
  		HTTPrequest request_;
  		HTTP_RequestParser request_parser_;
  		reply reply_;
};

typedef boost::shared_ptr<DICOMconn> DICOMconn_ptr;

class DICOMserver : private boost::noncopyable {
	
	public:
  		explicit DICOMserver(const std::string& address, const std::string& port, std::size_t thread_pool_size);      
      void start();
      

	private:
	  void start_accept();
	  void handle_accept(const boost::system::error_code& e);
	  void handle_stop();

	  std::size_t thread_pool_size_;
	  boost::asio::io_service io_service_;
	  boost::asio::signal_set signals_;
	  boost::asio::ip::tcp::acceptor acceptor_;
	  DICOMconn_ptr new_DICOMconn_;
	  request_handler request_handler_;
	  
};

class DICOMclient {
	
  public:  
       DICOMclient( boost::asio::io_service& s );
       DICOMclient( boost::asio::io_service& s, DICOMclient *c );        
       ~DICOMclient();

       boost::asio::io_service& ios;       
       tcp::socket sock;
       tcp::endpoint ep;               

       boost::property_tree::ptree request( const std::string& json );
       boost::property_tree::ptree ModuleRequest(XBYmodule* module, boost::property_tree::ptree *request);
       
       void HeartbeatEchoRequest(XBYmodule* module);

    private:
        DICOMclient* dc;


};

#endif // XBY_DICOM_H
