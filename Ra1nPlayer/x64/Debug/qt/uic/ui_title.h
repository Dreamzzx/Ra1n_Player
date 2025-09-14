/********************************************************************************
** Form generated from reading UI file 'title.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TITLE_H
#define UI_TITLE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TitleClass
{
public:
    QHBoxLayout *horizontalLayout_3;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *ctrlMenuBtn;
    QSpacerItem *horizontalSpacer_2;
    QLabel *movie_name;
    QSpacerItem *horizontalSpacer;
    QHBoxLayout *horizontalLayout;
    QPushButton *minBtn;
    QPushButton *fullBtn;
    QPushButton *closeBtn;

    void setupUi(QWidget *TitleClass)
    {
        if (TitleClass->objectName().isEmpty())
            TitleClass->setObjectName(QString::fromUtf8("TitleClass"));
        TitleClass->resize(749, 72);
        TitleClass->setStyleSheet(QString::fromUtf8(""));
        horizontalLayout_3 = new QHBoxLayout(TitleClass);
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        ctrlMenuBtn = new QPushButton(TitleClass);
        ctrlMenuBtn->setObjectName(QString::fromUtf8("ctrlMenuBtn"));
        ctrlMenuBtn->setMinimumSize(QSize(150, 50));
        ctrlMenuBtn->setMaximumSize(QSize(150, 50));
        ctrlMenuBtn->setStyleSheet(QString::fromUtf8("font: 14pt \"Microsoft YaHei UI\";"));

        horizontalLayout_2->addWidget(ctrlMenuBtn);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);

        movie_name = new QLabel(TitleClass);
        movie_name->setObjectName(QString::fromUtf8("movie_name"));
        movie_name->setMinimumSize(QSize(100, 0));
        movie_name->setMaximumSize(QSize(800, 16777215));
        movie_name->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";"));
        movie_name->setAlignment(Qt::AlignCenter);

        horizontalLayout_2->addWidget(movie_name);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        minBtn = new QPushButton(TitleClass);
        minBtn->setObjectName(QString::fromUtf8("minBtn"));
        minBtn->setMinimumSize(QSize(50, 50));
        minBtn->setMaximumSize(QSize(50, 50));
        minBtn->setStyleSheet(QString::fromUtf8("QPushButton:hover{\n"
"    color: aqua;\n"
"}\n"
"\n"
"QPushButton:pressed{\n"
"    color: red;\n"
"}\n"
""));

        horizontalLayout->addWidget(minBtn);

        fullBtn = new QPushButton(TitleClass);
        fullBtn->setObjectName(QString::fromUtf8("fullBtn"));
        fullBtn->setMinimumSize(QSize(50, 50));
        fullBtn->setMaximumSize(QSize(50, 50));
        fullBtn->setStyleSheet(QString::fromUtf8("QPushButton:hover{\n"
"    color: aqua;\n"
"}\n"
"\n"
"QPushButton:pressed{\n"
"    color: red;\n"
"}\n"
""));

        horizontalLayout->addWidget(fullBtn);

        closeBtn = new QPushButton(TitleClass);
        closeBtn->setObjectName(QString::fromUtf8("closeBtn"));
        closeBtn->setMinimumSize(QSize(50, 50));
        closeBtn->setMaximumSize(QSize(50, 50));
        closeBtn->setStyleSheet(QString::fromUtf8("QPushButton#closeBtn:hover{\n"
"    color: tomato;\n"
"}\n"
"\n"
"QPushButton#closeBtn:pressed{\n"
"    color:red;\n"
"}"));

        horizontalLayout->addWidget(closeBtn);


        horizontalLayout_2->addLayout(horizontalLayout);


        horizontalLayout_3->addLayout(horizontalLayout_2);


        retranslateUi(TitleClass);

        QMetaObject::connectSlotsByName(TitleClass);
    } // setupUi

    void retranslateUi(QWidget *TitleClass)
    {
        TitleClass->setWindowTitle(QCoreApplication::translate("TitleClass", "Title", nullptr));
        ctrlMenuBtn->setText(QCoreApplication::translate("TitleClass", "Ra1nPlayer", nullptr));
        movie_name->setText(QString());
        minBtn->setText(QString());
        fullBtn->setText(QString());
        closeBtn->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class TitleClass: public Ui_TitleClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TITLE_H
