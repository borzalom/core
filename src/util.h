// XtraBYtes - The Proof of Signature Blocktech Superchain - http://xtrabytes.global
// Copyright (c) 2017, Zoltan Szabo. All rights reserved.
// Copyright (c) 2017, XtraBYtes Founders and Developers ( see AUTHORS )
// Licensed under GNU General Public License Version 3 or later (the "GPL")

#ifndef XBY_UTIL_H
#define XBY_UTIL_H

#include <string>
#include <sstream>
#include <time.h>
#include <vector>
#include <iostream>
#include <boost/logic/tribool.hpp>
#include <boost/tuple/tuple.hpp>

extern const char* const HEX_CHARS;
extern const char* const BASE64_CHARS;

extern const std::string HTTP_STATUS_STR_OK;
extern const std::string HTTP_STATUS_STR_BAD_REQUEST;
extern const std::string HTTP_STATUS_STR_SERVER_ERROR;
extern const std::string KEY_VALUE_SEPARATOR_CHARS;
extern const std::string CRLF_CHARS;

template <typename T> 
std::string NumberToString ( T Number ) {
     std::ostringstream ss;
     ss << Number;
     return ss.str();
}
  
struct KeyValue
{
  std::string key;
  std::string value;
};

struct HTTPrequest
{
  std::string method;
  std::string uri;
  int http_version_major;
  int http_version_minor;
  std::vector<KeyValue> headers;
  std::string data;
};


class HexString {
	
  private:
  		char *HEXdata;	
  		int size;
  		bool IsData;
  		
  public:
  		HexString();
  		~HexString();
  		bool Allocate(int Dsize);
  		bool SetBin(char* data);
  		bool SetHex(char* data);
  		bool Wipe();
  		std::string toString();
};

class RandomNumber {
	
private:
    unsigned int Index;
    unsigned int Offset;

    static unsigned int GetPermutation(unsigned int x);

public:
    RandomNumber();
    unsigned int GetNextRandomNumber();

};

class Encoders {
	
public:
	std::string Base64_Encode(unsigned char const* , unsigned int len);

};

class Decoders {

public:
	std::string Base64_Decode(std::string const& s);

};

class CharType {
 
 public: 
  static bool is_char(int c);
  static bool is_ctl(int c);
  static bool is_tspecial(int c);
  static bool is_digit(int c);
  
};

extern bool ConsoleLog;

enum LogLevel
{
    LL_FATAL_ERROR,
    LL_ERROR,
    LL_WARNING,
    LL_LOG
};

void LogPrint(LogLevel loglevel, const std::string &str);

void ExceptionPrint(std::exception* e, const char* Thread);

class HTTP_RequestParser
{
public:

  HTTP_RequestParser();
  void reset();

  template <typename InputIterator>
  boost::tuple<boost::tribool, InputIterator> parse(HTTPrequest& req,
      InputIterator begin, InputIterator end)
{	 
    while (begin != end)
    {
      boost::tribool result = parseNextChar(req, *begin++);
      if  (state_ == data && begin == end ) {
        result = true;      
      }
      if (result || !result)
        return boost::make_tuple(result, begin);
    }
    boost::tribool result = boost::indeterminate;
    return boost::make_tuple(result, begin);
};


private:
  boost::tribool parseNextChar(HTTPrequest& req, char input);  

  enum state
  {
    method_start,
    method,
    uri,
    http_version_h,
    http_version_t_1,
    http_version_t_2,
    http_version_p,
    http_version_slash,
    http_version_major_start,
    http_version_major,
    http_version_minor_start,
    http_version_minor,
    expecting_newline_1,
    header_line_start,
    header_lws,
    header_name,
    space_before_header_value,
    header_value,
    data_start,
    data,
    expecting_newline_2,
    expecting_newline_3
  } state_;
  
};

#endif // XBY_UTIL_H
