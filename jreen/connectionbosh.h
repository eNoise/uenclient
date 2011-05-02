/****************************************************************************
 *
 *  This file is part of qutIM
 *
 *  Copyright (c) 2011 by Nigmatullin Ruslan <euroelessar@gmail.com>
 *
 ***************************************************************************
 *                                                                         *
 *   This file is part of free software; you can redistribute it and/or    *
 *   modify it under the terms of the GNU General Public License as        *
 *   published by the Free Software Foundation; either version 2 of the    *
 *   License, or (at your option) any later version.                       *
 *                                                                         *
 ***************************************************************************
 ****************************************************************************/

#ifndef CONNECTIONBOSH_H
#define CONNECTIONBOSH_H

#include "connection.h"
#include "streamprocessor.h"

class QNetworkReply;

namespace Jreen
{
class ConnectionBOSHPrivate;
class JREEN_EXPORT ConnectionBOSH : public Connection, public StreamProcessor
{
	Q_OBJECT
	Q_DECLARE_PRIVATE(ConnectionBOSH)
	Q_INTERFACES(Jreen::StreamProcessor)
public:
	ConnectionBOSH(const QString &host, int port = 5280);
	~ConnectionBOSH();

	bool open();
	void close();
	qint64 bytesAvailable() const;
	SocketState socketState() const;
	SocketError socketError() const;
protected:
	void authorized();
	QString sessionID() const;
	void setJID(const JID &jid);
	void setStreamParser(XmlStreamParser *parser);
	void restartStream();
	qint64 readData(char *data, qint64 maxlen);
	qint64 writeData(const char *data, qint64 len);
protected slots:
	void onRequestFinished(QNetworkReply *);
private:
	QScopedPointer<ConnectionBOSHPrivate> d_ptr;
};
}

#endif // CONNECTIONBOSH_H
