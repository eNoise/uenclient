/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) <year>  <name of author>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#include "helper.h"

#include <time.h>
#include <sys/time.h>
#include <QRegExp>

QString Helper::timeToString(time_t t, QString format)
{
	struct tm * ptm = localtime(&t);
	char buf[100];
	strftime (buf, 100, format.toAscii().constData(),  ptm);
	QString result(buf);
	//delete ptm;
	return result;
}

void Helper::chatTextModify(QString& text)
{
	text.replace(QRegExp("((http|ftp|magnet):[^(\\s|\\n)]+)"),"<a href=\"\\1\">\\1</a>");
	text.replace(QRegExp(":([a-z0-9_-]+):"),"<img src=\"smiles/kolobok/\\1.gif\" />");
	text.replace("\n","<br />");
	if(text.startsWith("/me "))
		text = text.mid(4);
}


