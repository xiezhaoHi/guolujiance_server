/********************************************************************************
** Form generated from reading UI file 'h2smonitorserver.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_H2SMONITORSERVER_H
#define UI_H2SMONITORSERVER_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_H2SMonitorServerClass
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *H2SMonitorServerClass)
    {
        if (H2SMonitorServerClass->objectName().isEmpty())
            H2SMonitorServerClass->setObjectName(QStringLiteral("H2SMonitorServerClass"));
        H2SMonitorServerClass->resize(600, 400);
        menuBar = new QMenuBar(H2SMonitorServerClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        H2SMonitorServerClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(H2SMonitorServerClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        H2SMonitorServerClass->addToolBar(mainToolBar);
        centralWidget = new QWidget(H2SMonitorServerClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        H2SMonitorServerClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(H2SMonitorServerClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        H2SMonitorServerClass->setStatusBar(statusBar);

        retranslateUi(H2SMonitorServerClass);

        QMetaObject::connectSlotsByName(H2SMonitorServerClass);
    } // setupUi

    void retranslateUi(QMainWindow *H2SMonitorServerClass)
    {
        H2SMonitorServerClass->setWindowTitle(QApplication::translate("H2SMonitorServerClass", "H2SMonitorServer", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class H2SMonitorServerClass: public Ui_H2SMonitorServerClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_H2SMONITORSERVER_H
