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
#include "glooxsession.h"

#include <time.h>
#ifndef NDEBUG
#include <QDebug>
#endif
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QKeyEvent>
#include <QPushButton>
#include <QLabel>
#include <QFile>
#include <QCryptographicHash>
#include "helper.h"
#include "chatuseritem.h"
#include "gloox/vcardupdate.h"

#include <algorithm>

ChatDialog::ChatDialog(uenclient* main, QString login, QString password, QString nick): mainWindow(main)
{
	createWindow(login, password, nick);
}

ChatDialog::ChatDialog(QString login, QString password, QString nick)
{
	createWindow(login, password, nick);
}

void ChatDialog::createWindow(QString login, QString password, QString nick)
{
	inChatList = new QListWidget();
	inputLine = new QLineEdit();
	chatBox = new QTextBrowser();
	chatBox->setOpenExternalLinks(true);
	subjectLine = new QLineEdit();

	chatBox->setStyleSheet("QTextBrowser { background: url('share/icons/chat_background.jpg'); background-repeat: no-repeat; background-attachment: fixed; }");
	
	ChatUserItem* userListDelegate = new ChatUserItem();
	inChatList->setItemDelegate(userListDelegate);
	
	chat = new QHBoxLayout();
	QVBoxLayout* main = new QVBoxLayout();
	QHBoxLayout* subjectLayout = new QHBoxLayout();
	
	inChatList->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
	
	subjectLayout->addWidget(new QLabel(tr("Subject: ")));
	subjectLayout->addWidget(subjectLine);
	QPushButton* updateSubject = new QPushButton(tr("Update subject"));
	subjectLayout->addWidget(updateSubject);
	connect(updateSubject, SIGNAL(pressed()), SLOT(setSubject()));
	
	chat->addWidget(chatBox);
	chat->addWidget(inChatList);
	main->addLayout(subjectLayout);
	main->addLayout(chat);
	main->addWidget(inputLine);
	setLayout(main);
	
	inputLineHeight = inputLine->height();
	inputLine->installEventFilter(this);
	
	connect(this, SIGNAL(updateSubject(QString)), SLOT(subjectUpdated(QString)));
	connect(inputLine, SIGNAL(returnPressed()), this, SLOT(sendMessage()));
	connect(inChatList, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(beginPrivate(QListWidgetItem*)));
	connect(inChatList, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(quoteNick(QListWidgetItem*)));
	//connect(inputLine, SIGNAL(), this, SLOT(sendMessage()));
	connect(this, SIGNAL(reciveMessage(QString,QString,QString)), this, SLOT(addToMessageBox(QString,QString,QString)));
	connect(this, SIGNAL(rebuildUserList()), this, SLOT(updateUserList()));
	connect(this, SIGNAL(changeUserState(bool,QString,QString)), SLOT(printUserState(bool,QString,QString)));
	connect(this, SIGNAL(startPrivate(QString,QString,QString)), SLOT(beginPrivate(QString,QString,QString)));
#ifndef NDEBUG
    qDebug() << "[UENDEBUG] " << "Start gloox session"; 
#endif
	client = new gloox::Client(gloox::JID((login + "/UeNClient").toUtf8().data()), password.toUtf8().data());
	client->disco()->setVersion("uenclient", "0.1.0");
	client->disco()->setIdentity("client", "pc", "uenclient");
	client->registerConnectionListener( this );
	client->registerMessageHandler( this );
	client->registerStanzaExtension(new gloox::VCardUpdate());
	vcardManager = new gloox::VCardManager(client);
	client->logInstance().registerLogHandler(gloox::LogLevelDebug, gloox::LogAreaAll, this);
	forumroom = new gloox::MUCRoom(client, gloox::JID(("main@conference.jabber.uruchie.org/" + nick).toUtf8().data()), this, NULL);
#ifndef NDEBUG
    qDebug() << "[UENDEBUG] " << "Gloox configure finished"; 
#endif
	GlooxSession* gloox = new GlooxSession(this);
	gloox->start();
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

void ChatDialog::handleLog (gloox::LogLevel level, gloox::LogArea area, const std::string &message)
{
#ifndef NDEBUG
	qDebug() << "[GLOOX] " << message.c_str();
#endif
}

void ChatDialog::handleMUCSubject(gloox::MUCRoom* thisroom, const std::string& nick, const std::string& subject)
{
	emit updateSubject(QString().fromUtf8(subject.c_str()));
}

void ChatDialog::setSubject() const
{
	QString curThread = subjectLine->text();
	forumroom->setSubject(curThread.toUtf8().data());
}

void ChatDialog::subjectUpdated(const QString& thread) const
{
	subjectLine->setText(thread);
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
		QVariant avatar;
		avatar.setValue(part.avatar);
		item->setData(ChatUserItem::userNick, nick);
		item->setData(ChatUserItem::userStatus, status);
		item->setData(ChatUserItem::userColor, color);
		item->setData(ChatUserItem::userJID, jid);
		item->setData(ChatUserItem::userAvatar, avatar);
		inChatList->insertItem(i, item);
		i++;
	}
}

void ChatDialog::printUserState(bool online, QString jid, QString nick)
{
	QString fin = QString().sprintf("<font color=\"green\">*** [%s] <b>%s</b>(%s) %s</font>", Helper::timeToString(time(NULL), "%H:%M:%S").toUtf8().data(), nick.toUtf8().data(), jid.toUtf8().data(), ((online) ? "зашел" : "вышел"));
	chatBox->append(fin);
}

ChatDialog::~ChatDialog()
{

}

void ChatDialog::handleMessage(const gloox::Message& msg, gloox::MessageSession* session)
{
	if(session)
		emit startPrivate(session, QString().fromUtf8(msg.body().c_str()));
	else if(QString().fromUtf8(msg.body().c_str()) != "")
		emit startPrivate(QString().fromUtf8(msg.from().full().c_str()), QString().fromUtf8(msg.from().username().c_str()), QString().fromUtf8(msg.body().c_str()));
}

void ChatDialog::handleMUCMessage(gloox::MUCRoom* thisroom, const gloox::Message& msg, bool priv)
{
	emit reciveMessage(QString().fromUtf8(msg.body().c_str()), QString().fromUtf8((msg.from().full().c_str())), QString().fromUtf8((msg.from().resource().c_str())));
}

void ChatDialog::onConnect()
{
#ifndef NDEBUG
    qDebug() << "[UENDEBUG] " << "Gloox connected"; 
#endif
	mainWindow->isJabberOn = true;
	mainWindow->updateServicesStatus();
	forumroom->join();
#ifndef NDEBUG
    qDebug() << "[UENDEBUG] " << "Room joined"; 
#endif
}

void ChatDialog::handleMUCParticipantPresence(gloox::MUCRoom* thisroom, const gloox::MUCRoomParticipant participant, const gloox::Presence& presence)
{
	Participant part = participant;
	if(!part.jid)
		part.jid = new gloox::JID(part.nick->full());
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
				emit changeUserState(false, QString().fromUtf8(part.jid->bare().c_str()), part.nickresque);
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
		
		if(presence.tag()) // fix posible chash
		{
			gloox::VCardUpdate avatarHash(presence.tag()->findChild("x", gloox::XMLNS, gloox::XMLNS_X_VCARD_UPDATE));
			part.avatarhash = avatarHash.hash().c_str();
			if(part.avatarhash.length() > 0)
			{
				QFile avatar(QString(CLIENT_DATA_DIR) + "/avatars/" + part.avatarhash);
				avatar.open(QIODevice::ReadOnly);
				if(avatar.size() > 0)
					part.avatar = avatar.readAll();
				else
				{
					vcardManager->fetchVCard(*part.nick, this);
					//XEP-0084 Here
				}
			
				avatar.close();
			}
		}
		
		isChange = true;
		participants.push_back(part);
		emit changeUserState(true, QString().fromUtf8(part.jid->bare().c_str()), part.nickresque);
		tabIterator = participants.begin();
	}
	if(isChange)
		emit rebuildUserList();
}

void ChatDialog::handleVCard(const gloox::JID& jid, const gloox::VCard* vcard)
{
	// saves avatar
	if(vcard->photo().binval.size() > 0)
	{
		for(std::vector<Participant>::iterator it = participants.begin(); it != participants.end(); it++)
		{
			if(it->roomjid == QString().fromUtf8(jid.full().c_str()))
			{
				it->avatar = QByteArray().fromRawData(vcard->photo().binval.data(),vcard->photo().binval.size());
				it->avatarhash = QCryptographicHash::hash(it->avatar, QCryptographicHash::Sha1).toHex();
				QFile avatar(QString(CLIENT_DATA_DIR) + "/avatars/" + it->avatarhash);
				avatar.open(QIODevice::WriteOnly);
				avatar.write(it->avatar);
				//fixes broken for display avatars
				avatar.close();
				avatar.open(QIODevice::ReadOnly);
				it->avatar = avatar.readAll();
				// fix end
				avatar.close();
				emit rebuildUserList();
				break;
			}
		}
	}
}

void ChatDialog::sendMessage()
{
	if(inputLine->text().startsWith("/nick ") && inputLine->text().mid(6).length() > 0)
	{
		forumroom->setNick(inputLine->text().mid(6).toUtf8().data());
		inputLine->clear();
		return;
	}
	gloox::Message m( gloox::Message::Groupchat, gloox::JID("main@conference.jabber.uruchie.org"), inputLine->text().toUtf8().data() );
	client->send( m );
	inputLine->clear();
	tabIterator = participants.begin();
}

void ChatDialog::addToMessageBox(QString msg, const QString& from, const QString& nick)
{
	QString color = "888888";
	bool findJid = false;
	for(std::vector<Participant>::iterator it = participants.begin(); it != participants.end(); it++)
	{
		//qDebug() << it->color.c_str();
		if(it->roomjid == from)
		{
			color = it->color;
			findJid = true;
			break;
		}
	}
	
	QString format;
	if(findJid)
		format = "<font color=\"#%s\">[%s] &lt;%s&gt;</font> %s";
	else
		format = "<font color=\"#%s\">[%s] &lt;%s&gt; %s</font>";
	if(msg.startsWith("/me "))
		format = "<font color=\"#%s\">[%s]* %s %s</font>";
	Helper::chatTextModify(msg);
	//qDebug() << msg;
	
	QString fin = QString().sprintf(format.toUtf8().data(), color.toUtf8().data(), Helper::timeToString(time(NULL), "%H:%M:%S").toUtf8().data(), nick.toUtf8().data(), msg.toUtf8().data());
	chatBox->append(fin);
}


void ChatDialog::quoteNick(QListWidgetItem* item)
{
	inputLine->setText(inputLine->text() + qvariant_cast<QString>(item->data(ChatUserItem::userNick)) + ": "); 
}

void ChatDialog::beginPrivate(QListWidgetItem* item)
{
	beginPrivate(qvariant_cast<QString>(item->data(ChatUserItem::userJID)), qvariant_cast<QString>(item->data(ChatUserItem::userNick)));
}

void ChatDialog::beginPrivate(QString jid, QString nick, QString defaultMsg)
{
	if(!chats.contains(jid))
	{
		PrivateChat* priv;
		if(defaultMsg != "")
			priv = new PrivateChat(this, client, gloox::JID(jid.toUtf8().data()), defaultMsg);
		else
			priv = new PrivateChat(this, client, gloox::JID(jid.toUtf8().data()));
		mainWindow->tabWidget->addTab(priv, nick);
		chats[jid] = priv;
	}
	mainWindow->tabWidget->setCurrentWidget(chats[jid]);
}

void ChatDialog::beginPrivate(gloox::MessageSession* session, QString defaultMsg)
{	
	if(!session)
		return;
	QString jid = QString().fromUtf8(session->target().full().c_str());
	if(!chats.contains(jid))
	{
		PrivateChat* priv;
		if(defaultMsg != "")
			priv = new PrivateChat(this, session, defaultMsg);
		else
			priv = new PrivateChat(this, session);
		mainWindow->tabWidget->addTab(priv, jid);
		chats[jid] = priv;
	}
}

void ChatDialog::onTabClose(int index)
{
	PrivateChat* priv = (PrivateChat*)mainWindow->tabWidget->widget(index);
	bool isFind = false;
	
	QMap<QString, PrivateChat*>::iterator it;
	for(it = chats.begin(); it != chats.end(); it++)
	{
		if(*it == priv)
		{
			isFind = true;
			break;
		}
	}
	if(!isFind)
		return;
	
	mainWindow->tabWidget->removeTab(index);
	chats.erase(it);
	delete priv;
}


#include "chatdialog.moc"