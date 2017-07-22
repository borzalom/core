// XtraBYtes - The Proof of Signature Blocktech Superchain - http://xtrabytes.global
// Copyright (c) 2017, Zoltan Szabo. All rights reserved.
// Copyright (c) 2017, XtraBYtes Founders and Developers ( see AUTHORS )
// Licensed under GNU General Public License Version 3 or later (the "GPL")


#include "xtrabytes.h"
#include "modules.h"
#include "dicom.h"


boost::property_tree::ptree XBYmodules::ModuleRegistration(boost::property_tree::ptree JSONrequest) {

    boost::property_tree::ptree JSONreply;
    std::string ModuleID = JSONrequest.get<std::string>("module-id");    
    
    bool ModuleExist = false;
    for( std::vector<XBYmodule*>::iterator itr = modules.begin(), itr_end = modules.end(); itr != itr_end; ++itr ) {
       XBYmodule *m = *itr;  
       if(!ModuleID.compare(m->ModuleID)) {
          ModuleExist = true;
          break;
       } 
    }
    
    if ( ModuleExist ) {
    	 JSONreply.put("DICOM", "STOP FLOOD!");
    } else {
	    modules.push_back(new XBYmodule());
	    XBYmodule *xby_module = modules.back();
	    
	    xby_module->DICOMport = JSONrequest.get<int>("dicom-port");
	    xby_module->ModuleName = JSONrequest.get<std::string>("module-name"); 
	    xby_module->ModuleID = ModuleID;
	    xby_module->LastHeartbeat = 0;
	    xby_module->ModuleEnable = false;
	    boost::property_tree::ptree::value_type v;
	    BOOST_FOREACH(boost::property_tree::ptree::value_type &v, JSONrequest.get_child("signals")) {            
          xby_module->ModuleSignals.push_back(GetSignalByName(v.second.data()));
       }	    
	    
	    emit xGUI->guimodel->ModuleListChanged();    	      
	        
	    JSONreply.put("DICOM", "Module accepted");         
    }    
    
    
    return JSONreply;
}

XBYmodule::XBYmodule() {
  
}

XBYmodules::~XBYmodules() {
	 while(!modules.empty()) {
        delete modules.back();
        modules.pop_back();
    }
}

void XBYmodules::PingModules() {
	
	  for( std::vector<XBYmodule*>::iterator itr = modules.begin(), itr_end = modules.end(); itr != itr_end; ++itr ) {        
        boost::asio::io_service ios;
        DICOMclient dicomclient( ios );
        dicomclient.HeartbeatEchoRequest(*itr); 
        if  ((time(NULL) - (*itr)->LastHeartbeat) > 4 ) {
        	    if ((*itr)->ModuleAvailable) {
        	    	LogPrint(LL_LOG,"Module "+(*itr)->ModuleID+" unavailable. Temporary disabled.");
        	    } 
             (*itr)->ModuleAvailable = false;
        } else {
        	    if (!(*itr)->ModuleAvailable) {
        	    	LogPrint(LL_LOG,"Module "+(*itr)->ModuleID+" available. Reenabled.");
        	    } 
            (*itr)->ModuleAvailable = true;
        }
        
                 
     }	  

}

boost::property_tree::ptree XBYmodules::ModuleInvoke( std::string moduleID , MODULE_SIGNAL Signal , boost::property_tree::ptree *args ) {

   boost::property_tree::ptree reply_warapper;
	for( std::vector<XBYmodule*>::iterator itr = modules.begin(), itr_end = modules.end(); itr != itr_end; ++itr ) {
		
	   if (((moduleID.compare("*")==0) || (moduleID.compare((*itr)->ModuleID)==0)) && (*itr)->ModuleAvailable) {
		   for( std::vector<MODULE_SIGNAL>::iterator sitr = (*itr)->ModuleSignals.begin(), sitr_end = (*itr)->ModuleSignals.end(); sitr != sitr_end; ++sitr ) {
		   	MODULE_SIGNAL ms = *sitr;		   	
		      if (GetSignalName(Signal).compare(GetSignalName(ms))==0) {		      	 	      	 		      	 
                boost::asio::io_service ios;
                DICOMclient dicomclient( ios );
                args->put("DICOM", GetSignalName(ms));
                boost::property_tree::ptree module_reply = dicomclient.ModuleRequest( *itr, args);                
                reply_warapper.put_child((*itr)->ModuleID, module_reply);            
		      }
		   }
		   	   
	   } 
	     
	   
	}
   return reply_warapper;

   
}

XBYmodules XBY_Modules;
