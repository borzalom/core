// XtraBYtes - The Proof of Signature Blocktech Superchain - http://xtrabytes.global
// Copyright (c) 2017, Zoltan Szabo. All rights reserved.
// Copyright (c) 2017, XtraBYtes Founders and Developers ( see AUTHORS )
// Licensed under GNU General Public License Version 3 or later (the "GPL")

#ifndef GUIMODEL_H
#define GUIMODEL_H

#include <QObject>

#include "../overviewpage.h"
#include "../modulespage.h"


class GuiModel : public QObject  {
	
   Q_OBJECT

	public:
      explicit GuiModel(QObject *parent = 0);
      void init(OverviewPage *overviewPage , ModulesPage *modulesPage );
      ~GuiModel();

	private:
		void ConnectModelSignals();
		void DisconnectModelSignals();
		bool ModelSignalsConnected;
		OverviewPage *overviewPage; 
		ModulesPage *modulesPage;
		
   signals:
      void ModuleListChanged();  
      void OverviewPageContentChanged();         

};





#endif // GUIMODEL_H
