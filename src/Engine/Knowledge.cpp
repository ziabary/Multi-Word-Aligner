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

#include "Knowledge.h"
#include <QStringList>
#include "Common.h"
#include "External/CachedDictionary.h"

Knowledge* Knowledge::Instance = NULL;

void Knowledge::init()
{
    this->MaxWordID=0;
    this->getIDByToken(TOKEN_START); //Special Token used for start
    this->getIDByToken(TOKEN_END); //Special Token used for end

    this->SequenceDic = new clsASM(); //TODO Maybe its good to enable configuration
    this->LM = new clsASM();//TODO Maybe its good to enable configuration
    this->WordDic = CachedDictionary::instance();
}

quint32 Knowledge::getIDByToken(const QString &_token, bool _addIfNotExist)
{
    if (_token.isEmpty())
        return 0;
    quint32 ID = this->Token2ID.value(_token);
     if(!ID && _addIfNotExist){
         this->MaxWordID++;
         this->Token2ID.insert(_token, MaxWordID);
         this->ID2token.insert(MaxWordID, _token);
     }
     return MaxWordID;
}

QString Knowledge::getTokenByID(quint32 _id)
{
    return this->ID2token.value(_id);
}

QStringList Knowledge::predictNextTokenByDic(const QString &_token, bool _learn)
{
    return this->predictNextByASM(this->SequenceDic, _token, _learn);
}

void Knowledge::add2SequenceDic(const QStringList &_flWords,
                      const QStringList &_slWords)
{
    this->SequenceDic->executeOnce(NULL); // Start of a new sequence
    this->SequenceDic->executeOnce(this->getIDByToken(TOKEN_START));
    for (auto Token : _flWords)
        this->SequenceDic->executeOnce(this->getIDByToken(Token));

    this->SequenceDic->executeOnce(this->getIDByToken(QString(TOKEN_COUNT_PATTERN).arg(_flWords.size())));

    for (auto Token : _slWords)
        this->SequenceDic->executeOnce(this->getIDByToken(Token));
    this->SequenceDic->executeOnce(this->getIDByToken(TOKEN_START));
}

QStringList Knowledge::predictNextTokenByLM(const QString &_token, bool _learn)
{
    return this->predictNextByASM(this->LM, _token, _learn);
}

void Knowledge::add2LM(const QString _phrase)
{
    QStringList Tokens = _phrase.split(" ");
    Tokens.removeAll("");
    this->SequenceDic->executeOnce(NULL); // Start of a new sequence
    this->SequenceDic->executeOnce(this->getIDByToken(TOKEN_START));
    for (auto Token : Tokens)
        this->SequenceDic->executeOnce(this->getIDByToken(Token));
}

QStringList Knowledge::lookupDic(const QString &_word)
{
    return this->WordDic->lookup(_word);
}

void Knowledge::save(const QString &_baseDir)
{

}

void Knowledge::load(const QString &_baseDir)
{

}

Knowledge::Knowledge()
{}

QStringList Knowledge::predictNextByASM(clsASM *_asm, const QString _token, bool _learn)
{
    QStringList PredictedTokens;
    const std::unordered_set<ColID_t>& Prediction =
            _asm->executeOnce(this->getIDByToken(_token),false);
    for(ColID_t Predicted : Prediction)
        PredictedTokens.append(this->getTokenByID(Predicted));
    return PredictedTokens;
}

