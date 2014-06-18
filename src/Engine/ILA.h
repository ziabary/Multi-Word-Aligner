/*************************************************************************
 * MWA - A Multi word alignerc
 * Copyright (C) 2012-2014  S.Mohammad M. Ziabary <mehran.m@aut.ac.ir>
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
 @author S.Mohammad M. Ziabary <mehran.m@aut.ac.ir>
 */

#ifndef ILA_H
#define ILA_H
#include <QString>
#include <QStringList>

class ILA
{
private:
    struct stuReservedTranslation
    {
        QStringList::Iterator Iter;
        bool IsCleared;

        stuReservedTranslation(QStringList::Iterator _secondLangIter,
                               bool _IsCleared = false)
        {
            this->Iter = _secondLangIter;
            this->IsCleared = _IsCleared;
        }
    };

    struct stuReservation
    {
        QStringList::Iterator FirstLangIter;
        QString FirstLangTokens;
        bool IsSequence;
        bool AcceptedSequence;
        bool WasAccepted;
        QList<stuReservedTranslation> SecondLangTokens;

        stuReservation(QStringList::Iterator _firstLangIter,
                       const QList<stuReservedTranslation>& _secondLangReserved,
                       bool _accepted,
                       const QString _firstLangTokens = "")
        {
            this->FirstLangIter =_firstLangIter;
            this->FirstLangTokens = _firstLangTokens.isEmpty() ? *_firstLangIter : _firstLangTokens;
            this->SecondLangTokens = _secondLangReserved;
            this->IsSequence = false;
            this->WasAccepted = _accepted;
            this->AcceptedSequence = _firstLangTokens.size() ? true : false;
        }
    };

public:
    static inline ILA& instance(){
        return Instance ? *Instance : *(Instance = new ILA);
    }

    void process(const QString &_flPhrase,
                 const QString& _slPhrase);

private:
    void reserve(QStringList::Iterator &_firstLangToken,
                 const QList<QStringList::Iterator> &_secondLangTokens,
                 bool _isAccepted);
    void reserve(QStringList::Iterator &_flwStart,
                 QStringList::Iterator &_flwEnd,
                 const QList<QStringList::Iterator> &_secondLangTokens, bool _isAccepted);

    void add2SequenceDic(const QString& _firstLangTokens,
                         const QList<stuReservedTranslation> &_secondLangTokens);
    void add2SequenceDic(const QString &_firstLangTokens,
                         const QString &_translation);

    bool areSameTranslations(const QString& _flToken,
                             const QList<stuReservedTranslation>& _suggestedTranslation);

    QStringList getPredictionByKnowledge(const QStringList &_phrasePart,
                                         bool _isFirstRequest = false);
    quint16 merge(quint16 _currIter,
                  quint16 _nextIter,
                  quint16 _firstIdenticalWordIdx);

private:
    ILA();
    static ILA* Instance;
    QList<stuReservation>   Reservations;
};

#endif // ILA_H
