/********************************************************************************
** Form generated from reading UI file 'h2smonitorclient.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_H2SMONITORCLIENT_H
#define UI_H2SMONITORCLIENT_H

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

class Ui_H2SMonitorClientClass
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *H2SMonitorClientClass)
    {
        if (H2SMonitorClientClass->objectName().isEmpty())
            H2SMonitorClientClass->setObjectName(QStringLiteral("H2SMonitorClientClass"));
        H2SMonitorClientClass->resize(600, 400);
        menuBar = new QMenuBar(H2SMonitorClientClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        H2SMonitorClientClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(H2SMonitorClientClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        H2SMonitorClientClass->addToolBar(mainToolBar);
        centralWidget = new QWidget(H2SMonitorClientClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        H2SMonitorClientClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(H2SMonitorClientClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        H2SMonitorClientClass->setStatusBar(statusBar);

        retranslateUi(H2SMonitorClientClass);

        QMetaObject::connectSlotsByName(H2SMonitorClientClass);
    } // setupUi

    void retranslateUi(QMainWindow *H2SMonitorClientClass)
    {
        H2SMonitorClientClass->setWindowTitle(QApplication::translate("H2SMonitorClientClass", "H2SMonitorClient", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class H2SMonitorClientClass: public Ui_H2SMonitorClientClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_H2SMONITORCLIENT_H
