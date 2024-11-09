#include "globalhelper.h"
#include <QFile>
#include <QPushButton>

GlobalHelper::GlobalHelper()
{
}

QString GlobalHelper::GetQssStr(QString strCssPath)
{
    QString strQss;
    QFile FileCss(strCssPath);
    if (FileCss.open(QIODevice::ReadOnly))
    {
        strQss = FileCss.readAll();
        FileCss.close();
    }
    else
    {
        qDebug() << "read css failed!";
    }
    return strQss;
}
