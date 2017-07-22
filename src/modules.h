// XtraBYtes - The Proof of Signature Blocktech Superchain - http://xtrabytes.global
// Copyright (c) 2017, Zoltan Szabo. All rights reserved.
// Copyright (c) 2017, XtraBYtes Founders and Developers ( see AUTHORS )
// Licensed under GNU General Public License Version 3 or later (the "GPL")


#ifndef XBY_MODULES_H
#define XBY_MODULES_H

#include <boost/property_tree/ptree.hpp>
#include <map>
#include <vector>

#include "dicom.h"

class XBYmodule {
	
	public:
		XBYmodule();
				
		std::vector<MODULE_SIGNAL> ModuleSignals;
		int DICOMport;
		std::string ModuleName;    
		std::string ModuleID;
		unsigned long int LastHeartbeat;
		bool ModuleEnable;
		bool ModuleAvailable;
};

class XBYmodules {
	
  public:	
    ~XBYmodules();
    void PingModules();
    boost::property_tree::ptree ModuleInvoke( std::string moduleID , MODULE_SIGNAL Signal , boost::property_tree::ptree *args );
    boost::property_tree::ptree ModuleRegistration(boost::property_tree::ptree JSONrequest);        
    std::vector<XBYmodule*> modules;
    
};

extern XBYmodules XBY_Modules;

#endif // XBY_MODULES_H
