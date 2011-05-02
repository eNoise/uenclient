/****************************************************************************
 *  iq.h
 *
 *  Copyright (c) 2009 by Nigmatullin Ruslan <euroelessar@gmail.com>
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

#ifndef IQ_H
#define IQ_H

#include "stanza.h"

namespace Jreen
{

class IQPrivate;
class JREEN_EXPORT IQ : public Stanza
{
	Q_DECLARE_PRIVATE(IQ)
public:
	typedef QSharedPointer<IQ> Ptr;
	/**
	* Describes the different valid IQ types.
	*/
	enum Type
	{
		Get = 0,                    /**< The stanza is a request for information or requirements. */
		Set,                        /**< The stanza provides required data, sets new values, or
									 * replaces existing values. */
		Result,                     /**< The stanza is a response to a successful get or set request. */
		Error,                      /**< An error has occurred regarding processing or delivery of a
									 * previously-sent get or set (see Stanza Errors (Section 9.3)). */
		Invalid                     /**< The stanza is invalid */
	};
	IQ(Type type, const JID& to, const QString& id = QString());
	IQ();
	IQ(IQPrivate &p);
	inline IQ &operator =(const IQ &stanza)
		{ return *static_cast<IQ *>(&Stanza::operator =(stanza)); }
	Type subtype() const;
	void accept() const;
	bool accepted() const;
};

}

#endif // IQ_H
