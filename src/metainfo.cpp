/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Nokia Corporation and its Subsidiary(-ies) nor
**     the names of its contributors may be used to endorse or promote
**     products derived from this software without specific prior written
**     permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
** $QT_END_LICENSE$
**
****************************************************************************/

#include "bencodeparser.h"
#include "metainfo.h"

#include <QDateTime>
#include <QMetaType>
#include <QString>
#include <QFile>
#include <QDir>
#include <QCryptographicHash>

MetaInfo::MetaInfo()
{
    clear();
}

void MetaInfo::clear()
{
    errString = "Unknown error";
    content.clear();
    infoData.clear();
    metaInfoMultiFiles.clear();
    metaInfoAnnounce.clear();
    metaInfoAnnounceList.clear();
    metaInfoCreationDate = QDateTime();
    metaInfoComment.clear();
    metaInfoCreatedBy.clear();
    metaInfoName.clear();
    metaInfoPieceLength = 0;
    metaInfoSha1Sums.clear();
}

bool MetaInfo::parse(const QByteArray &data)
{
    clear();
    content = data;

    BencodeParser parser;
    if (!parser.parse(content)) {
        errString = parser.errorString();
        return false;
    }

    infoData = parser.infoSection();

    QMap<QByteArray, QVariant> dict = parser.dictionary();
    if (!dict.contains("info"))
        return false;

    QMap<QByteArray, QVariant> info = qVariantValue<Dictionary>(dict.value("info"));

    if (info.contains("files")) {
        metaInfoFileForm = MultiFileForm;

        QList<QVariant> files = info.value("files").toList();

        for (int i = 0; i < files.size(); ++i) {
            QMap<QByteArray, QVariant> file = qVariantValue<Dictionary>(files.at(i));
            QList<QVariant> pathElements = file.value("path").toList();
            QByteArray path;
            foreach (QVariant p, pathElements) {
                if (!path.isEmpty())
                    path += "/";
                path += p.toByteArray();
            }

            MetaInfoMultiFile multiFile;
            multiFile.length = file.value("length").toLongLong();
            multiFile.path = QString::fromUtf8(path);
            multiFile.md5sum = file.value("md5sum").toByteArray();
            metaInfoMultiFiles << multiFile;
        }

        metaInfoName = QString::fromUtf8(info.value("name").toByteArray());
        metaInfoPieceLength = info.value("piece length").toInt();
        QByteArray pieces = info.value("pieces").toByteArray();
        for (int i = 0; i < pieces.size(); i += 20)
            metaInfoSha1Sums << pieces.mid(i, 20);
    } else if (info.contains("length")) {
        metaInfoFileForm = SingleFileForm;
        metaInfoSingleFile.length = info.value("length").toLongLong();
        metaInfoSingleFile.md5sum = info.value("md5sum").toByteArray();
        metaInfoSingleFile.name = QString::fromUtf8(info.value("name").toByteArray());
        metaInfoSingleFile.pieceLength = info.value("piece length").toInt();

        QByteArray pieces = info.value("pieces").toByteArray();
        for (int i = 0; i < pieces.size(); i += 20)
            metaInfoSingleFile.sha1Sums << pieces.mid(i, 20);
    }

    metaInfoAnnounce = QString::fromUtf8(dict.value("announce").toByteArray());

    if (dict.contains("announce-list")) {
        // ### unimplemented
    }

    if (dict.contains("creation date"))
        metaInfoCreationDate.setTime_t(dict.value("creation date").toInt());
    if (dict.contains("comment"))
        metaInfoComment = QString::fromUtf8(dict.value("comment").toByteArray());
    if (dict.contains("created by"))
        metaInfoCreatedBy = QString::fromUtf8(dict.value("created by").toByteArray());

    return true;
}

bool MetaInfo::hashDirectory(const QString& path, int metaLenght)
{
	QDir fdir(path);
	if(!fdir.isReadable())
		return false;
	
	foreach(QString curFilePath, fdir.entryList())
	{
		QFile curFile(curFilePath);
		curFile.open(QIODevice::ReadOnly);
		if(!curFile.isOpen() || !curFile.isReadable())
		{
			curFile.close();
			return false;
		}
		
		MetaInfoMultiFile curMetaInfo;
		curMetaInfo.length = curFile.size();
		curMetaInfo.path = curFilePath.remove(fdir.path());
		
		while(!curFile.atEnd())
			metaInfoSha1Sums << QCryptographicHash::hash(curFile.read(metaLenght), QCryptographicHash::Sha1);
	
		metaInfoMultiFiles << curMetaInfo;
		
		curFile.close();
	}
	
	return true;
}

bool MetaInfo::hashFile(const QString& file, int metaLenght)
{
	QFile fopen(file);
	fopen.open(QIODevice::ReadOnly);
	if(!fopen.isOpen() || !fopen.isReadable())
	{
		fopen.close();
		return false;
	}
	
	metaInfoFileForm = SingleFileForm;
	MetaInfoSingleFile hashFile;
	
	hashFile.pieceLength = metaLenght;
	hashFile.length = fopen.size();
	hashFile.name = fopen.fileName();
	
	while(!fopen.atEnd())
		hashFile.sha1Sums << QCryptographicHash::hash(fopen.read(metaLenght), QCryptographicHash::Sha1);
	
	metaInfoSingleFile = hashFile;
	
	fopen.close();
	return true;
}

QByteArray MetaInfo::infoValue() const
{
    return infoData;
}

QString MetaInfo::errorString() const
{
    return errString;
}

MetaInfo::FileForm MetaInfo::fileForm() const
{
    return metaInfoFileForm;
}

QString MetaInfo::announceUrl() const
{
    return metaInfoAnnounce;
}

QStringList MetaInfo::announceList() const
{
    return metaInfoAnnounceList;
}

QDateTime MetaInfo::creationDate() const
{
    return metaInfoCreationDate;
}

QString MetaInfo::comment() const
{
    return metaInfoComment;
}

QString MetaInfo::createdBy() const
{
    return metaInfoCreatedBy;
}

MetaInfoSingleFile MetaInfo::singleFile() const
{
    return metaInfoSingleFile;
}

QList<MetaInfoMultiFile> MetaInfo::multiFiles() const
{
    return metaInfoMultiFiles;
}

QString MetaInfo::name() const
{
    return metaInfoName;
}

int MetaInfo::pieceLength() const
{
    return metaInfoPieceLength;
}

QList<QByteArray> MetaInfo::sha1Sums() const
{
    return metaInfoSha1Sums;
}

qint64 MetaInfo::totalSize() const
{
    if (fileForm() == SingleFileForm)
        return singleFile().length;

    qint64 size = 0;
    foreach (MetaInfoMultiFile file, multiFiles())
        size += file.length;
    return size;
}
