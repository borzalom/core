// XtraBYtes - The Proof of Signature Blocktech Superchain - http://xtrabytes.global
// Copyright (c) 2017, Zoltan Szabo. All rights reserved.
// Copyright (c) 2017, XtraBYtes Founders and Developers ( see AUTHORS )
// Licensed under GNU General Public License Version 3 or later (the "GPL")

#include "m-xbridge.h"
#include "../gui.h"
#include "overviewpage.h"

XBridgeModel::XBridgeModel(QObject *parent) :
    QObject(parent)
{ 
    ConnectModelSignals();
}
 
XBridgeModel::~XBridgeModel()
{
    DisconnectModelSignals();
}

void XBridgeModel::ConnectModelSignals(){
}

void XBridgeModel::DisconnectModelSignals(){
}

void XBridgeModel::update_numBlocks(int numBlocks)
{	
    emit changed_numBlocks(numBlocks);
}