// XtraBYtes - The Proof of Signature Blocktech Superchain - http://xtrabytes.global
// Copyright (c) 2017, Zoltan Szabo. All rights reserved.
// Copyright (c) 2017, XtraBYtes Founders and Developers ( see AUTHORS )
// Licensed under GNU General Public License Version 3 or later (the "GPL")

#include <QApplication>
#include <QPushButton>

#include "xtrabytesgui.h"
 
int main(int argc, char **argv)
{
  QApplication app (argc, argv);
  
  XtraBYtesGUI xtrabytesGUI;
  xtrabytesGUI.show();
  return app.exec();
}