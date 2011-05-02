/****************************************************************************
 *
 *  This file is part of qutIM
 *
 *  Copyright (c) 2011 by Nigmatullin Ruslan <euroelessar@gmail.com>
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

#include "pubsubmanager_p.h"
#include "pubsubeventfactory_p.h"
#include "pubsubpublishfactory_p.h"
#include "client_p.h"

namespace Jreen
{
namespace PubSub
{
class PublishOptionsPrivate : public QSharedData
{
public:
	PublishOptionsPrivate() : accessModel(PublishOptions::PresenceAccess), persistent(true) {}
	PublishOptionsPrivate(PublishOptionsPrivate &o)
		: QSharedData(o), accessModel(o.accessModel), persistent(o.persistent) {}
	PublishOptions::AccessModel accessModel;
	bool persistent;
};

PublishOptions::PublishOptions() : d_ptr(new PublishOptionsPrivate)
{
	d_ptr->accessModel = PresenceAccess;
	d_ptr->persistent = true;
}

PublishOptions::PublishOptions(const PublishOptions &o) : d_ptr(o.d_ptr)
{
}

PublishOptions &PublishOptions::operator =(const PublishOptions &o)
{
	d_ptr = o.d_ptr;
	return *this;
}

PublishOptions::~PublishOptions()
{
}

PublishOptions::AccessModel PublishOptions::accessModel() const
{
	return d_ptr->accessModel;
}

void PublishOptions::setAccessModel(AccessModel model)
{
	d_ptr->accessModel = model;
}

bool PublishOptions::isPersistent() const
{
	return d_ptr->persistent;
}

void PublishOptions::setPersistent(bool persistent)
{
	d_ptr->persistent = persistent;
}

Manager::Manager(Client *client) : QObject(client), d_ptr(new ManagerPrivate)
{
	Q_D(Manager);
	d->client = client;
	d->client->registerStanzaExtension(new EventFactory(d->factories));
	d->client->registerStanzaExtension(new PublishFactory(d->factories));
	connect(d->client, SIGNAL(newMessage(Jreen::Message)),
			this, SLOT(handleMessage(Jreen::Message)));
}

Manager::~Manager()
{
}

static const char *access_strs[] = {
	"authorize",
	"open",
	"presence",
	"roster",
	"whitelist"
};

void Manager::publishItems(const QList<StanzaExtension::Ptr> &items, const JID &to)
{
	IQ iq(IQ::Set, to);
	iq.addExtension(new Publish(items, DataForm::Ptr()));
	d_func()->client->send(iq);
}

void Manager::publishItems(const QList<StanzaExtension::Ptr> &items, const JID &to,
						   const PublishOptions &options)
{
	IQ iq(IQ::Set, to);
	DataForm::Ptr form(new DataForm(DataForm::Submit));
	form->setTypeName(QLatin1String("http://jabber.org/protocol/pubsub#publish-options"));
	form->appendField(DataFormFieldNone(QLatin1String("pubsub#access_model"),
										QStringList(enumToStr(options.accessModel(), access_strs))));
	form->appendField(DataFormFieldBoolean(QLatin1String("pubsub#persist_items"), true));
	iq.addExtension(new Publish(items, form));
	d_func()->client->send(iq);
}

void Manager::addEntityType(int type)
{
	Q_D(Manager);
	ClientPrivate *c = ClientPrivate::get(d->client);
	AbstractStanzaExtensionFactory *factory = c->factories.value(type);
	Q_ASSERT(factory);
	d->factories.append(factory);
	QString node = factory->features().value(0);
	Q_ASSERT(!node.isEmpty());
	d->client->disco()->addFeature(node);
	d->client->disco()->addFeature(node + QLatin1String("+notify"));
	qDebug("%s %d %s", Q_FUNC_INFO, d->factories.size(), qPrintable(node));
}

void Manager::handleMessage(const Jreen::Message &message)
{
	if (Event::Ptr event = message.findExtension<Event>())
		emit eventReceived(event, message.from());
}
}
}
