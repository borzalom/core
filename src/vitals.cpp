// XtraBYtes - The Proof of Signature Blocktech Superchain - http://xtrabytes.global
// Copyright (c) 2017, Zoltan Szabo. All rights reserved.
// Copyright (c) 2017, XtraBYtes Founders and Developers ( see AUTHORS )
// Licensed under GNU General Public License Version 3 or later (the "GPL")

#include "vitals.h"
#include "util.h"

#include <iostream>
#include <istream>
#include <ostream>
#include <sstream>
#include <string>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/system/error_code.hpp>

using boost::asio::ip::tcp;

XBridge::XBridge( boost::asio::io_service& s ) :ios(s),sock(s) {
     xb = new XBridge(s,this);   
}

XBridge::XBridge( boost::asio::io_service& s, XBridge *x ) :ios(s),sock(s) {}

XBridge::~XBridge() {}

boost::property_tree::ptree XBridge::request( const std::string& json )
    {
    	  
        boost::system::error_code error = boost::asio::error::host_not_found;
        sock.close();
        sock.connect( ep, error );
        if( error )
            throw boost::system::system_error(error);

        boost::asio::streambuf request_buf;
        std::ostream request_info(&request_buf);
        request_info << "POST / HTTP/1.1\n";
        request_info << "Host: 127.0.0.1\n";
        request_info << "Content-Type: application/json-rpc\n";
        request_info << "Authorization: Basic " << b64encoded_password << "\n";
        request_info << "Content-Length: "<<json.size() << "\n\n";

        request_info << json;
        std::cout << std::endl;

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
        boost::property_tree::json_parser::read_json( rtnss, pt );
        return pt;

}


bool XBridge::connect( const std::string& host_port, const std::string& user, const std::string& pass )
{     
    std::string pre_encode = user + ":" + pass;
    xb->user = user;
    xb->pass = pass;
    Encoders encoders;
    xb->b64encoded_password = encoders.Base64_Encode((unsigned char *)pre_encode.c_str(),(unsigned)strlen(pre_encode.c_str()));

    std::string host = host_port.substr( 0, host_port.find(':') );
    std::string port = host_port.substr( host.size() + 1 );

    tcp::resolver resolver(xb->ios);
    tcp::resolver::query q(host,port);
    tcp::resolver::iterator epi = resolver.resolve(q);
    tcp::resolver::iterator end;

    boost::system::error_code error = boost::asio::error::host_not_found;
    while( error && epi != end )
    {
        xb->sock.close();
        xb->sock.connect( *epi, error );
    }
    if( error )
    {
        std::cout << boost::system::system_error(error).what() << std::endl;
        return false;
    }
    xb->ep = *epi;
            
    boost::property_tree::ptree pt = xb->request("{\"jsonrpc\": \"1.0\", \"id\":\"1\", \"method\": \"getinfo\", \"params\": [] }");

    return true;
}

uint64_t XBridge::GetBlocks() {
	
  boost::property_tree::ptree pt = xb->request("{\"jsonrpc\": \"1.0\", \"id\":\"1\", \"method\": \"getinfo\", \"params\": [] }");
  uint64_t blocks = pt.get<uint64_t>("result.blocks");
  return blocks;
  
}
