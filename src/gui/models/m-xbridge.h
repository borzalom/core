// XtraBYtes - The Proof of Signature Blocktech Superchain - http://xtrabytes.global
// Copyright (c) 2017, Zoltan Szabo. All rights reserved.
// Copyright (c) 2017, XtraBYtes Founders and Developers ( see AUTHORS )
// Licensed under GNU General Public License Version 3 or later (the "GPL")

#ifndef M_XBRIDGE_H
#define M_XBRIDGE_H

#include <QObject>

class XBridgeModel : public QObject  {
	
   Q_OBJECT

	public:
      explicit XBridgeModel(QObject *parent = 0);
      ~XBridgeModel();

	private:
		void ConnectModelSignals();
		void DisconnectModelSignals();
		
	signals:
    	void changed_numBlocks(int numBlocks);

	public slots:
    	void update_numBlocks(int numBlocks);

};





#endif // M_XBRIDGE_H
