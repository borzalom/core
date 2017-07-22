// XtraBYtes - The Proof of Signature Blocktech Superchain - http://xtrabytes.global
// Copyright (c) 2017, Zoltan Szabo. All rights reserved.
// Copyright (c) 2017, XtraBYtes Founders and Developers ( see AUTHORS )
// Licensed under GNU General Public License Version 3 or later (the "GPL")

#ifndef XTRABYTESGUI_H
#define XTRABYTESGUI_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QObject>

#include <boost/foreach.hpp>

#include "aboutdialog.h"
#include "overviewpage.h"
#include "modulespage.h"
#include "gui/models/guimodel.h"

class QAction;
class QMenu;
class QPlainTextEdit;
class QSessionManager;

class OverviewPage;

class XtraBYtesGUI : public QMainWindow
{
    Q_OBJECT

public:
    explicit XtraBYtesGUI(QWidget *parent = 0);
    ~XtraBYtesGUI();
    GuiModel *guimodel;

private:
    
    QToolBar *toolbar;

    QStackedWidget *centralStackedWidget;

    OverviewPage *overviewPage;
    ModulesPage *modulesPage;        

    void createMenus();
    void createStatusBar();
    void createActions();
    void createToolBars();
    
    QAction *aboutAction;
    QAction *overviewAction;
    QAction *modulesAction;
    QAction *exitAction;

private slots:    
    
    void aboutClicked();
    void gotoOverviewPage();
    void gotoModulesPage();

    
};

#endif