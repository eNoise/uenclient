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

#ifndef DATAFORM_P_H
#define DATAFORM_P_H

#include "dataform.h"
#include <QVariant>

namespace Jreen
{
	class DataFormFieldPrivate : public QSharedData
	{
	public:
		DataFormFieldPrivate() : type(DataFormField::Invalid), required(false) {}
		DataFormFieldPrivate(DataFormFieldPrivate &o)
			: QSharedData(o), type(o.type), var(o.var), desc(o.desc), label(o.label),
			values(o.values), required(false), options(o.options) {}
		DataFormField::Type type;
		QString var;
		QString desc;
		QString label;
		QStringList values;
		bool required;
		QList<QPair<QString,QString> > options;
		
		static DataFormFieldPrivate *get(DataFormField *field) { return field->d_ptr.data(); }
	private:
		DataFormFieldPrivate &operator =(const DataFormFieldPrivate &o);
	};
	
	class DataFormFieldContainerPrivate
	{
	public:
		template <typename T>
		DataFormField field(const T &name) const
		{
			for (int i = 0; i < fields.size(); i++) {
				if (fields.at(i).var() == name)
					return fields.at(i);
			}
			return DataFormField();
		}
		
		QList<DataFormField> fields;
	};
	
	class DataFormPrivate : public DataFormFieldContainerPrivate
	{
	public:
		QString title;
		DataFormReported::Ptr reported;
		QList<DataFormItem::Ptr> items;
		DataForm::Type type;
	};
}

#endif // DATAFORM_P_H
