// XtraBYtes - The Proof of Signature Blocktech Superchain - http://xtrabytes.global
// Copyright (c) 2017, Zoltan Szabo. All rights reserved.
// Copyright (c) 2017, XtraBYtes Founders and Developers ( see AUTHORS )
// Licensed under GNU General Public License Version 3 or later (the "GPL")


#include <QString>

#include "gui.h"

void GUI::displayNumConnections(int i) {
  if ( overviewPage != NULL ) {
      overviewPage->SetlabelAlerts(QString::number(i));  
  }	

}

void GUI::displayText(std::string text) {
  if ( overviewPage != NULL ) {
      overviewPage->SetlabelAlerts(QString::fromUtf8(text.c_str()));  
  }	

}


void GUI::initOverviewPage(OverviewPage *op ) {
     overviewPage = op;    
}

GUI gui;


// XBridgeInterface xbridgeinterface; 

 