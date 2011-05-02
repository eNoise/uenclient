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

#ifndef XMLENTITY_H
#define XMLENTITY_H

#include "jreen.h"
#include <QSharedPointer>

namespace Jreen
{
	class JREEN_EXPORT XmlEntity
	{
		Q_DISABLE_COPY(XmlEntity)
	public:
		typedef QSharedPointer<XmlEntity> Ptr;
		
		XmlEntity();
		virtual ~XmlEntity();
		
		static int registerXmlEntityType(const char *type);
		
		virtual int entityType() const = 0;
	};
	
	template <typename T>
	Q_INLINE_TEMPLATE T entity_cast(XmlEntity *se)
	{
		if (se && reinterpret_cast<T>(0)->staticXmlEntityType() == se->entityType())
			return static_cast<T>(se);
		return 0;
	}
}

#define J_ENTITY(Class, XPath) \
	public:  \
		typedef QSharedPointer<Class> Ptr; \
		static int staticXmlEntityType() \
		{ \
			static QBasicAtomicInt entityType = Q_BASIC_ATOMIC_INITIALIZER(0); \
			if (!entityType) { \
				entityType = XmlEntity::registerXmlEntityType( #Class ); \
				Class *useFullNameWithNamespaces = reinterpret_cast< ::Class* >(0); \
				Q_UNUSED(useFullNameWithNamespaces); \
			} \
			return entityType; \
		} \
		virtual int entityType() const \
		{ \
			Q_UNUSED(static_cast<const ::Class*>(this)); \
			return staticEntityType(); \
		} \
	private:

#endif // XMLENTITY_H
