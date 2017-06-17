// XtraBYtes - The Proof of Signature Blocktech Superchain - http://xtrabytes.global
// Copyright (c) 2017, Zoltan Szabo. All rights reserved.
// Copyright (c) 2017, XtraBYtes Founders and Developers ( see AUTHORS )
// Licensed under GNU General Public License Version 3 or later (the "GPL")


//VITALS - Virtual Information Transmissions ALigning STaTiC Superscructures

#ifndef XBY_VITALS_H
#define XBY_VITALS_H

#include <string>
#include <boost/asio.hpp>
#include <boost/property_tree/json_parser.hpp>

using boost::asio::ip::tcp;

class XBridge {
  public:
  
       XBridge( boost::asio::io_service& s );
       XBridge( boost::asio::io_service& s, XBridge *x );        
       ~XBridge();

        

       boost::asio::io_service& ios;       
       tcp::socket sock;
       tcp::endpoint ep;
               

       std::string user;
       std::string pass;
       std::string b64encoded_password;

       boost::property_tree::ptree request( const std::string& json );
       bool connect( const std::string& host_port, const std::string& user, const std::string& pass );
       uint64_t GetBlocks();

    private:
        XBridge* xb;


};

#endif // XBY_VITALS_H
