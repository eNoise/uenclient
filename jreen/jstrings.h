/****************************************************************************
 *  jstrings.h
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

#ifndef JSTRINGS_H
#define JSTRINGS_H

#include "jreen.h"

namespace Jreen {
//enums
template<typename T, int N>
Q_INLINE_TEMPLATE int strToEnum(const T &str, const char *(&strings)[N])
{
	for(int i=0; i < N; i++) {
		if(QLatin1String(strings[i]) == str)
			return i;
	}
	return -1;
}

template<typename X,typename T, int N>
Q_INLINE_TEMPLATE X strToEnum(const T &str, const char *(&strings)[N])
{
	return static_cast<X>(strToEnum(str,strings));
}

template<int N>
Q_INLINE_TEMPLATE QLatin1String enumToStr(int i, const char *(&strings)[N])
{
	return QLatin1String((i < 0 || i >= N) ? 0 : strings[i]);
}

//flags
template<typename T, int N>
Q_INLINE_TEMPLATE int strToFlag(const T &str, const char *(&strings)[N])
{
	int flag = 0x1;
	for(int i=0;i<=N;i++) {
		if(QLatin1String(strings[i]) == str)
			return i;
		flag <<= 1;
	}
	return -1;
}

template<typename T, int N, typename X>
Q_INLINE_TEMPLATE int strToFlag(const T &str, const char *(&strings)[N])
{
	return static_cast<X>(strToFlag(str,strings));
}

template<int N>
Q_INLINE_TEMPLATE QString flagToStr(int i, const char *(&strings)[N])
{
	int n=1;
	while(n < i)
		n <<= 1;
	if(n<0 || n>=N)
		return QString();
	return QLatin1String(strings[n]);
}


}

#endif // JSTRINGS_H
