/********************************************************************************
** Form generated from reading UI file 'medialist.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MEDIALIST_H
#define UI_MEDIALIST_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QListWidget>

QT_BEGIN_NAMESPACE

class Ui_MediaListClass
{
public:

    void setupUi(QListWidget *MediaListClass)
    {
        if (MediaListClass->objectName().isEmpty())
            MediaListClass->setObjectName(QString::fromUtf8("MediaListClass"));
        MediaListClass->resize(600, 400);

        retranslateUi(MediaListClass);

        QMetaObject::connectSlotsByName(MediaListClass);
    } // setupUi

    void retranslateUi(QListWidget *MediaListClass)
    {
        MediaListClass->setWindowTitle(QCoreApplication::translate("MediaListClass", "MediaList", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MediaListClass: public Ui_MediaListClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MEDIALIST_H
