// XtraBYtes - The Proof of Signature Blocktech Superchain - http://xtrabytes.global
// Copyright (c) 2017, Zoltan Szabo. All rights reserved.
// Copyright (c) 2017, XtraBYtes Founders and Developers ( see AUTHORS )
// Licensed under GNU General Public License Version 3 or later (the "GPL")

#include "util.h"
#include <cstdlib>
#include <memory.h>
#include <string>

const char* const HEX = "0123456789ABCDEF";

//      HexString
// ------------------------------------------------------------


bool HexString::Allocate(int Dsize) {
	size = Dsize;
   HEXdata = (char *)malloc(Dsize);
   IsData = (HEXdata != NULL);
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
   if ( IsData || size>=1 ) {
     return false;
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
		
    std::string output;
    output.reserve(2 * size);
    for (int i = 0; i < size; ++i)
    {
        const unsigned char c = HEXdata[i];
        output.push_back(HEX[c >> 4]);
        output.push_back(HEX[c & 15]);
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
