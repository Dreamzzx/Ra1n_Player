#pragma once
#include <QDebug>
#include <QString>

class GlobalHelper
{
public :
	GlobalHelper();
	static QString GetQssStr(QString strCssPath);
};

