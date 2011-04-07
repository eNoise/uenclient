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


#include "chatdialog.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include "helper.h"

ChatDialog::ChatDialog()
{
	inChatList = new QListWidget();
	inputLine = new QLineEdit();
	chatBox = new QTextBrowser();

	QHBoxLayout* main = new QHBoxLayout();
	QVBoxLayout* chat = new QVBoxLayout();
	chat->addWidget(chatBox);
	chat->addWidget(inputLine);
	main->addLayout(chat);
	main->addWidget(inChatList);
	setLayout(main);
	
	connect(inputLine, SIGNAL(returnPressed()), this, SLOT(sendMessage()));
	connect(this, SIGNAL(reciveMessage(QString,QString)), this, SLOT(addToMessageBox(QString,QString)));
	
	client = new gloox::Client(gloox::JID("pichi@jabber.uruchie.org/UeNClient"), "iampichi");
	//client->disco()->setVersion("UeN Client", "0.1.0");
	client->registerConnectionListener( this );
	client->registerMessageHandler( this );
	forumroom = new gloox::MUCRoom(client, gloox::JID("main@conference.jabber.uruchie.org/pichi"), this, NULL);
	
	if(pthread_create(&glooxthread, NULL, &ChatDialog::glooxconnect, (void*)this) > 0)
		return;
}

void* ChatDialog::glooxconnect(void* context) 	
{
	((ChatDialog *)context)->client->connect();
	pthread_exit(context);
	return context;
}

ChatDialog::~ChatDialog()
{

}

void ChatDialog::handleMessage(const gloox::Message& msg, gloox::MessageSession* session)
{

}

void ChatDialog::handleMUCMessage(gloox::MUCRoom* thisroom, const gloox::Message& msg, bool priv)
{
	emit reciveMessage(QString().fromUtf8(msg.body().c_str()), QString().fromUtf8((msg.from().resource().c_str())));
	//chatBox->append();
}

void ChatDialog::onConnect()
{
	forumroom->join();
}

void ChatDialog::handleMUCParticipantPresence(gloox::MUCRoom* thisroom, const gloox::MUCRoomParticipant participant, const gloox::Presence& presence)
{
	
}

void ChatDialog::sendMessage()
{
	gloox::Message m( gloox::Message::Groupchat, gloox::JID("main@conference.jabber.uruchie.org"), inputLine->text().toUtf8().data() );
	client->send( m );
	inputLine->clear();
}

void ChatDialog::addToMessageBox(const QString& msg, const QString& from)
{
	QString fin = QString().sprintf("[%s] <%s> %s", Helper::timeToString(time(NULL), "%d.%m.%y").toUtf8().data(), from.toUtf8().data(), msg.toUtf8().data());
	chatBox->append(fin);
}


#include "chatdialog.moc"