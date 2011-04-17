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

#include <QDebug>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QKeyEvent>
#include "helper.h"
#include "chatuseritem.h"


#include <algorithm>

ChatDialog::ChatDialog(uenclient* main): mainWindow(main)
{
	createWindow();
}

ChatDialog::ChatDialog()
{
	createWindow();
}

void ChatDialog::createWindow()
{
	inChatList = new QListWidget();
	inputLine = new QLineEdit();
	chatBox = new QTextBrowser();

	chatBox->setStyleSheet("QTextBrowser { background: url('bak.jpg'); }");
	
	ChatUserItem* userListDelegate = new ChatUserItem();
	inChatList->setItemDelegate(userListDelegate);
	
	chat = new QHBoxLayout();
	QVBoxLayout* main = new QVBoxLayout();
	
	inChatList->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
	
	chat->addWidget(chatBox);
	chat->addWidget(inChatList);
	main->addLayout(chat);
	main->addWidget(inputLine);
	setLayout(main);
	
	inputLineHeight = inputLine->height();
	inputLine->installEventFilter(this);
	
	connect(inputLine, SIGNAL(returnPressed()), this, SLOT(sendMessage()));
	connect(inChatList, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(beginPrivate(QListWidgetItem*)));
	//connect(inputLine, SIGNAL(), this, SLOT(sendMessage()));
	connect(this, SIGNAL(reciveMessage(QString,QString,QString)), this, SLOT(addToMessageBox(QString,QString,QString)));
	connect(this, SIGNAL(rebuildUserList()), this, SLOT(updateUserList()));
	connect(this, SIGNAL(changeUserState(bool,QString)), SLOT(printUserState(bool,QString)));
	client = new gloox::Client(gloox::JID("pichi@jabber.uruchie.org/UeNClient"), "iampichi");
	//client->disco()->setVersion("UeN Client", "0.1.0");
	client->registerConnectionListener( this );
	client->registerMessageHandler( this );
	forumroom = new gloox::MUCRoom(client, gloox::JID("main@conference.jabber.uruchie.org/pichi"), this, NULL);
	
	if(pthread_create(&glooxthread, NULL, &ChatDialog::glooxconnect, (void*)this) > 0)
		return;
}

bool ChatDialog::eventFilter(QObject* pObject, QEvent* pEvent)
{
	if (pEvent->type() == QEvent::KeyPress)
	{
		QKeyEvent* pKeyEvent = static_cast<QKeyEvent*>(pEvent);
		if(pKeyEvent->key() == Qt::Key_Space && (pKeyEvent->modifiers() & Qt::ShiftModifier))
		{
			//todo resize
			inputLine->setText(inputLine->text() + "\n");
		}
		else if(pKeyEvent->key() == Qt::Key_Tab)
		{	
			if(tabIterator == participants.end())
				tabIterator = participants.begin();
			
			inputLine->setText(tabIterator->nickresque + ": ");
			
			tabIterator++;
		}
	}
	else
	{
		return QObject::eventFilter(pObject, pEvent);
	}
}


void ChatDialog::updateUserList()
{
	inChatList->clear();
	int i=1;
	foreach(Participant part, participants)
	{
		QListWidgetItem* item = new QListWidgetItem;
		QVariant nick;
		nick.setValue(part.nickresque);
		QVariant status;
		status.setValue(QString().fromUtf8(part.status.c_str()));
		QVariant color;
		color.setValue(part.color);
		QVariant jid;
		jid.setValue(QString().fromUtf8(part.jid->full().c_str()));
		item->setData(ChatUserItem::userNick, nick);
		item->setData(ChatUserItem::userStatus, status);
		item->setData(ChatUserItem::userColor, color);
		item->setData(ChatUserItem::userJID, jid);
		inChatList->insertItem(i, item);
		i++;
	}
}

void ChatDialog::printUserState(bool online, QString nick)
{
	QString fin = QString().sprintf("<font color=\"green\">*** [%s] <b>%s</b> %s</font>", Helper::timeToString(time(NULL), "%H:%M:%S").toUtf8().data(), nick.toUtf8().data(), ((online) ? "зашел" : "вышел"));
	chatBox->append(fin);
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
	//emit beginPrivate(QString().fromUtf8((msg.from().full().c_str())), QString().fromUtf8((msg.from().full().c_str())));
}

void ChatDialog::handleMUCMessage(gloox::MUCRoom* thisroom, const gloox::Message& msg, bool priv)
{
	emit reciveMessage(QString().fromUtf8(msg.body().c_str()), QString().fromUtf8((msg.from().full().c_str())), QString().fromUtf8((msg.from().resource().c_str())));
	//chatBox->append();
}

void ChatDialog::onConnect()
{
	forumroom->join();
}

void ChatDialog::handleMUCParticipantPresence(gloox::MUCRoom* thisroom, const gloox::MUCRoomParticipant participant, const gloox::Presence& presence)
{
	Participant part = participant;
	part.colorGenerate();
	part.nickresque = QString().fromUtf8(part.nick->resource().c_str());
	part.roomjid = QString().fromUtf8(part.nick->full().c_str());
	bool isChange = false;
	if(presence.presence() == gloox::Presence::Unavailable)
	{
		for(std::vector<Participant>::iterator it = participants.begin(); it != participants.end(); it++)
		{
			if(it->roomjid == part.roomjid)
			{
				isChange = true;
				participants.erase(it);
				emit changeUserState(false, part.nickresque);
				break;
			}
		}
	}
	else
	{
		//part.nick = new gloox::JID();
		//memcpy(part->nick, participant.nick, sizeof(gloox::JID));
		//qDebug() << part.nickresque;
		
		for(std::vector<Participant>::iterator it = participants.begin(); it != participants.end(); it++)
		{
			if(it->roomjid == part.roomjid)
				return;
		}
		isChange = true;
		participants.push_back(part);
		emit changeUserState(true, part.nickresque);
		tabIterator = participants.begin();
	}
	if(isChange)
		emit rebuildUserList();
}

void ChatDialog::sendMessage()
{
	gloox::Message m( gloox::Message::Groupchat, gloox::JID("main@conference.jabber.uruchie.org"), inputLine->text().toUtf8().data() );
	client->send( m );
	inputLine->clear();
	tabIterator = participants.begin();
}

void ChatDialog::addToMessageBox(QString msg, const QString& from, const QString& nick)
{
	QString color = "FF0000";
	for(std::vector<Participant>::iterator it = participants.begin(); it != participants.end(); it++)
	{
		//qDebug() << it->color.c_str();
		if(it->roomjid == from)
		{
			color = it->color;
			break;
		}
	}
	
	msg.replace(QRegExp("((http|ftp|magnet):[^(\\s|\\n)]+)"),"<a href=\"\\1\">\\1</a>");
	msg.replace(QRegExp(":([a-z0-9_-]+):"),"<img src=\"smiles/kolobok/\\1.gif\" />");
	msg.replace("\n","<br />");
	
	//qDebug() << msg;
	
	QString fin = QString().sprintf("<font color=\"#%s\">[%s] &lt;%s&gt;</font> %s", color.toUtf8().data(), Helper::timeToString(time(NULL), "%H:%M:%S").toUtf8().data(), nick.toUtf8().data(), msg.toUtf8().data());
	chatBox->append(fin);
}


void ChatDialog::beginPrivate(QListWidgetItem* item)
{
	beginPrivate(qvariant_cast<QString>(item->data(ChatUserItem::userJID)), qvariant_cast<QString>(item->data(ChatUserItem::userNick)));
}

void ChatDialog::beginPrivate(QString jid, QString nick)
{
	if(!chats.contains(jid))
	{
		PrivateChat* priv = new PrivateChat(client, gloox::JID(jid.toUtf8().data()));
		mainWindow->tabWidget->addTab(priv, nick);
		chats[jid] = priv;
	}
	mainWindow->tabWidget->setCurrentWidget(chats[jid]);
}

#include "chatdialog.moc"