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


#include "loginform.h"
#include "uenclient.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>


LoginForm::LoginForm(QWidget* parent) : QDialog(parent)
{
	login = new QLineEdit;
	password = new QLineEdit;
	password->setEchoMode(QLineEdit::Password);
	nick = new QLineEdit;
	startLogin = new QPushButton(tr("Login"));
	
	QVBoxLayout* main = new QVBoxLayout;
	QHBoxLayout* loginLine = new QHBoxLayout;
	QHBoxLayout* passwordLine = new QHBoxLayout;
	QHBoxLayout* nickLine = new QHBoxLayout;
	
	loginLine->addWidget(new QLabel(tr("Login:")));
	loginLine->addWidget(login);
	passwordLine->addWidget(new QLabel(tr("Password:")));
	passwordLine->addWidget(password);
	nickLine->addWidget(new QLabel(tr("Your nickname:")));
	nickLine->addWidget(nick);
	
	main->addLayout(loginLine);
	main->addLayout(passwordLine);
	main->addLayout(nickLine);
	main->addWidget(startLogin);
	
	setLayout(main);
	setModal(true);
	setWindowTitle(tr("Enter your forum login"));
	connect(this, SIGNAL(rejected()), parent, SLOT(close()));
	connect(startLogin, SIGNAL(pressed()), this, SLOT(doLogin()));
}

LoginForm::~LoginForm()
{

}

void LoginForm::doLogin()
{
	QString l = login->text();
	QString p = password->text();
	QString n = nick->text();
	if(l.size() == 0 || p.size() == 0 || n.size() == 0)
		return; // Пустые поля
	((uenclient*)parent())->setJabberJID(l + "@jabber.uruchie.org");
	((uenclient*)parent())->setJabberPassword(p);
	((uenclient*)parent())->setJabberNick(n);
	((uenclient*)parent())->startSession();
	close();
}

#include "loginform.moc"

