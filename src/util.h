// XtraBYtes - The Proof of Signature Blocktech Superchain - http://xtrabytes.global
// Copyright (c) 2017, Zoltan Szabo. All rights reserved.
// Copyright (c) 2017, XtraBYtes Founders and Developers ( see AUTHORS )
// Licensed under GNU General Public License Version 3 or later (the "GPL")

#ifndef XBY_UTIL_H
#define XBY_UTIL_H

#include <string>
#include <time.h>

extern const char* const HEX_CHARS;
extern const char* const BASE64_CHARS;

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

#endif // XBY_UTIL_H
