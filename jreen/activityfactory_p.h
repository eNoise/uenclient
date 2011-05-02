/****************************************************************************
 *  activityfactory_p.h
 *
 *  Copyright (c) 2010 by Sidorov Aleksey <sauron@citadelspb.com>
 *  Copyright (c) 2011 by Prokhin Alexey <alexey.prokhin@yandex.ru>
 *
 ***************************************************************************
 *                                                                         *
 *   This library is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************
*****************************************************************************/

#ifndef ACTIVITYFACTORY_P_H
#define ACTIVITYFACTORY_P_H

#include "stanzaextension.h"
#include "activity.h"

namespace Jreen {

class ActivityFactory : public StanzaExtensionFactory<Activity>
{
public:
	ActivityFactory();
	virtual ~ActivityFactory();
	QStringList features() const;
	bool canParse(const QStringRef &name, const QStringRef &uri, const QXmlStreamAttributes &attributes);
	void handleStartElement(const QStringRef &name, const QStringRef &uri, const QXmlStreamAttributes &attributes);
	void handleEndElement(const QStringRef &name, const QStringRef &uri);
	void handleCharacterData(const QStringRef &text);
	void serialize(StanzaExtension *extension, QXmlStreamWriter *writer);
	StanzaExtension::Ptr createExtension();
	static QLatin1String generalName(Activity::General general);
	static Activity::General generalByName(const QStringRef &general);
	static QLatin1String specificName(Activity::Specific specific);
	static Activity::Specific specificByName(const QStringRef &specific);
private:
	void clear();
	enum State { AtNowhere, AtText, AtType};
	int m_depth;
	State m_state;
	Activity::General m_general;
	Activity::Specific m_specific;
	QString m_text; //may be need to replace by langmap?
};

} // namespace Jreen

#endif // ACTIVITYFACTORY_P_H