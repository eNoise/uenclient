/****************************************************************************
 *
 *  This file is part of qutIM
 *
 *  Copyright (c) 2010 by Nigmatullin Ruslan <euroelessar@gmail.com>
 *
 ***************************************************************************
 *                                                                         *
 *   This file is part of free software; you can redistribute it and/or    *
 *   modify it under the terms of the GNU General Public License as        *
 *   published by the Free Software Foundation; either version 2 of the    *
 *   License, or (at your option) any later version.                       *
 *                                                                         *
 ***************************************************************************
 ****************************************************************************/

#ifndef IQFACTORY_P_H
#define IQFACTORY_P_H

#include "stanzafactory.h"
#include "iq.h"

namespace Jreen
{
class IqFactory : public StanzaFactory
{
public:
	IqFactory(Client *client);
	int stanzaType();
	Stanza::Ptr createStanza();
	void serialize(Stanza *stanza, QXmlStreamWriter *writer);
	bool canParse(const QStringRef &name, const QStringRef &uri, const QXmlStreamAttributes &attributes);
	void handleStartElement(const QStringRef &name, const QStringRef &uri, const QXmlStreamAttributes &attributes);
	void handleEndElement(const QStringRef &name, const QStringRef &uri);
	void handleCharacterData(const QStringRef &name);
private:
	int m_depth;
	IQ::Type m_type;
};
}

#endif // IQFACTORY_P_H
