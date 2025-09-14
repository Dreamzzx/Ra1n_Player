/********************************************************************************
** Form generated from reading UI file 'playlist.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PLAYLIST_H
#define UI_PLAYLIST_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QWidget>
#include "medialist.h"

QT_BEGIN_NAMESPACE

class Ui_PlayListClass
{
public:
    QGridLayout *gridLayout;
    MediaList *play_list;

    void setupUi(QWidget *PlayListClass)
    {
        if (PlayListClass->objectName().isEmpty())
            PlayListClass->setObjectName(QString::fromUtf8("PlayListClass"));
        PlayListClass->resize(106, 274);
        gridLayout = new QGridLayout(PlayListClass);
        gridLayout->setSpacing(0);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        play_list = new MediaList(PlayListClass);
        play_list->setObjectName(QString::fromUtf8("play_list"));
        play_list->setMinimumSize(QSize(20, 0));

        gridLayout->addWidget(play_list, 0, 0, 1, 1);


        retranslateUi(PlayListClass);

        QMetaObject::connectSlotsByName(PlayListClass);
    } // setupUi

    void retranslateUi(QWidget *PlayListClass)
    {
        PlayListClass->setWindowTitle(QCoreApplication::translate("PlayListClass", "PlayList", nullptr));
    } // retranslateUi

};

namespace Ui {
    class PlayListClass: public Ui_PlayListClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PLAYLIST_H
