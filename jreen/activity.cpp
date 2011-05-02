/****************************************************************************
 *  activity.cpp
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

#include "activity.h"
#include "activityfactory_p.h"

namespace Jreen {

class ActivityPrivate
{
public:
	Activity::General general;
	Activity::Specific specific;
	QString text;
};

Activity::Activity(General general, Specific specific, const QString &text) :
	d_ptr(new ActivityPrivate)
{
	Q_D(Activity);
	d->general = general;
	d->specific = specific;
	d->text = text;
}

Activity::Activity(const QString &general, const QString &specific, const QString &text) :
	d_ptr(new ActivityPrivate)
{
	Q_D(Activity);
	d->general = ActivityFactory::generalByName(QStringRef(&general));
	d->specific = ActivityFactory::specificByName(QStringRef(&specific));
	d->text = text;
}

Activity::~Activity()
{

}

Activity::General Activity::general() const
{
	return d_func()->general;
}

QString Activity::generalName() const
{
	return ActivityFactory::generalName(d_func()->general);
}

QString Activity::generalName(General general)
{
	return ActivityFactory::generalName(general);
}

Activity::Specific Activity::specific() const
{
	return d_func()->specific;
}

QString Activity::specificName()
{
	return ActivityFactory::specificName(d_func()->specific);
}

QString Activity::specificName(Specific specific)
{
	return ActivityFactory::specificName(specific);
}

const QString &Activity::text() const
{
	return d_func()->text;
}

void Activity::setText(const QString &text)
{
	d_func()->text = text;
}

} // namespace Jreen
