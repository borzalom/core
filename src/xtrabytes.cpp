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
#include "dicom.h"
#include "gui/models/guimodel.h"

using namespace std;
using namespace boost;
  
boost::mutex SettingsMutex;
std::map<std::string, std::vector<std::string> > SettingsMap;    
XtraBYtesGUI *xGUI;

int main(int argc, char **argv)
{
      	
	  QApplication app (argc, argv);

     XtraBYtesGUI xtrabytesGUI;     	  
	  xtrabytesGUI.show();
	  xGUI = &xtrabytesGUI;
	  StartThreads();
	  
	  return app.exec();  
  
   	
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

// FIXMEE!! TMP FUNCTIONS

std::string GetSettingsStringValue(std::string id ) {
  BOOST_FOREACH(std::string str, SettingsMap[id]) {
  	  return str;
  }	
  return "";
}
