// XtraBYtes - The Proof of Signature Blocktech Superchain - http://xtrabytes.global
// Copyright (c) 2017, Zoltan Szabo. All rights reserved.
// Copyright (c) 2017, XtraBYtes Founders and Developers ( see AUTHORS )
// Licensed under GNU General Public License Version 3 or later (the "GPL")

#ifndef MODULESPAGE_H
#define MODULESPAGE_H

#include <QDialog>
#include <QLabel>
#include <QString>
#include <QStateMachine>
#include <QPushButton>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QTextStream>

#include <map>

#include "../modules.h"

class ModuleContainer;
class GuiModel;

namespace Ui {
    class ModulesPage;
}

class ModulesPage : public QWidget
{
    Q_OBJECT

public:
    explicit ModulesPage(QWidget *parent = 0);
    ~ModulesPage();
    
    

private:
    Ui::ModulesPage *ui;
    GuiModel *gui_model;
    QGridLayout *modulelistlayout;        
    
    //std::vector<ModuleContainer*> ModuleContainerWrapper;
    
    std::map< std::string, std::vector<ModuleContainer*> > ModuleContainerWrapper;
    
public slots:
    void UpdateModuleWidgets();

};


class ModuleContainer {
    public:
        ModuleContainer(XBYmodule *m) {

            module = m;
                
    			off = new QState();
    			off->assignProperty(&OnOffButton, "text", "Off");
    			off->setObjectName("off");

    			on = new QState();
    			on->setObjectName("on");
    			on->assignProperty(&OnOffButton, "text", "On");

    			off->addTransition(&OnOffButton, SIGNAL(clicked()), on);
    			on->addTransition(&OnOffButton, SIGNAL(clicked()), off);

    			OnOffState.addState(off);
    			OnOffState.addState(on);
    			OnOffState.setInitialState(off);
    			OnOffState.start();

    			OnOffButton.resize(100, 50);
    			OnOffButton.show();  

        }

        QPushButton* GetOnOffButtonAddr() {
          return &OnOffButton;
        }
       
        QLabel* GetModuleDescription() {

        QLabel *description = new QLabel();
            
        QString desc;
        QTextStream(&desc) << "<p><b>" << module->ModuleName.c_str() << "</b>(" << module->ModuleID.c_str() << ")</p>";
            
        description->setText(desc);

          return description;
        }

        XBYmodule *module;
        QPushButton OnOffButton;
        QStateMachine OnOffState;
        QState *off;
        QState *on;    

    };

#endif // MODULESPAGE_H
