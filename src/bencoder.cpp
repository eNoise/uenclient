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


#include "bencoder.h"

Bencoder::Bencoder()
{

}

Bencoder::~Bencoder()
{

}

QByteArray Bencoder::encodeTorrent(const MetaInfo& torrent)
{
	Dictionary reciver;
	reciver["info"] = Dictionary();
	if(torrent.metaInfoFileForm == MetaInfo::MultiFileForm)
	{
		(qVariantValue<Dictionary>(reciver["info"]))["files"] = List();
		(qVariantValue<Dictionary>(reciver["info"]))["piece length"] = torrent.metaInfoPieceLength;
		foreach(MetaInfoMultiFile file, torrent.metaInfoMultiFiles)
		{
			Dictionary fileadd;
			fileadd["length"] = file.length;
			if(file.md5sum.size() > 0)
				fileadd["md5sum"] = file.md5sum;
			QString path;
			if(file.path.startsWith("/"))
				path = file.path.mid(1);
			else
				path = file.path;
			fileadd["path"] = path.split("/");
			qVariantValue<List>((qVariantValue<Dictionary>(reciver["info"]))["files"]) << fileadd; 
		}
		foreach(QByteArray hash, torrent.metaInfoSha1Sums)
			qVariantValue<QByteArray>((qVariantValue<Dictionary>(reciver["info"]))["pieces"]) += hash;
	}
	else
	{
		(qVariantValue<Dictionary>(reciver["info"]))["piece length"] = torrent.metaInfoSingleFile.pieceLength;
		(qVariantValue<Dictionary>(reciver["info"]))["length"] = torrent.metaInfoSingleFile.length;
		if(torrent.metaInfoSingleFile.md5sum.size() > 0)
			(qVariantValue<Dictionary>(reciver["info"]))["md5sum"] = torrent.metaInfoSingleFile.md5sum;
		(qVariantValue<Dictionary>(reciver["info"]))["name"] = torrent.metaInfoSingleFile.name;
		foreach(QByteArray hash, torrent.metaInfoSingleFile.sha1Sums)
			qVariantValue<QByteArray>((qVariantValue<Dictionary>(reciver["info"]))["pieces"]) += hash;
	}
	
	reciver["announce"] = torrent.metaInfoAnnounce;
	if(!torrent.metaInfoCreationDate.isNull())
		reciver["creation date"] = torrent.metaInfoCreationDate.toTime_t();
	if(torrent.metaInfoComment.size() > 0)
		reciver["comment"] = torrent.metaInfoComment;
	if(torrent.metaInfoCreatedBy.size() > 0)
		reciver["created by"] = torrent.metaInfoCreatedBy;
	
	reciver["encoding"] = QString("UTF-8");
	
	return encode(reciver);
}

QByteArray Bencoder::encode(const QVariant& mixed)
{
	if(mixed.type() == QVariant::Int || mixed.type() == QVariant::UInt)
		return encodeInt(mixed.toInt());
	else if(mixed.type() == QVariant::Double)
		return encodeDouble(mixed.toDouble());
	else if(mixed.type() == QVariant::String)
		return encodeString(mixed.toString());
	else if(mixed.type() == QVariant::List)
		return encodeList(mixed.toList());
	else if(mixed.type() == QVariant::Map)
		return encodeDictionary(mixed.toMap());
	return "";
}


QByteArray Bencoder::encodeDictionary(const Dictionary& array)
{
	QMapIterator<QString,QVariant> it(array);
	QByteArray ret = "d";
	while(it.hasNext())
	{
		it.next();
		ret += encodeString(it.key());
		ret += encode(it.value());
	}
	ret += "e";
}

QByteArray Bencoder::encodeDouble(double value)
{
	return (QString("i") + QString().sprintf("%.0f", value) + "e").toUtf8();
}

QByteArray Bencoder::encodeInt(int value)
{
	return (QString("i") + value + "e").toUtf8();
}

QByteArray Bencoder::encodeList(const List& array)
{
	QByteArray ret = "l";
	foreach(QVariant val, array)
		ret += encode(val);
	ret += "e";
	return ret;
}

QByteArray Bencoder::encodeString(const QString& str)
{
	return (str.length() + ":" + str).toUtf8();
}

QByteArray Bencoder::encodeString(const QByteArray& str)
{
	return str.length() + ":" + str;
}

