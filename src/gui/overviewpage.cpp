// XtraBYtes - The Proof of Signature Blocktech Superchain - http://xtrabytes.global
// Copyright (c) 2017, Zoltan Szabo. All rights reserved.
// Copyright (c) 2017, XtraBYtes Founders and Developers ( see AUTHORS )
// Licensed under GNU General Public License Version 3 or later (the "GPL")

#include "overviewpage.h"
#include "ui_overviewpage.h"


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

void OverviewPage::SetlabelAlerts(QString qs) {
   ui->labelAlerts->setText(qs);
}


