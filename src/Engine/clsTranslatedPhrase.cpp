/*************************************************************************
 * HLMT - A Human Like Machine Translation framework
 * Copyright (C) 2013  S.M.Mohammadzadeh <mehran.m@aut.ac.ir>
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
 @author S.Mohammad M. Ziabary <mehran.m@aut.ac.ir>
 */

#include "clsTranslatedPhrase.h"

clsTranslatedPhrase::clsTranslatedPhrase()
{
    this->IsMultiWord = false;
}

QStringList clsTranslatedPhrase::suggestedWords() const
{
    QStringList OutputWords;
    for(const QStringList& WordList : this->SuggestedPhrase)
        OutputWords += WordList.first();
    return OutputWords;
}

QStringList clsTranslatedPhrase::wholeSuggestedWords() const
{
    QStringList OutputWords;
    for(const QStringList& WordList : this->SuggestedPhrase)
        OutputWords += WordList;
    for(const QStringList& WordList : this->ReservedPhrase)
        OutputWords += WordList;
    return OutputWords;
}

void clsTranslatedPhrase::clear()
{
    this->SuggestedPhrase.clear();
    this->ReservedPhrase.clear();
    this->IsMultiWord = false;
}

QString clsTranslatedPhrase::toStr(bool _simplify) const
{
    QString OutputPhrase;
    for(const QStringList& WordList : this->SuggestedPhrase)
        if (WordList.size() == 1 || _simplify)
            OutputPhrase += WordList.at(qrand() % WordList.size()) + " ";
        else
        {
            OutputPhrase += "{";
            for(const QString Word : WordList)
                OutputPhrase += Word + "|";
            OutputPhrase += "} ";
        }

    if (this->ReservedPhrase.size())
        OutputPhrase += "~~ ";

    for(const QStringList& WordList : this->ReservedPhrase)
        if (WordList.size() == 1 || _simplify)
            OutputPhrase += WordList.at(qrand() % WordList.size()) + " ";
        else
        {
            OutputPhrase += "{";
            for(const QString Word : WordList)
                OutputPhrase += Word + "|";
            OutputPhrase += "} ";
        }
    return OutputPhrase;
}
