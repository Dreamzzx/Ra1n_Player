/********************************************************************************
** Form generated from reading UI file 'mainwidget.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWIDGET_H
#define UI_MAINWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "ctrlbar.h"
#include "displaywind.h"

QT_BEGIN_NAMESPACE

class Ui_MainWidgetClass
{
public:
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout;
    QWidget *ShowCtrlBarBgWidget;
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *verticalLayout;
    DisplayWind *showWind;
    QVBoxLayout *verticalLayout_4;
    QLabel *label;
    CtrlBar *ctrlBarWind;
    QVBoxLayout *verticalLayout_3;
    QDockWidget *playListDockWidget;
    QWidget *playListContents;
    QDockWidget *titleDockWidget;
    QWidget *titleContents;

    void setupUi(QMainWindow *MainWidgetClass)
    {
        if (MainWidgetClass->objectName().isEmpty())
            MainWidgetClass->setObjectName(QString::fromUtf8("MainWidgetClass"));
        MainWidgetClass->resize(730, 482);
        MainWidgetClass->setStyleSheet(QString::fromUtf8(""));
        centralWidget = new QWidget(MainWidgetClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        horizontalLayout = new QHBoxLayout(centralWidget);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        ShowCtrlBarBgWidget = new QWidget(centralWidget);
        ShowCtrlBarBgWidget->setObjectName(QString::fromUtf8("ShowCtrlBarBgWidget"));
        verticalLayout_2 = new QVBoxLayout(ShowCtrlBarBgWidget);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        showWind = new DisplayWind(ShowCtrlBarBgWidget);
        showWind->setObjectName(QString::fromUtf8("showWind"));
        verticalLayout_4 = new QVBoxLayout(showWind);
        verticalLayout_4->setSpacing(0);
        verticalLayout_4->setContentsMargins(11, 11, 11, 11);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        verticalLayout_4->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(showWind);
        label->setObjectName(QString::fromUtf8("label"));
        label->setStyleSheet(QString::fromUtf8("background-color: rgb(52, 52, 52);"));

        verticalLayout_4->addWidget(label);


        verticalLayout->addWidget(showWind);

        ctrlBarWind = new CtrlBar(ShowCtrlBarBgWidget);
        ctrlBarWind->setObjectName(QString::fromUtf8("ctrlBarWind"));
        verticalLayout_3 = new QVBoxLayout(ctrlBarWind);
        verticalLayout_3->setSpacing(0);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(0, 0, 0, 0);

        verticalLayout->addWidget(ctrlBarWind);

        verticalLayout->setStretch(0, 9);
        verticalLayout->setStretch(1, 1);

        verticalLayout_2->addLayout(verticalLayout);


        horizontalLayout->addWidget(ShowCtrlBarBgWidget);

        MainWidgetClass->setCentralWidget(centralWidget);
        playListDockWidget = new QDockWidget(MainWidgetClass);
        playListDockWidget->setObjectName(QString::fromUtf8("playListDockWidget"));
        playListContents = new QWidget();
        playListContents->setObjectName(QString::fromUtf8("playListContents"));
        playListDockWidget->setWidget(playListContents);
        MainWidgetClass->addDockWidget(Qt::RightDockWidgetArea, playListDockWidget);
        titleDockWidget = new QDockWidget(MainWidgetClass);
        titleDockWidget->setObjectName(QString::fromUtf8("titleDockWidget"));
        titleDockWidget->setMinimumSize(QSize(58, 60));
        titleDockWidget->setMaximumSize(QSize(524287, 60));
        titleContents = new QWidget();
        titleContents->setObjectName(QString::fromUtf8("titleContents"));
        titleContents->setStyleSheet(QString::fromUtf8(""));
        titleDockWidget->setWidget(titleContents);
        MainWidgetClass->addDockWidget(Qt::TopDockWidgetArea, titleDockWidget);

        retranslateUi(MainWidgetClass);

        QMetaObject::connectSlotsByName(MainWidgetClass);
    } // setupUi

    void retranslateUi(QMainWindow *MainWidgetClass)
    {
        MainWidgetClass->setWindowTitle(QCoreApplication::translate("MainWidgetClass", "MainWidget", nullptr));
        label->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class MainWidgetClass: public Ui_MainWidgetClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWIDGET_H
