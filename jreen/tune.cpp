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

#include "tune.h"
#include <QUrl>

namespace Jreen
{
	class TunePrivate
	{
	public:
		QString artist;
		int length;
		int rating;
		QString source;
		QString title;
		QString track;
		QUrl uri;
	};
	
	Tune::Tune() : d_ptr(new TunePrivate)
	{
		Q_D(Tune);
		d->length = -1;
		d->rating = -1;
	}

	Tune::~Tune()
	{
	}

	void Tune::setArtist(const QString &artist)
	{
		d_func()->artist = artist;
	}

	QString Tune::artist() const
	{
		return d_func()->artist;
	}
	
	void Tune::setLength(int length)
	{
		d_func()->length = qMax(length, -1);
	}

	int Tune::length() const
	{
		return d_func()->length;
	}
	
	void Tune::setRating(int rating)
	{
		d_func()->rating = qBound(-1, rating, 10);
	}

	int Tune::rating() const
	{
		return d_func()->rating;
	}
	
	void Tune::setSource(const QString &source)
	{
		d_func()->source = source;
	}

	QString Tune::source() const
	{
		return d_func()->source;
	}
	
	void Tune::setTitle(const QString &title)
	{
		d_func()->title = title;
	}

	QString Tune::title() const
	{
		return d_func()->title;
	}
	
	void Tune::setTrack(const QString &track)
	{
		d_func()->track = track;
	}

	QString Tune::track() const
	{
		return d_func()->track;
	}
	
	void Tune::setUri(const QUrl &uri)
	{
		d_func()->uri = uri;
	}

	QUrl Tune::uri() const
	{
		return d_func()->uri;
	}
}
