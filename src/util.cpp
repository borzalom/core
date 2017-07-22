// XtraBYtes - The Proof of Signature Blocktech Superchain - http://xtrabytes.global
// Copyright (c) 2017, Zoltan Szabo. All rights reserved.
// Copyright (c) 2017, XtraBYtes Founders and Developers ( see AUTHORS )
// Licensed under GNU General Public License Version 3 or later (the "GPL")

#include "util.h"
#include <cstdlib>
#include <memory.h>
#include <string>
#include <sstream>
#include <iostream>

#include <boost/thread/mutex.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

const char* const HEX_CHARS = "0123456789ABCDEF";
const char* const BASE64_CHARS = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

const std::string KEY_VALUE_SEPARATOR_CHARS = ": ";
const std::string CRLF_CHARS = "\r\n";

const std::string HTTP_STATUS_STR_OK = "HTTP/1.0 200 OK\r\n";
const std::string HTTP_STATUS_STR_BAD_REQUEST = "HTTP/1.0 400 Bad Request\r\n";
const std::string HTTP_STATUS_STR_SERVER_ERROR = "HTTP/1.0 500 Internal Server Error\r\n";


//      HexString
// ------------------------------------------------------------


bool HexString::Allocate(int Dsize) {
	IsData = false;
	if (Dsize > 0) {
	   size = Dsize;
	   HEXdata = (char *)malloc(Dsize);
   	IsData = (HEXdata != NULL);
	}	else {
	size = -1;
	}
   return IsData; 
}

bool HexString::SetBin(char* data) {
	if ( IsData && size>=1 ) { 
	  memcpy( HEXdata, data, size );
	  return true;
	}
	return false;  
}

bool HexString::SetHex(char* data) {
   if ( !IsData || size <= 0) {
     return false;
   }
   else
   {
	   //set hex here
   }
}

bool HexString::Wipe() {	
   if (IsData) {
   	memset( HEXdata, 0, size );
   	free(HEXdata);
   	size = 0;
      IsData = false;
      return true;
   }   	
   return false;
}

std::string HexString::toString() {
    	if (!IsData)
		return "HexString is null";
    std::string output;
    output.reserve(2 * size);
    for (int i = 0; i < size; ++i)
    {
        const unsigned char c = HEXdata[i];
        output.push_back(HEX_CHARS[c >> 4]);
        output.push_back(HEX_CHARS[c & 15]);
    }
    return output;	
   
}

HexString::HexString() {
	IsData = false;
	size = 0;
}

HexString::~HexString() {
  if (IsData) free(HEXdata);
}


//      RandomNumber
// ------------------------------------------------------------

RandomNumber::RandomNumber()
    {
     	  unsigned int RandomBase = (unsigned int) time(NULL); 
        Index = GetPermutation(GetPermutation(RandomBase) + 0x682f0161);
        Offset = GetPermutation(GetPermutation(RandomBase+1) + 0x46790905);
    }

unsigned int RandomNumber::GetPermutation(unsigned int x)
    {
        static const unsigned int prime = 4294967291u;
        if (x >= prime)
            return x;  
        unsigned int residue = ((unsigned long long) x * x) % prime;
        return (x <= prime / 2) ? residue : prime - residue;
    }


unsigned int RandomNumber::GetNextRandomNumber()
    {
        return GetPermutation((GetPermutation(Index++) + Offset) ^ 0x5bf03635);
    }


//      Encoders
// ------------------------------------------------------------


std::string Encoders::Base64_Encode(unsigned char const* bytes_to_encode, unsigned int in_len) {
	
  std::string ret;
  int i = 0;
  int j = 0;
  unsigned char char_array_3[3];
  unsigned char char_array_4[4];

  while (in_len--) {
    char_array_3[i++] = *(bytes_to_encode++);
    if (i == 3) {
      char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
      char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
      char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
      char_array_4[3] = char_array_3[2] & 0x3f;

      for(i = 0; (i <4) ; i++)
        ret += BASE64_CHARS[char_array_4[i]];
      i = 0;
    }
  }

  if (i)
  {
    for(j = i; j < 3; j++)
      char_array_3[j] = '\0';

    char_array_4[0] = ( char_array_3[0] & 0xfc) >> 2;
    char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
    char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
    char_array_4[3] =   char_array_3[2] & 0x3f;

    for (j = 0; (j < i + 1); j++)
      ret += BASE64_CHARS[char_array_4[j]];

    while((i++ < 3))
      ret += '=';

  }

  return ret;
}

//      Decoders
// ------------------------------------------------------------


std::string Decoders::Base64_Decode(std::string const& encoded_string) {

  int in_len = encoded_string.size();
  int i = 0;
  int j = 0;
  int in_ = 0;
  unsigned char char_array_4[4], char_array_3[3];
  std::string ret;

  while (in_len-- && ( encoded_string[in_] != '=') && 
     (isalnum(encoded_string[in_]) || (encoded_string[in_] == '+') || (encoded_string[in_] == '/'))) {
    char_array_4[i++] = encoded_string[in_]; in_++;
    if (i ==4) {
      for (i = 0; i <4; i++)
        char_array_4[i] = std::string(BASE64_CHARS).find(char_array_4[i]);

      char_array_3[0] = ( char_array_4[0] << 2       ) + ((char_array_4[1] & 0x30) >> 4);
      char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
      char_array_3[2] = ((char_array_4[2] & 0x3) << 6) +   char_array_4[3];

      for (i = 0; (i < 3); i++)
        ret += char_array_3[i];
      i = 0;
    }
  }

  if (i) {
    for (j = i; j <4; j++)
      char_array_4[j] = 0;

    for (j = 0; j <4; j++)
      char_array_4[j] = std::string(BASE64_CHARS).find(char_array_4[j]);

    char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
    char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
    char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

    for (j = 0; (j < i - 1); j++) ret += char_array_3[j];
  }

  return ret;
}

//      LogPrint
// ------------------------------------------------------------

bool WriteConsoleLog = true;

static std::string GetExceptionStr(std::exception* e, const char* ThreadName)
{   
    std::stringstream ss;

    if (e) {
       ss << "EXCEPTION: "  << typeid(*e).name() << std::endl << e->what();   
    } else {
       ss << "UNKNOWN EXCEPTION: "; 
    }
    ss << std::endl << " in " << ThreadName << std::endl;

    return ss.str();        
}

std::string DateTimeStrFormat(const char* format, int64_t nTime)
{
    std::locale loc(std::locale::classic(), new boost::posix_time::time_facet(format));
    std::stringstream ss;
    ss.imbue(loc);
    ss << boost::posix_time::from_time_t(nTime);
    return ss.str();
}

void ExceptionPrint(std::exception* e, const char* Thread)
{
    std::string message = GetExceptionStr( e, Thread);
    LogPrint(LL_FATAL_ERROR, message);
    throw;
}

boost::mutex LOG_mutex;
void LogPrint(LogLevel loglevel, const std::string &str) {       
       
       boost::mutex::scoped_lock scoped_lock(LOG_mutex);		
       if (WriteConsoleLog) {
       	 switch (loglevel) {
       	 	  case LL_FATAL_ERROR: std::cout << "FATAL ERROR"; break;
              case LL_ERROR: std::cout << "ERROR"; break; 
              case LL_WARNING: std::cout << "WARNNG";  break;
              case LL_LOG: std::cout << "LOG";  break;
       	 }
       	 std::cout << ": " << DateTimeStrFormat("%Y-%m-%d %H:%M:%S", time(NULL));
          std::cout << " " << str << std::endl;          
       }  else  {
       	  // FIXMEE !!!
       }

}


//   HTTP Request parser
// ------------------------------------------------------------


HTTP_RequestParser::HTTP_RequestParser()
  : state_(method_start)
{
}

void HTTP_RequestParser::reset()
{
  state_ = method_start;
}


boost::tribool HTTP_RequestParser::parseNextChar(HTTPrequest& req, char input)
{
  switch (state_)
  {
  case data_start:	
  case data:
      req.data.push_back(input);
      state_ = data;
      return boost::indeterminate;
  	
  case method_start:
    if (!CharType::is_char(input) || CharType::is_ctl(input) || CharType::is_tspecial(input))
    {
      return false;
    }
    else
    {
      state_ = method;
      req.method.push_back(input);
      return boost::indeterminate;
    }
  case method:
    if (input == ' ')
    {
      state_ = uri;
      return boost::indeterminate;
    }
    else if (!CharType::is_char(input) || CharType::is_ctl(input) || CharType::is_tspecial(input))
    {
      return false;
    }
    else
    {
      req.method.push_back(input);
      return boost::indeterminate;
    }
  case uri:
    if (input == ' ')
    {
      state_ = http_version_h;
      return boost::indeterminate;
    }
    else if (CharType::is_ctl(input))
    {
      return false;
    }
    else
    {
      req.uri.push_back(input);
      return boost::indeterminate;
    }
  case http_version_h:
    if (input == 'H')
    {
      state_ = http_version_t_1;
      return boost::indeterminate;
    }
    else
    {
      return false;
    }
  case http_version_t_1:
    if (input == 'T')
    {
      state_ = http_version_t_2;
      return boost::indeterminate;
    }
    else
    {
      return false;
    }
  case http_version_t_2:
    if (input == 'T')
    {
      state_ = http_version_p;
      return boost::indeterminate;
    }
    else
    {
      return false;
    }
  case http_version_p:
    if (input == 'P')
    {
      state_ = http_version_slash;
      return boost::indeterminate;
    }
    else
    {
      return false;
    }
  case http_version_slash:
    if (input == '/')
    {
      req.http_version_major = 0;
      req.http_version_minor = 0;
      state_ = http_version_major_start;
      return boost::indeterminate;
    }
    else
    {
      return false;
    }
  case http_version_major_start:
    if (CharType::is_digit(input))
    {
      req.http_version_major = req.http_version_major * 10 + input - '0';
      state_ = http_version_major;
      return boost::indeterminate;
    }
    else
    {
      return false;
    }
  case http_version_major:
    if (input == '.')
    {
      state_ = http_version_minor_start;
      return boost::indeterminate;
    }
    else if (CharType::is_digit(input))
    {
      req.http_version_major = req.http_version_major * 10 + input - '0';
      return boost::indeterminate;
    }
    else
    {
      return false;
    }
  case http_version_minor_start:
    if (CharType::is_digit(input))
    {
      req.http_version_minor = req.http_version_minor * 10 + input - '0';
      state_ = http_version_minor;
      return boost::indeterminate;
    }
    else
    {
      return false;
    }
  case http_version_minor:
    if (input == '\r')
    {
      state_ = expecting_newline_1;
      return boost::indeterminate;
    }
    else if (CharType::is_digit(input))
    {
      req.http_version_minor = req.http_version_minor * 10 + input - '0';
      return boost::indeterminate;
    }
    else
    {
      return false;
    }
  case expecting_newline_1:
    if (input == '\n')
    {
      state_ = header_line_start;
      return boost::indeterminate;
    }
    else
    {
      return false;
    }
  case header_line_start:
    if (input == '\r')
    {
      state_ = expecting_newline_3;
      return boost::indeterminate;
    }
    else if (!req.headers.empty() && (input == ' ' || input == '\t'))
    {
      state_ = header_lws;
      return boost::indeterminate;
    }
    else if (!CharType::is_char(input) || CharType::is_ctl(input) || CharType::is_tspecial(input))
    {
      return false;
    }
    else
    {
      req.headers.push_back(KeyValue());
      req.headers.back().key.push_back(input);
      state_ = header_name;
      return boost::indeterminate;
    }
  case header_lws:
    if (input == '\r')
    {
      state_ = expecting_newline_2;
      return boost::indeterminate;
    }
    else if (input == ' ' || input == '\t')
    {
      return boost::indeterminate;
    }
    else if (CharType::is_ctl(input))
    {
      return false;
    }
    else
    {
      state_ = header_value;
      req.headers.back().value.push_back(input);
      return boost::indeterminate;
    }
  case header_name:
    if (input == ':')
    {
      state_ = space_before_header_value;
      return boost::indeterminate;
    }
    else if (!CharType::is_char(input) || CharType::is_ctl(input) || CharType::is_tspecial(input))
    {
      return false;
    }
    else
    {
      req.headers.back().key.push_back(input);
      return boost::indeterminate;
    }
  case space_before_header_value:
    if (input == ' ')
    {
      state_ = header_value;
      return boost::indeterminate;
    }
    else
    {
      return false;
    }
  case header_value:
    if (input == '\r')
    {
      state_ = expecting_newline_2;
      return boost::indeterminate;
    }
    else if (CharType::is_ctl(input))
    {
      return false;
    }
    else
    {
      req.headers.back().value.push_back(input);
      return boost::indeterminate;
    }
  case expecting_newline_2:
    if (input == '\n')
    {
      state_ = header_line_start;
      return boost::indeterminate;
    }
    else
    {
      return false;
    }
  case expecting_newline_3:
    state_ = data_start;
    return boost::indeterminate;
  default:
    return false;
  }
}

bool CharType::is_char(int c)
{
  return c >= 0 && c <= 127;
}

bool CharType::is_ctl(int c)
{
  return (c >= 0 && c <= 31) || (c == 127);
}

bool CharType::is_tspecial(int c)
{
  switch (c)
  {
  case '(': case ')': case '<': case '>': case '@':
  case ',': case ';': case ':': case '\\': case '"':
  case '/': case '[': case ']': case '?': case '=':
  case '{': case '}': case ' ': case '\t':
    return true;
  default:
    return false;
  }
}

bool CharType::is_digit(int c)
{
  return c >= '0' && c <= '9';
}
