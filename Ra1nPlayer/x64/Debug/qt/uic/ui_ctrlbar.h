/********************************************************************************
** Form generated from reading UI file 'ctrlbar.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CTRLBAR_H
#define UI_CTRLBAR_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "myslider.h"

QT_BEGIN_NAMESPACE

class Ui_CtrlBarClass
{
public:
    QVBoxLayout *verticalLayout;
    QGridLayout *gridLayout;
    QLabel *played_time;
    MySlider *ProgressBar;
    QLabel *end_time;
    QHBoxLayout *horizontalLayout;
    QPushButton *rewindBtn;
    QPushButton *play_or_pause_Btn;
    QPushButton *fast_forwardBtn;
    QPushButton *endBtn;
    QPushButton *voiceBtn;
    MySlider *volume_progressBar;
    QLabel *label;
    QComboBox *speed_comboBox;
    QComboBox *HWComboBox;
    QSpacerItem *horizontalSpacer;

    void setupUi(QWidget *CtrlBarClass)
    {
        if (CtrlBarClass->objectName().isEmpty())
            CtrlBarClass->setObjectName(QString::fromUtf8("CtrlBarClass"));
        CtrlBarClass->resize(742, 84);
        verticalLayout = new QVBoxLayout(CtrlBarClass);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        gridLayout = new QGridLayout();
        gridLayout->setSpacing(6);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        played_time = new QLabel(CtrlBarClass);
        played_time->setObjectName(QString::fromUtf8("played_time"));

        gridLayout->addWidget(played_time, 0, 0, 1, 1);

        ProgressBar = new MySlider(CtrlBarClass);
        ProgressBar->setObjectName(QString::fromUtf8("ProgressBar"));
        ProgressBar->setOrientation(Qt::Horizontal);

        gridLayout->addWidget(ProgressBar, 0, 1, 1, 1);

        end_time = new QLabel(CtrlBarClass);
        end_time->setObjectName(QString::fromUtf8("end_time"));

        gridLayout->addWidget(end_time, 0, 2, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        rewindBtn = new QPushButton(CtrlBarClass);
        rewindBtn->setObjectName(QString::fromUtf8("rewindBtn"));
        rewindBtn->setMinimumSize(QSize(40, 0));
        rewindBtn->setMaximumSize(QSize(40, 16777215));

        horizontalLayout->addWidget(rewindBtn);

        play_or_pause_Btn = new QPushButton(CtrlBarClass);
        play_or_pause_Btn->setObjectName(QString::fromUtf8("play_or_pause_Btn"));
        play_or_pause_Btn->setMinimumSize(QSize(40, 0));
        play_or_pause_Btn->setMaximumSize(QSize(40, 16777215));

        horizontalLayout->addWidget(play_or_pause_Btn);

        fast_forwardBtn = new QPushButton(CtrlBarClass);
        fast_forwardBtn->setObjectName(QString::fromUtf8("fast_forwardBtn"));
        fast_forwardBtn->setMinimumSize(QSize(40, 0));
        fast_forwardBtn->setMaximumSize(QSize(40, 16777215));

        horizontalLayout->addWidget(fast_forwardBtn);

        endBtn = new QPushButton(CtrlBarClass);
        endBtn->setObjectName(QString::fromUtf8("endBtn"));
        endBtn->setMinimumSize(QSize(40, 0));
        endBtn->setMaximumSize(QSize(40, 16777215));

        horizontalLayout->addWidget(endBtn);

        voiceBtn = new QPushButton(CtrlBarClass);
        voiceBtn->setObjectName(QString::fromUtf8("voiceBtn"));
        voiceBtn->setMinimumSize(QSize(40, 0));
        voiceBtn->setMaximumSize(QSize(40, 16777215));

        horizontalLayout->addWidget(voiceBtn);

        volume_progressBar = new MySlider(CtrlBarClass);
        volume_progressBar->setObjectName(QString::fromUtf8("volume_progressBar"));
        volume_progressBar->setMinimumSize(QSize(100, 0));
        volume_progressBar->setMaximumSize(QSize(100, 16777215));
        volume_progressBar->setOrientation(Qt::Horizontal);

        horizontalLayout->addWidget(volume_progressBar);

        label = new QLabel(CtrlBarClass);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout->addWidget(label);

        speed_comboBox = new QComboBox(CtrlBarClass);
        speed_comboBox->setObjectName(QString::fromUtf8("speed_comboBox"));

        horizontalLayout->addWidget(speed_comboBox);

        HWComboBox = new QComboBox(CtrlBarClass);
        HWComboBox->setObjectName(QString::fromUtf8("HWComboBox"));

        horizontalLayout->addWidget(HWComboBox);

        horizontalSpacer = new QSpacerItem(300, 40, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        gridLayout->addLayout(horizontalLayout, 1, 0, 1, 3);


        verticalLayout->addLayout(gridLayout);


        retranslateUi(CtrlBarClass);

        QMetaObject::connectSlotsByName(CtrlBarClass);
    } // setupUi

    void retranslateUi(QWidget *CtrlBarClass)
    {
        CtrlBarClass->setWindowTitle(QCoreApplication::translate("CtrlBarClass", "CtrlBar", nullptr));
        played_time->setText(QCoreApplication::translate("CtrlBarClass", "00\357\274\23200", nullptr));
        end_time->setText(QCoreApplication::translate("CtrlBarClass", "00\357\274\23200", nullptr));
        rewindBtn->setText(QString());
        play_or_pause_Btn->setText(QString());
        fast_forwardBtn->setText(QString());
        endBtn->setText(QString());
        voiceBtn->setText(QString());
        label->setText(QCoreApplication::translate("CtrlBarClass", "\345\200\215\351\200\237\357\274\232", nullptr));
    } // retranslateUi

};

namespace Ui {
    class CtrlBarClass: public Ui_CtrlBarClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CTRLBAR_H
