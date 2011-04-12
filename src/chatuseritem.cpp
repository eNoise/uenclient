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


#include "chatuseritem.h"
#include <QPainter>
#include <QDebug>
#include <QApplication>

ChatUserItem::ChatUserItem()
{

}

ChatUserItem::~ChatUserItem()
{

}

QSize ChatUserItem::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    return 3.1 * QStyledItemDelegate::sizeHint(option, index);
}


void ChatUserItem::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    QStyledItemDelegate::paint(painter, option, index);
    painter->save();
    
    QString nick = qvariant_cast<QString>(index.data(userNick));
    QString status = qvariant_cast<QString>(index.data(userStatus));
    QString color = qvariant_cast<QString>(index.data(userColor));
    QFont nickf = QApplication::font();
    nickf.setBold(true);
    QFont statusf = QApplication::font();
    statusf.setPixelSize(10);
    
    QRect rect = option.rect;
    
    QPen pen;
    
    QIcon avatar("best.png");
    //qDebug() << avatar.actualSize(option.decorationSize);
    painter->drawPixmap(QPoint(2,rect.top()),avatar.pixmap(50,50));
    
    pen.setColor(QColor("#" + color));
    painter->setPen(pen);
    painter->setFont(nickf);
    painter->drawText(QRect(54, rect.top(), rect.width(), 17), nick);
    painter->setFont(statusf);
    pen.setColor(QColor(qRgb(155,155,155)));
    painter->setPen(pen);
    painter->drawText(QRect(54, rect.top()+17, rect.width(), 17), status);
    
    
    painter->restore();
}

//#include "chatuseritem.moc"