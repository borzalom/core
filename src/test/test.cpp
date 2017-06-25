// XtraBYtes - The Proof of Signature Blocktech Superchain - http://xtrabytes.global
// Copyright (c) 2017, Zoltan Szabo. All rights reserved.
// Copyright (c) 2017, XtraBYtes Founders and Developers ( see AUTHORS )
// Licensed under GNU General Public License Version 3 or later (the "GPL")

#define BOOST_TEST_MODULE XtraBYtes_Tests
#include <boost/test/included/unit_test.hpp>

#include <boost/asio.hpp>

#include "../util.h"
#include "../crypto.h"
#include "../redfat.h"
#include "../vitals.h"
#include "../threads.h"

const char* TestDataBin = "The Proof of Signature Blockchain Revolution";
const char* TestDataHex = "5468652050726F6F66206F66205369676E617475726520426C6F636B636861696E205265766F6C7574696F6E";
const char* TestDataBase64 = "VGhlIFByb29mIG9mIFNpZ25hdHVyZSBCbG9ja2NoYWluIFJldm9sdXRpb24=";
const char* EmptyStr = "";

BOOST_AUTO_TEST_CASE( hexstring_test )
{
	 BOOST_TEST_MESSAGE( "Hex string test:" );
	 HexString hexstring;
	 BOOST_REQUIRE( hexstring.Wipe() == false );	 
    BOOST_REQUIRE( hexstring.Allocate((unsigned)strlen(TestDataBin)) == true );    
    BOOST_REQUIRE( hexstring.SetBin((char*)TestDataBin) == true );
    BOOST_REQUIRE( strcmp(hexstring.toString().c_str(),TestDataHex) == 0 );
    BOOST_REQUIRE( hexstring.Wipe() == true );
    // BOOST_REQUIRE( hexstring.SetHex((char*)TestDataHex) == true );     
	
	/**
	*Test negative size
	*/
	
	BOOST_TEST_MESSAGE("Negative test:");
	BOOST_REQUIRE ( hexstring.Allocate(-1) == false);
	
	/**
	*Test zero size
	*/
	
	BOOST_TEST_MESSAGE("Zero test:");
	BOOST_REQUIRE( hexstring.Allocate(0) == false);
	BOOST_REQUIRE( hexstring.SetBin((char*)EmptyStr) == false);
	BOOST_REQUIRE( hexstring.SetHex((char*)EmptyStr) == false);
	BOOST_REQUIRE( hexstring.Wipe() == false);
	BOOST_REQUIRE(strcmp(hexstring.toString().c_str(), "HexString is null") == 0);
	
}

BOOST_AUTO_TEST_CASE( encode_decode_tests )
{
  
  Encoders encoders;   
  Decoders decoders;
  
  BOOST_TEST_MESSAGE( "Base64 encode/decode tests:" );
  BOOST_REQUIRE( strcmp(encoders.Base64_Encode((unsigned char *)TestDataBin, (unsigned)strlen(TestDataBin)).c_str(),TestDataBase64) == 0 );
  BOOST_REQUIRE( strcmp(decoders.Base64_Decode((char *)TestDataBase64).c_str(),TestDataBin) == 0 );
}

BOOST_AUTO_TEST_CASE( random_number_test )
{
	 BOOST_TEST_MESSAGE( "Random number test:" );
	 RandomNumber randomnumber;
	 for( int i = 0; i < 100; ++i) {
	   BOOST_REQUIRE( randomnumber.GetNextRandomNumber() != randomnumber.GetNextRandomNumber() );
	 }	   
}


BOOST_AUTO_TEST_CASE( crypto_test )
{
  
  BOOST_TEST_MESSAGE( "Crypto test:" );   
  HexString privkey; 

  CryptoKey cryptokey; 
  cryptokey.NewKey();
  
  BOOST_REQUIRE( cryptokey.GetPrivKey( &privkey ) == true );
  BOOST_TEST_MESSAGE( "PrivKey:" << privkey.toString() );
}

BOOST_AUTO_TEST_CASE( redfat_test )
{
  
  BOOST_TEST_MESSAGE( "REdundant Distributed FAult Tolerant Database tests:" );   
  REDFAT redfat; 

}

BOOST_AUTO_TEST_CASE( xbridge_test )
{  
    boost::asio::io_service ios;
    XBridge xbridge( ios );
    BOOST_REQUIRE( xbridge.connect( "localhost:34001", "xbyuser", "xbypass" ) == true );   
    BOOST_TEST_MESSAGE( "Blocks:" << xbridge.GetBlocks() );
}

BOOST_AUTO_TEST_CASE( log_test ) {
   LogPrint(LOG,TestDataBin);
}

BOOST_AUTO_TEST_CASE( threads_test ) {
   StartThreads();
}

