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

#ifndef CLSTRANSLATEDPHRASE_H
#define CLSTRANSLATEDPHRASE_H

#include <QStringList>

class clsTranslatedPhrase
{
public:
    clsTranslatedPhrase();

    inline void append(const QStringList& _suggestions){
        this->SuggestedPhrase.append(_suggestions);
        if (this->ReservedPhrase.size())
        {
            this->IsMultiWord = true;
            return;
        }
        for(const QString& Suggestion : _suggestions)
            if (Suggestion.split(" ").size() > 1)
            {
                this->IsMultiWord = true;
                break;
            }
    }

    inline void append(const QString& _suggestion){
        this->SuggestedPhrase.append(QStringList()<<_suggestion);
        if (_suggestion.split(" ").size() > 1)
            this->IsMultiWord = true;
    }

    inline void reserve(const QStringList& _suggestions){
        this->ReservedPhrase.append(_suggestions);
        if (this->SuggestedPhrase.size() > 1)
        {
            this->IsMultiWord = true;
            return;
        }
        for(const QString& Suggestion : _suggestions)
            if (Suggestion.split(" ").size() > 1)
            {
                this->IsMultiWord = true;
                break;
            }
    }

    inline void reserve(const QString& _suggestion){
        this->ReservedPhrase.append(QStringList()<<_suggestion);
        if (_suggestion.split(" ").size() > 1)
            this->IsMultiWord = true;
    }

    inline bool isEmpty() const{
        return this->SuggestedPhrase.isEmpty();
    }

    inline qint32 isMultiWord() const {
        return this->IsMultiWord;
    }

    inline qint32 countOfReserved() const {
        return this->ReservedPhrase.size();
    }

    inline const QStringList& reservedAt(int _index) const{
        return this->ReservedPhrase.at(_index);
    }

    inline bool hasReserved() const{
        return this->ReservedPhrase.size();
    }

    inline qint32 countReservedWordsAt(int _index){
        return this->reservedAt(_index).size();
    }

    QStringList suggestedWords() const;

    QStringList wholeSuggestedWords() const;

    inline QList<QStringList> wholeSuggestions() const{
        return this->SuggestedPhrase + this->ReservedPhrase;
    }

    void clear();

    QString toStr(bool _simplify = true) const;

private:
    QList<QStringList> SuggestedPhrase;
    bool IsMultiWord;

public:
    QList<QStringList> ReservedPhrase;
};

#endif // CLSTRANSLATEDPHRASE_H
