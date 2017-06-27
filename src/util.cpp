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

#include <boost/date_time/posix_time/posix_time.hpp>

const char* const HEX_CHARS = "0123456789ABCDEF";
const char* const BASE64_CHARS = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";


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


void LogPrint(LogLevel loglevel, const std::string &str) {
	
    if (!WriteConsoleLog) return;

    switch (loglevel) {
        case LL_FATAL_ERROR: std::cout << "FATAL ERROR"; break;
        case LL_ERROR: std::cout << "ERROR"; break; 
        case LL_WARNING: std::cout << "WARNNG";  break;
        case LL_LOG: std::cout << "LOG";  break;
    }

    std::cout << ": " << DateTimeStrFormat("%Y-%m-%d %H:%M:%S", time(NULL));
    std::cout << " " << str << std::endl;          
}
