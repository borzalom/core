// XtraBYtes - The Proof of Signature Blocktech Superchain - http://xtrabytes.global
// Copyright (c) 2017, Zoltan Szabo. All rights reserved.
// Copyright (c) 2017, XtraBYtes Founders and Developers ( see AUTHORS )
// Licensed under GNU General Public License Version 3 or later (the "GPL")

#include "guimodel.h"
#include "../gui.h"

GuiModel::GuiModel(QObject *parent ) :
    QObject(parent)
{ 
   ModelSignalsConnected = false;
}
 
void GuiModel::init(OverviewPage *overviewPage , ModulesPage *modulesPage )  {
	  GuiModel::overviewPage = overviewPage; 
	  GuiModel::modulesPage = modulesPage;
     ConnectModelSignals();
     ModelSignalsConnected = true;
} 
 
GuiModel::~GuiModel()
{
	if (ModelSignalsConnected) {
	   DisconnectModelSignals();
	}
    
}

void GuiModel::ConnectModelSignals(){
	 connect( this, SIGNAL(ModuleListChanged()), modulesPage, SLOT(UpdateModuleWidgets()));
	 connect( this, SIGNAL(OverviewPageContentChanged()), overviewPage, SLOT(UpdateOverviewPageContent()));
	 
	  
}

void GuiModel::DisconnectModelSignals(){
}

