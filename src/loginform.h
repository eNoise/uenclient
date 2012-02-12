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


#ifndef LOGINFORM_H
#define LOGINFORM_H

#include <QDialog>
#include <QPushButton>
#include <QLineEdit>
#include <QCheckBox>
#include <QLabel>
#include <QNetworkAccessManager>

class LoginForm : public QDialog
{
Q_OBJECT
public:
	LoginForm(QWidget* parent);
	~LoginForm();
protected:
	void closeEvent(QCloseEvent *event);
private:
	QLineEdit* login;
	QLineEdit* password;
	QLineEdit* nick;
	QLabel* loginStatus;
	QCheckBox* isAutoLogin;
	QCheckBox* isForumLogin;
	QPushButton* startLogin;
	QNetworkAccessManager manager;
public slots:
	int doLogin();
	int exec();
	void requestFinish(QNetworkReply* reply);
signals:
	void closeSignal();
};

#endif // LOGINFORM_H
