// XtraBYtes - The Proof of Signature Blocktech Superchain - http://xtrabytes.global
// Copyright (c) 2017, Zoltan Szabo. All rights reserved.
// Copyright (c) 2017, XtraBYtes Founders and Developers ( see AUTHORS )
// Licensed under GNU General Public License Version 3 or later (the "GPL")

#include <boost/thread.hpp>
#include <boost/asio.hpp>

#include "util.h"
#include "gui.h"
#include "threads.h"
#include "vitals.h"


boost::thread_group threads;


void StartThreads() {

    threads.create_thread(boost::bind(&ThreadWrapper<void (*)()>, "ThreadSyncXbridge", &ThreadSyncXbridge));        
}

void ThreadSyncXbridge(){
	 
	boost::asio::io_service ios;
    XBridge xbridge( ios );
    bool connected = xbridge.connect( "localhost:34001", "xbyuser", "xbypass" );   
    int blocks = 0;
    while (true) {

      if (connected) {
         blocks = xbridge.GetBlocks();
         gui.displayNumConnections(blocks);
      } else {
         gui.displayText("Connection problem");
      }
 
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
