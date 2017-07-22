// XtraBYtes - The Proof of Signature Blocktech Superchain - http://xtrabytes.global
// Copyright (c) 2017, Zoltan Szabo. All rights reserved.
// Copyright (c) 2017, XtraBYtes Founders and Developers ( see AUTHORS )
// Licensed under GNU General Public License Version 3 or later (the "GPL")

#include "overviewpage.h"
#include "ui_overviewpage.h"

#include "../modules.h"

#include <QtWidgets>



OverviewPage::OverviewPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OverviewPage)
{	
    ui->setupUi(this);    
    
}

OverviewPage::~OverviewPage()
{
    delete ui;
}

void OverviewPage::UpdateOverviewPageContent() {

   boost::property_tree::ptree args;
   // FIXMEE!! need send back to module the content of this region
   args.put("UpdateOverviewPageContent", "UpdateOverviewPageContent-FIXMEE!");
   boost::property_tree::ptree ret = XBY_Modules.ModuleInvoke("*" , view_page_overview_content, &args );

   std::stringstream ss;   
   boost::property_tree::ptree::const_iterator end = ret.end();
   for (boost::property_tree::ptree::const_iterator it = ret.begin(); it != end; ++it) {
      try {
        ss << "<p>" << it->second.get<std::string>("view-page-overview-content") << "</p>";
      } catch(...) {}                     
   }
   
   ui->labelContent->setText(QString::fromStdString(ss.str()));

}


