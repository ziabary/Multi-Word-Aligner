/*************************************************************************
 * MWA - A Multi word alignerc
 * Copyright (C) 2012-2014  S.M.Mohammadzadeh <mehran.m@aut.ac.ir>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *************************************************************************/
/**
 @author S.M.Mohammadzadeh <mehran.m@aut.ac.ir>
 */

#include "ILA.h"

ILA* ILA::Instance = NULL;

ILA::ILA()
{
}


void ILA::process(const QString &_flPhrase, const QString &_slPhrase)
{
}

void ILA::reserve(QStringList::Iterator &_firstLangWord, const QList<QStringList::Iterator> &_secondLangWords, bool _isAccepted)
{
}

void ILA::reserve(QStringList::Iterator &_flwStart, QStringList::Iterator &_flwEnd, const QList<QStringList::Iterator> &_secondLangWords, bool _isAccepted)
{
}

void ILA::add2SequenceDic(const QString &_firstLangWords, const QList<stuReservedTranslation> &_secondLangWords)
{
}

void ILA::add2SequenceDic(const QString &_firstLangWords, const QString &_translation)
{
}

bool ILA::areSameTranslations(const QString &_flWords, const QList<stuReservedTranslation> &_suggestedTranslation)
{
}

QStringList ILA::getPredictionByKnowledge(const QStringList &_phrasePart, bool _isFirstRequest)
{
}

quint16 ILA::merge(quint16 _currIter, quint16 _nextIter, quint16 _firstIdenticalWordIdx)
{
}

