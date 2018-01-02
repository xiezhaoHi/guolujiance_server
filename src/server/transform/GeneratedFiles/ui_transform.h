/********************************************************************************
** Form generated from reading UI file 'transform.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TRANSFORM_H
#define UI_TRANSFORM_H

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

class Ui_transformClass
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *transformClass)
    {
        if (transformClass->objectName().isEmpty())
            transformClass->setObjectName(QStringLiteral("transformClass"));
        transformClass->resize(600, 400);
        menuBar = new QMenuBar(transformClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        transformClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(transformClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        transformClass->addToolBar(mainToolBar);
        centralWidget = new QWidget(transformClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        transformClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(transformClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        transformClass->setStatusBar(statusBar);

        retranslateUi(transformClass);

        QMetaObject::connectSlotsByName(transformClass);
    } // setupUi

    void retranslateUi(QMainWindow *transformClass)
    {
        transformClass->setWindowTitle(QApplication::translate("transformClass", "transform", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class transformClass: public Ui_transformClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TRANSFORM_H
