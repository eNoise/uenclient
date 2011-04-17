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


#ifndef PRIVATECHAT_H
#define PRIVATECHAT_H

#include <QWidget>
#include <QLineEdit>
#include <QTextBrowser>
#include <gloox/messagehandler.h>
#include <gloox/messagesession.h>
#include <gloox/client.h>
#include <gloox/jid.h>


class PrivateChat : public QWidget, public gloox::MessageHandler, public gloox::MessageSession
{
Q_OBJECT
public:
    PrivateChat(gloox::Client* c, gloox::JID jid);
    virtual ~PrivateChat();
private:
    QLineEdit* inputLine;
    QTextBrowser* chatBox;
    void handleMessage( const gloox::Message& msg, gloox::MessageSession* session);
signals:
    void reciveMessage(QString msg, const QString& from);
public slots:
    void sendMessage();
    void addToMessageBox(QString msg, const QString& from);
};

#endif // PRIVATECHAT_H
