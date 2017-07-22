// XtraBYtes - The Proof of Signature Blocktech Superchain - http://xtrabytes.global
// Copyright (c) 2017, Zoltan Szabo. All rights reserved.
// Copyright (c) 2017, XtraBYtes Founders and Developers ( see AUTHORS )
// Licensed under GNU General Public License Version 3 or later (the "GPL")

#include "modulespage.h"
#include "ui_modulespage.h"

#include <QtWidgets>
#include <QPushButton>
#include <QStateMachine>
#include <QList>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <vector>

#include <boost/foreach.hpp>

ModulesPage::ModulesPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ModulesPage)
{	
    ui->setupUi(this);
    
    modulelistlayout = new QGridLayout();
    modulelistlayout->setMargin(10);
    ui->contentLayout->addLayout(modulelistlayout);      
            
    //emit ModuleListChanged();
      
    QHBoxLayout* bottomLayout = new QHBoxLayout();
    QPushButton* btn1 = new QPushButton(QObject::tr("OK"));
    bottomLayout->addStretch();
    bottomLayout->addWidget(btn1);
    bottomLayout->addStretch();
    ui->contentLayout->addLayout(bottomLayout);
    
    
}

ModulesPage::~ModulesPage() {

	std::map<std::string, std::vector<ModuleContainer*> >::iterator itr = ModuleContainerWrapper.begin();
	while (itr != ModuleContainerWrapper.end()) { 
		    std::vector<ModuleContainer*> ModuleContainer = (*itr).second;
		    LogPrint(LL_LOG,"ModulesPage: "+ModuleContainer.back()->module->ModuleID+" removed.");
	       while(!ModuleContainer.empty()) {
            delete ModuleContainer.back();
            ModuleContainer.pop_back();
          }	     
	       ModuleContainerWrapper.erase(itr++);  
	}

        
   delete ui;
}

void ModulesPage::UpdateModuleWidgets() {

    int i = 0;
    for( std::vector<XBYmodule*>::iterator itr = XBY_Modules.modules.begin(), itr_end = XBY_Modules.modules.end(); itr != itr_end; ++itr ) {
    	
    	ModuleContainer *module_container = new ModuleContainer(*itr);
    	ModuleContainerWrapper[(*itr)->ModuleID].push_back( module_container );    	
    	
		modulelistlayout->addWidget(module_container->GetModuleDescription(),i,0);
	   modulelistlayout->addWidget(module_container->GetOnOffButtonAddr(),i,1);
    	
    	i++;        
    }
	      
}


 