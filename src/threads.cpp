// XtraBYtes - The Proof of Signature Blocktech Superchain - http://xtrabytes.global
// Copyright (c) 2017, Zoltan Szabo. All rights reserved.
// Copyright (c) 2017, XtraBYtes Founders and Developers ( see AUTHORS )
// Licensed under GNU General Public License Version 3 or later (the "GPL")

#include <boost/thread.hpp>
#include <boost/foreach.hpp>

#include "util.h"
#include "gui.h"
#include "dicom.h"
#include "modules.h"
#include "threads.h"
#include "vitals.h"
#include "xtrabytes.h"

boost::thread_group threads;


void StartThreads() {

    threads.create_thread(boost::bind(&ThreadWrapper<void (*)()>, "ThreadSyncXbridge", &ThreadSyncXbridge));       
    threads.create_thread(boost::bind(&ThreadWrapper<void (*)()>, "ThreadServerDICOM", &ThreadServerDICOM)); 
    threads.create_thread(boost::bind(&ThreadWrapper<void (*)()>, "ThreadModuleHeartBeat", &ThreadModuleHeartBeat));
    threads.create_thread(boost::bind(&ThreadWrapper<void (*)()>, "ThreadCron_1s", &ThreadCron_1s));
}


void ThreadServerDICOM() {
     
     DICOMserver dicom("127.0.0.1", "3344", 5);    
     dicom.start();
     	  
}

void ThreadSyncXbridge(){
	 
	boost::asio::io_service ios;
    XBridge xbridge( ios );
    bool connected;
    try {
       connected = xbridge.connect( "localhost:34001", "xbyuser", "xbypass" );
    } catch (...) {
    	  LogPrint(LL_LOG,"*** XBRIDGE requires the old client wallet to be running. Please start the old wallet first !");
        throw;
    } 
       
    int blocks = 0;
    while (true) {

      if (connected) {
         blocks = xbridge.GetBlocks();
         // FIXMEE gui.displayNumConnections(blocks);
      } else {
         // FIXMEE gui.displayText("Connection problem");
      }
 
      boost::this_thread::sleep( boost::posix_time::milliseconds(1011) );
    }
}

void ThreadModuleHeartBeat() {
   while (true) {
   	XBY_Modules.PingModules();
      boost::this_thread::sleep( boost::posix_time::milliseconds(2033) );
   }
}

void ThreadCron_1s() {
   while (true) {
   	emit xGUI->guimodel->OverviewPageContentChanged();
      boost::this_thread::sleep( boost::posix_time::milliseconds(1000) );
   }
}


template <typename Callable>
void ThreadWrapper(std::string ThreadName, Callable ThreadFunc)
{
	
    try {
    	LogPrint(LL_LOG,"Thread "+ThreadName+" started");
        ThreadFunc();
        LogPrint(LL_LOG,"Thread "+ThreadName+" stopped");
    } catch (boost::thread_interrupted) {    	  
        LogPrint(LL_LOG, "Thread "+ThreadName+" interrupted.");
        throw;
    } catch (std::exception& e) {
        ExceptionPrint(&e, ThreadName.c_str());
        throw;
    } catch (...) {
        ExceptionPrint(NULL, ThreadName.c_str());
        throw;
    }
    
};
