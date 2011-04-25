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


#ifndef GLOOXSESSION_H
#define GLOOXSESSION_H

#include <QThread>
#include <gloox/client.h>


class GlooxSession : public QThread
{

protected:
    void run();
    gloox::Client* client;
public:
    GlooxSession(gloox::Client* client);
    virtual ~GlooxSession();
};

#endif // GLOOXSESSION_H
