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


#include "glooxsession.h"
#include "chatdialog.h"
#include "uenclient.h"
#include "loginform.h"

#ifndef NDEBUG
#include <QDebug>
#endif

void GlooxSession::run()
{
#ifndef NDEBUG
    qDebug() << "[UENDEBUG] " << "Gloox thread started"; 
#endif
	client->connect();
#ifndef NDEBUG
    qDebug() << "[UENDEBUG] " << "Gloox authed: " << client->authed(); 
#endif
    // Restarting session
    if(!client->authed()) {
#ifndef NDEBUG
		qDebug() << "[UENDEBUG] " << "Restarting LoginForm"; 
#endif
		emit requestLoginForm();
    }
}

GlooxSession::GlooxSession(gloox::Client* client) : client(client)
{

}

GlooxSession::GlooxSession(ChatDialog* parent): client(parent->client),
						parent(parent)
{
	connect(this, SIGNAL(requestLoginForm()), ((uenclient*)parent->mainWindow)->loginForm, SLOT(loginLock()));
}

GlooxSession::~GlooxSession()
{

}

#include "glooxsession.moc"
