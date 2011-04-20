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


#include "privatechat.h"
#include <QVBoxLayout>
#include <gloox/message.h>
#include "helper.h"

PrivateChat::PrivateChat(gloox::Client* c, gloox::JID jid) : gloox::MessageSession(c, jid)
{
	QVBoxLayout* main = new QVBoxLayout;
	inputLine = new QLineEdit;
	chatBox = new QTextBrowser;
	main->addWidget(chatBox);
	main->addWidget(inputLine);
	setLayout(main);
	
	connect(this, SIGNAL(reciveMessage(QString,QString)), SLOT(addToMessageBox(QString,QString)));
	connect(inputLine, SIGNAL(returnPressed()), this, SLOT(sendMessage()));
	
	registerMessageHandler(this);
}

PrivateChat::~PrivateChat()
{

}


void PrivateChat::handleMessage(const gloox::Message& msg, gloox::MessageSession* session)
{
	emit reciveMessage(QString().fromUtf8(msg.body().c_str()), QString().fromUtf8((msg.from().full().c_str())));
	//chatBox->append();
}

void PrivateChat::addToMessageBox(QString msg, const QString& from)
{
	QString targ = QString().fromUtf8((target().full().c_str()));
	Helper::chatTextModify(msg);
	QString fin = QString().sprintf("<font color=\"#%s\">[%s] &lt;%s&gt;</font> %s", ((targ != from) ? "ff0000" : "0000ff"), Helper::timeToString(time(NULL), "%H:%M:%S").toUtf8().data(), from.toUtf8().data(), msg.toUtf8().data());
	chatBox->append(fin);
}

void PrivateChat::sendMessage()
{
	gloox::Message m( gloox::Message::Chat, target(), inputLine->text().toUtf8().data() );
	send( m );
	emit reciveMessage(inputLine->text(), "pichi");
	inputLine->clear();
}


#include "privatechat.moc"
