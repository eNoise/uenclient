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


#ifndef CHATUSERITEM_H
#define CHATUSERITEM_H

#include <QStyledItemDelegate>

class ChatUserItem : public QStyledItemDelegate
{
Q_OBJECT
public:
    ChatUserItem();
    virtual ~ChatUserItem();
    
    enum ItemRole
    {
		userNick = Qt::UserRole + 100, 
		userStatus = Qt::UserRole + 101,
		userColor = Qt::UserRole + 102,
		userJID = Qt::UserRole + 103,
		userAvatar = Qt::UserRole + 104
    };
    
    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const;

    QSize sizeHint(const QStyleOptionViewItem &option,
                   const QModelIndex &index) const;

};

#endif // CHATUSERITEM_H
