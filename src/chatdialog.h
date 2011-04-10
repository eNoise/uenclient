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


#ifndef CHATDIALOG_H
#define CHATDIALOG_H

#include <QWidget>
#include <QListWidget>
#include <QLineEdit>
#include <QTextBrowser>

#include <gloox/client.h>
#include <gloox/connectionlistener.h>
#include <gloox/messagehandler.h>
#include <gloox/mucroom.h>
#include <gloox/mucroomhandler.h>
#include <gloox/message.h>
#include <gloox/presence.h>

#include <pthread.h>
#include <vector>

#include <boost/concept_check.hpp>

class ChatDialog : public QWidget, public gloox::ConnectionListener, public gloox::MUCRoomHandler, 
		   public gloox::MessageHandler
{
Q_OBJECT
public:
    ChatDialog();
    virtual ~ChatDialog();
    
    struct Participant : public gloox::MUCRoomParticipant
    {
		gloox::JID* nick;
		gloox::MUCRoomAffiliation affiliation;
		gloox::MUCRoomRole role;
		gloox::JID* jid;
		int flags;
		std::string reason;
		gloox::JID* actor;             
		std::string newNick;
		std::string status;
		gloox::JID* alternate;           
 
		QString color;
		QString nickresque;
		QString roomjid;
	   
	   Participant(const gloox::MUCRoomParticipant& pr) :
		actor(pr.actor),
		affiliation(pr.affiliation),
		alternate(pr.alternate),
		flags(pr.flags),
		jid(pr.jid),
		newNick(pr.newNick),
		nick(pr.nick),
		reason(pr.reason),
		role(pr.role),
		status(pr.status)
	   {}
	   
	   void colorGenerate()
	   {
		  char hex[16] = {'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f'};
		  //generate color
		  for(int i=0; i<6; i++)
			color[i] = hex[rand() % 16];
		  //color[6] = '\0';     
	   }
    };
private:
    QListWidget* inChatList;
    QLineEdit* inputLine;
    QTextBrowser* chatBox;
    
    gloox::Client* client;
    gloox::MUCRoom* forumroom;
    pthread_t glooxthread;
    static void *glooxconnect(void *context);
    // stubs
    void onConnect();
    void onDisconnect  (gloox::ConnectionError e){}
    bool onTLSConnect  (const gloox::CertInfo  &info){return true;}

    void handleMessage( const gloox::Message& msg, gloox::MessageSession* session);
    void handleMUCMessage (gloox::MUCRoom *thisroom, const gloox::Message &msg, bool priv );

    // room enter
    void handleMUCParticipantPresence (gloox::MUCRoom *thisroom, const gloox::MUCRoomParticipant  participant, const gloox::Presence &presence);
                            
    bool handleMUCRoomCreation  (gloox::MUCRoom  *thisroom){return true; }
    void handleMUCSubject (gloox::MUCRoom *thisroom, const std::string &nick, const std::string &subject){}
    void handleMUCInviteDecline (gloox::MUCRoom *thisroom, const gloox::JID &invitee, const std::string &reason){}
    void handleMUCError (gloox::MUCRoom *thisroom, gloox::StanzaError error){}
    void handleMUCInfo (gloox::MUCRoom *thisroom, int features, const std::string &name, const gloox::DataForm *infoForm){}
    void handleMUCItems (gloox::MUCRoom *thisroom, const gloox::Disco::ItemList &items){}

    //Participant currentParticipant;
    std::vector<Participant> participants;
protected:
  void keyPressEvent(QKeyEvent *event);
signals:
  void reciveMessage(QString msg, const QString& from, const QString& nick);
  void rebuildUserList();
public slots:
  void sendMessage();
  void addToMessageBox(QString msg, const QString& from, const QString& nick);
  void updateUserList();
};

#endif // CHATDIALOG_H
