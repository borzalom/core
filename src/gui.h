// XtraBYtes - The Proof of Signature Blocktech Superchain - http://xtrabytes.global
// Copyright (c) 2017, Zoltan Szabo. All rights reserved.
// Copyright (c) 2017, XtraBYtes Founders and Developers ( see AUTHORS )
// Licensed under GNU General Public License Version 3 or later (the "GPL")

#ifndef XBY_GUI_H
#define XBY_GUI_H

#include "xtrabytesgui.h"
#include "overviewpage.h"

class GUI {
   public:
     void displayNumConnections(int i);
     void displayText(std::string text);
     void initOverviewPage(OverviewPage *op );
   private:
     OverviewPage *overviewPage;    
     XBridgeModel *xbridgemodel;

};

extern GUI gui;

#endif // XBY_GUI_H
