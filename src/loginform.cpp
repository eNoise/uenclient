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
#include <QSettings>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QScriptValue>
#include <QScriptEngine>
#include <QCryptographicHash>
#include <QAction>


#ifndef NDEBUG
#include <QDebug>
#endif

LoginForm::LoginForm(QWidget* parent) : QDialog(parent)
{
	login = new QLineEdit;
	password = new QLineEdit;
	password->setEchoMode(QLineEdit::Password);
	nick = new QLineEdit;
	loginStatus = new QLabel;
	startLogin = new QPushButton(tr("Login"));
	isAutoLogin = new QCheckBox(tr("Auto-login?"));
	isForumLogin = new QCheckBox(tr("Uruchie account?"));
	
	QVBoxLayout* main = new QVBoxLayout;
	QHBoxLayout* loginLine = new QHBoxLayout;
	QHBoxLayout* passwordLine = new QHBoxLayout;
	QHBoxLayout* nickLine = new QHBoxLayout;
	QHBoxLayout* checkLink = new QHBoxLayout;
	
	loginLine->addWidget(new QLabel(tr("Login:")));
	loginLine->addWidget(login);
	passwordLine->addWidget(new QLabel(tr("Password:")));
	passwordLine->addWidget(password);
	nickLine->addWidget(new QLabel(tr("Your nickname:")));
	nickLine->addWidget(nick);
	checkLink->addWidget(isAutoLogin);
	checkLink->addWidget(isForumLogin);
	
	main->addLayout(loginLine);
	main->addLayout(passwordLine);
	main->addLayout(nickLine);
	main->addLayout(checkLink);
	main->addWidget(loginStatus);
	main->addWidget(startLogin);
	
	setLayout(main);
	setModal(true);
	setWindowTitle(tr("Enter your forum login"));
	connect(startLogin, SIGNAL(pressed()), this, SLOT(doLogin()));
	
	QSettings settings("eNoise", "UeNclient");
	login->setText(settings.value("LastSessionLogin").toString());
	password->setText(settings.value("LastSessionPassword").toString());
	nick->setText(settings.value("LastSessionNick").toString());
	isAutoLogin->setCheckState((settings.value("IsAutoLogin").toBool()) ? Qt::Checked : Qt::Unchecked);
	if(settings.value("IsForumLogin").isNull())
		isForumLogin->setCheckState(Qt::Checked);
	else
		isForumLogin->setCheckState((settings.value("IsForumLogin").toBool()) ? Qt::Checked : Qt::Unchecked);
	
	connect(this, SIGNAL(closeSignal()), (uenclient*)parent, SLOT(closeThroughtTray()));
	connect(&manager, SIGNAL(finished(QNetworkReply*)), SLOT(requestFinish(QNetworkReply*)));
	((uenclient*)parent)->autoLogin->setChecked(isAutoLogin->checkState() == Qt::Checked);
	connect(isAutoLogin, SIGNAL(toggled(bool)), ((uenclient*)parent)->autoLogin, SLOT(setChecked(bool)));
#ifndef NDEBUG
    qDebug() << "[UENDEBUG] " << "Init login form finished"; 
#endif
}

LoginForm::~LoginForm()
{

}

void LoginForm::closeEvent(QCloseEvent* event) 
{
	emit closeSignal();
}

int LoginForm::exec()
{
	if(isAutoLogin->checkState() == Qt::Checked)
		return doLogin();
	else
		return QDialog::exec();
}

void LoginForm::requestFinish(QNetworkReply* reply)
{
	QScriptValue sc;
	QScriptEngine engine;
	QString json = QString(reply->readAll());
#ifndef NDEBUG
	qDebug() << "[UENDEBUG] " << "Returned JSON: " << json; 
#endif
	sc = engine.evaluate("(" + json + ")");
	if(sc.property("authenticated").toBool())
	{
		((uenclient*)parent())->setApiLogin(login->text());
		((uenclient*)parent())->setApiPassword(QCryptographicHash::hash(password->text().toUtf8(), QCryptographicHash::Md5).toHex());
		((uenclient*)parent())->setApiId(sc.property("user").property("userid").toString());
		((uenclient*)parent())->isSearchOn = true;
		if(nick->text().isEmpty())
			((uenclient*)parent())->setJabberNick(sc.property("user").property("username").toString());
		else
			((uenclient*)parent())->setJabberNick(nick->text());
		((uenclient*)parent())->setJabberJID(sc.property("user").property("jabber_user").toString() + "@jabber.uruchie.org");
		((uenclient*)parent())->setJabberPassword(sc.property("user").property("jabber_password").toString());
		// Login manualy
		emit hide();
		((uenclient*)parent())->confirmedLogin();
	}
	else
	{
#ifndef NDEBUG
	qDebug() << "[UENDEBUG] " << "UAPI login failed"; 
#endif
		loginStatus->setText("<font color=\"red\">" + tr("Wrong login or password.") + "</font>");
		startLogin->setEnabled(true);
	}
}

/**
 * Starts when login was wrong
 */
void LoginForm::loginLock()
{
	emit show();
	((uenclient*)parent())->hide();
	loginStatus->setText("<font color=\"red\">" + tr("Wrong login or password.") + "</font>");
	startLogin->setEnabled(true);
}

int LoginForm::doLogin()
{
	startLogin->setEnabled(false);
	QString l = login->text();
	QString p = password->text();
	QString n = nick->text();
	if(l.size() == 0 || p.size() == 0 || n.size() == 0)
		return QDialog::Rejected; // Пустые поля
		
	QSettings settings("eNoise", "UeNclient");
	settings.setValue("LastSessionLogin", l);
	settings.setValue("LastSessionPassword", p);
	settings.setValue("LastSessionNick", n);
	settings.setValue("IsForumLogin", (isForumLogin->checkState() == Qt::Checked));
	settings.sync();
	
	if(isForumLogin->checkState() == Qt::Unchecked)
	{
		((uenclient*)parent())->setJabberJID(l + "@jabber.uruchie.org");
		((uenclient*)parent())->setJabberPassword(p);
		((uenclient*)parent())->setJabberNick(n);
		hide();
		((uenclient*)parent())->confirmedLogin();
		return QDialog::Accepted;
	}
	else
	{
		//login->setReadOnly(true);
		//password->setReadOnly(true);
		//nick->setReadOnly(true);
		QString params = "module=forum&action=login";
		params += "&username=" + l;
		params += "&password=" + QCryptographicHash::hash(p.toUtf8(), QCryptographicHash::Md5).toHex();
		QNetworkRequest request(QUrl("http://uruchie.org/api.php"));
		request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
		manager.post(request,QUrl(params).toEncoded());
	}
}

#include "loginform.moc"

