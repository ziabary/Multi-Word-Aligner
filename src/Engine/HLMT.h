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

#ifndef HLMT_H
#define HLMT_H

#include "clsTranslatedPhrase.h"

class HLMT
{
public:
    static inline HLMT& instance(){
        return Instance ? *Instance : *(Instance = new HLMT);
    }

    const clsTranslatedPhrase& translate(const QString& _phrase, bool _onlineLearn = false);

private:
    HLMT();
    void add2TranslatedPhrase(const QString &_currWord, QStringList _suggestions, bool _disambiguate = true);
    void reserveTranslation(const QString &_currWord, QStringList _suggestions, bool _disambiguate = true);
    void disambiguateByTranslationPath(QStringList* _suggestedWords);
    void disambiguateOldReservations(const QString&, QStringList *_newSuggestions);
    bool disambiguateReservationsFrom(int _index, QStringList *_newSuggestions);
    bool disambiguateByReordering(int _index, const QStringList* _newSuggestions);
    QStringList completeTranslationSequence(QStringList::Iterator& _flStart,
                                            QStringList::Iterator& _flEnd,
                                            const QString& _currTrans = "");

private:
    static HLMT* Instance;
    bool IsExpanded;
    clsTranslatedPhrase     TranslatedPhrase;
    bool OnlineLearn;
};

#endif // HLMT_H
