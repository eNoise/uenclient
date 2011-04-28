/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) 2011  <copyright holder> <email>

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


#ifndef BENCODER_H
#define BENCODER_H

#include <QString>
#include <QByteArray>
#include <QVector>
#include <QMap>
#include <QVariant>

#include "metainfo.h"

typedef QMap<QString,QVariant> Dictionary;
Q_DECLARE_METATYPE(Dictionary)
typedef QList<QVariant> List;
Q_DECLARE_METATYPE(List)

class Bencoder
{

public:
    Bencoder();
    virtual ~Bencoder();
    QByteArray encodeTorrent(const MetaInfo& torrent);
private:
    QByteArray encode(const QVariant& mixed);
    QByteArray encodeInt(int value);
    QByteArray encodeDouble(double value);
    QByteArray encodeString(const QString& str);
    QByteArray encodeString(const QByteArray& str);
    QByteArray encodeList(const List& array);
    QByteArray encodeDictionary(const Dictionary& array);
};

#endif // BENCODER_H
