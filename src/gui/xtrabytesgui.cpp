// XtraBYtes - The Proof of Signature Blocktech Superchain - http://xtrabytes.global
// Copyright (c) 2017, Zoltan Szabo. All rights reserved.
// Copyright (c) 2017, XtraBYtes Founders and Developers ( see AUTHORS )
// Licensed under GNU General Public License Version 3 or later (the "GPL")

#include <QtWidgets>
#include <QMenuBar>
#include <QStatusBar>
#include <QCloseEvent>
#include <QMessageBox>
#include <QStackedWidget>

#include "xtrabytesgui.h"
#include "aboutdialog.h"
#include "overviewpage.h"
#include "modulespage.h"
#include "models/guimodel.h"
#include "../gui.h"

XtraBYtesGUI::XtraBYtesGUI(QWidget *parent):
    QMainWindow(parent)
{
  
	#ifndef Q_OS_MAC
	    qApp->setWindowIcon(QIcon(":icons/xtrabytesicon"));
	    setWindowIcon(QIcon(":icons/xtrabytesicon"));
	#else
	    //setUnifiedTitleAndToolBarOnMac(true);
	    QApplication::setAttribute(Qt::AA_DontShowIconsInMenus);
	#endif

    createStatusBar();
    createActions();
    createMenus();
    createToolBars();
    
    setWindowTitle(tr("XtraBYtes Community Corporation Blockchain System"));
    setMinimumSize(900, 600);
    resize(900, 600);

    setStyleSheet("background-color: #2381c9; color: #ffffff; ");
    
    setUnifiedTitleAndToolBarOnMac(true);

    overviewPage = new OverviewPage();
    modulesPage = new ModulesPage();
    
    guimodel = new GuiModel();
    guimodel->init( overviewPage, modulesPage );
        
    
    centralStackedWidget = new QStackedWidget(this);
    centralStackedWidget->addWidget(overviewPage);
    centralStackedWidget->addWidget(modulesPage);

    QWidget *centralWidget = new QWidget();
    QVBoxLayout *centralLayout = new QVBoxLayout(centralWidget);

    centralLayout->addWidget(centralStackedWidget);
    setCentralWidget(centralWidget);

    gotoOverviewPage();
}

XtraBYtesGUI::~XtraBYtesGUI() {
}

void XtraBYtesGUI::createActions() {

    QActionGroup *tabGroup = new QActionGroup(this);    
       
    aboutAction = new QAction(QIcon(":/icons/about"),tr("&About XtraBYtes"), this);
    aboutAction->setToolTip(tr("Show information about XtraBYtes"));
    aboutAction->setShortcut( tr("Ctrl+A") );
    tabGroup->addAction(aboutAction);
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(aboutClicked()));            

    overviewAction = new QAction(QIcon(":/icons/dashboard"), tr("&Dashboard"), this);
    overviewAction->setToolTip(tr("Show general overview of XtraBYtes system"));
    overviewAction->setCheckable(true);
    overviewAction->setShortcut(QKeySequence(Qt::ALT + Qt::Key_1));
    tabGroup->addAction(overviewAction);    
    connect(overviewAction, SIGNAL(triggered()), this, SLOT(gotoOverviewPage()));

    modulesAction = new QAction(QIcon(":/icons/modules"), tr("&Modules"), this);
    modulesAction->setToolTip(tr("Show general overview of modules."));
    modulesAction->setCheckable(true);
    modulesAction->setShortcut(QKeySequence(Qt::ALT + Qt::Key_1));
    tabGroup->addAction(modulesAction);    
    connect(modulesAction, SIGNAL(triggered()), this, SLOT(gotoModulesPage()));

    
    exitAction = new QAction( QIcon(":/icons/exit"), tr("E&xit"), this );
    exitAction->setShortcut( tr("Ctrl+Q") );
    exitAction->setToolTip( tr("Quit the application") );
    tabGroup->addAction(exitAction);
    connect( exitAction, SIGNAL(triggered()), qApp, SLOT(closeAllWindows()) );

}

void XtraBYtesGUI::createMenus() {
		    		     	 
	    
    QMenu *exitMenu = new QMenu(tr("&Exit"));
    exitMenu->addAction(exitAction);        
    menuBar()->addMenu(exitMenu);
    
    
}

static QWidget* makeToolBarSpacer()
{
    QWidget* spacer = new QWidget();
    spacer->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    spacer->setStyleSheet("QWidget { background: none; }");
    return spacer;
}

void XtraBYtesGUI::createToolBars()
{
    toolbar = new QToolBar(tr("Tabs toolbar"));
    toolbar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    int maxwidth = 180;

    QLabel *l = new QLabel();
    
    l->setScaledContents(true);
    l->setPixmap(QPixmap(":/images/menuimage"));
    
    toolbar->addWidget(l);
    
    toolbar->addAction(overviewAction);
    toolbar->addAction(modulesAction);
    toolbar->addAction(aboutAction);
    toolbar->addAction(exitAction);    
    toolbar->addWidget(makeToolBarSpacer());

    toolbar->setOrientation(Qt::Vertical);
    toolbar->setMovable(false);    

    addToolBar(Qt::LeftToolBarArea, toolbar);    

    foreach(QAction *action, toolbar->actions()) {
        toolbar->widgetForAction(action)->setFixedWidth(maxwidth);
    }
}

void XtraBYtesGUI::createStatusBar() {
    statusBar()->showMessage(tr("Ready"));
}

void XtraBYtesGUI::aboutClicked()
{
    AboutDialog dlg;
    dlg.exec();
}

void XtraBYtesGUI::gotoOverviewPage()
{
    centralStackedWidget->setCurrentWidget(overviewPage);
}


void XtraBYtesGUI::gotoModulesPage()
{
    centralStackedWidget->setCurrentWidget(modulesPage);
}



