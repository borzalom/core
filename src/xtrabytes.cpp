// XtraBYtes - The Proof of Signature Blocktech Superchain - http://xtrabytes.global
// Copyright (c) 2017, Zoltan Szabo. All rights reserved.
// Copyright (c) 2017, XtraBYtes Founders and Developers ( see AUTHORS )
// Licensed under GNU General Public License Version 3 or later (the "GPL")

#include <QApplication>
#include <QPushButton>

#include <boost/algorithm/string.hpp>    
#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/foreach.hpp>

#include "xtrabytes.h"
#include "genesis.h"
#include "threads.h"
#include "gui/xtrabytesgui.h"
#include "gui/models/m-xbridge.h"

using namespace std;
using namespace boost;
  
boost::mutex SettingsMutex;
std::map<std::string, std::vector<std::string> > SettingsMap;    

int main(int argc, char **argv)
{
    
  ParseCommandLine( argc , argv );  

  if ( SettingsMap["nogui"].size() > 0 || SettingsMap["help"].size() > 0 ) {
    std::cout << std::endl << "Welcome to XtraBYtes command line mode." << std::endl;

    if ( SettingsMap["help"].size() > 0 ) {
       std::cout << std::endl << "Commands:" << std::endl;
       std::cout << "help" << std::endl;
       std::cout << "getstaticbyid" << std::endl;
       std::cout << "getstaticbypubkey" << std::endl << std::endl;
    }
    
    if ( GetSettingsStringValue("getstaticbyid").size() > 0 ) {
    	  
        std::cout << std::endl << Genesis_Block::GetSTaTiCbyID(GetSettingsStringValue("getstaticbyid").c_str()) << std::endl;
    }

    if ( GetSettingsStringValue("getstaticbypubkey").size() > 0 ) {
    	  
        std::cout << std::endl << Genesis_Block::GetSTaTiCbyPubKey(GetSettingsStringValue("getstaticbypubkey").c_str()) << std::endl;
    }

  } else {
  	
	  QApplication app (argc, argv);
      XtraBYtesGUI xtrabytesGUI;
     
      XBridgeModel xbridgemodel;      
     
      xtrabytesGUI.setXBridgeModel(&xbridgemodel);	  
	  

	  xtrabytesGUI.show();
	  StartThreads();
	  return app.exec();  
  
  }  
    	
  return 1;  	
}

void ParseCommandLine(int argc, const char* const argv[])
{

    SettingsMutex.lock();
    SettingsMap.clear();

    for (int i = 1; i < argc; i++)
    {
        std::string str(argv[i]);
        std::string strValue;
        size_t is_index = str.find('=');
        if (is_index != std::string::npos)
        {
            strValue = str.substr(is_index+1);
            str = str.substr(0, is_index);
        }
        #ifdef WIN32
          boost::to_lower(str);
          if (boost::algorithm::starts_with(str, "/"))
            str = "-" + str.substr(1);
        #endif

        if (str[0] != '-')
            break;

        if (str.length() > 1 && str[1] == '-')
            str = str.substr(1);

        str = str.substr(1);         
        
        SettingsMap[str].push_back(strValue);
    }
     
   SettingsMutex.unlock(); 
}

// FIXED - onedeveloper/nitroxido

std::string GetSettingsStringValue(std::string id ) {
  std:string result; // the result
  try {
    result = SettingsMap.at(id); // try to find the id in the map
  }
  catch (const std::out_of_range& oor) {
    result = ""; // not found, use standard value
  }
  return result; // always return something
}

std::string GetSettingsStringValue(std::string id ) {
  BOOST_FOREACH(std::string str, SettingsMap[id]) {
  	  return str;
  }	
  return "";
}
