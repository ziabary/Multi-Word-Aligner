/*************************************************************************
 * HLMT - A Human Like Machine Translation framework
 * Copyright (C) 2013 - 2014 S.M.Mohammadzadeh <mehran.m@aut.ac.ir>
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

#include "HLMT.h"
#include "Engine.h"
#include "Knowledge.h"

HLMT* HLMT::Instance = NULL;

HLMT::HLMT()
{
}

const clsTranslatedPhrase &HLMT::translate(const QString &_phrase, bool _onlineLearn)
{
    QStringList FLTokens = _phrase.split(" ");
    FLTokens.removeAll("");
    this->OnlineLearn = _onlineLearn;

    this->TranslatedPhrase.clear();
    QStringList SuggestedTranslations;
    QStringList SuggestedNextTokens;
//    QStringList ReverseTranslations;

    for(QStringList::Iterator CurrTokenIter = FLTokens.begin();
        CurrTokenIter != FLTokens.end();
        CurrTokenIter++)
    {
        //If word is not registered keep it as is
        if (Knowledge::instance().getIDByToken(*CurrTokenIter, false)){
            if (TranslatedPhrase.hasReserved())
                this->TranslatedPhrase.reserve(*CurrTokenIter);
            else
                this->TranslatedPhrase.append(*CurrTokenIter);
            continue;
        }

        SuggestedTranslations = Knowledge::instance().lookupDic(*CurrTokenIter);

        //check sequence dic
        QStringList::Iterator TranslationStartIter = FLTokens.end();

        Knowledge::instance().predictNextTokenByDic("",this->OnlineLearn);//start new prediction sequence
        Knowledge::instance().predictNextTokenByDic(TOKEN_START,this->OnlineLearn);//start sequence;

        /*
         * lookup following tokens and chekc wheter sequence translations are stored in dic or not
         * 1- A token is translated to a sequence
         * 2- A sequence is translated to another sequence
         * 3- No valid sequence is found
         */
        for (QStringList::Iterator CCTokenIter = CurrTokenIter;
             CCTokenIter != FLTokens.end();
             CCTokenIter++)
        {
            SuggestedNextTokens =
                    Knowledge::instance().predictNextTokenByDic(*CCTokenIter, this->OnlineLearn);
            if (SuggestedNextTokens.isEmpty())
                break;
            bool SequenceFound = false;
            for( QString SNTIter : SuggestedNextTokens)
                if (SNTIter == QString(TOKEN_COUNT_PATTERN).arg(
                            (quint32)(CCTokenIter - CurrTokenIter) + 1))
                    TranslationStartIter = CCTokenIter; //translation Sequence is found
                else if (CCTokenIter + 1 != FLTokens.end() &&  SNTIter == *(CCTokenIter + 1)){
                    SequenceFound = true;
                    break;
                }
            //Sequence was not found so break loop.
            if (SequenceFound == false)
                break;

        }
        /*
         * A sequence translation (Types 1 or 2) were found
         */
        if (TranslationStartIter != FLTokens.end())
        {
            //Complete translation sequence until the end of sequence
            QStringList SuggestedSequences =
                    this->completeTranslationSequence(CurrTokenIter, TranslationStartIter);
            //if there were more words in translation sequence add them to suggested translation
            if (SuggestedSequences.size())
                SuggestedTranslations += SuggestedSequences;
            //Skip tokens that were translated using sequence dic.
            CurrTokenIter = TranslationStartIter;
        }

        /*
         * At his point we have translated at least one token so add it to output
         * if there are multiple suggestions try to disambiguate
         */
        if (SuggestedTranslations.size() == 1)
            this->add2TranslatedPhrase(*CurrTokenIter, SuggestedTranslations);
        else if (SuggestedTranslations.size() > 1)
        {
            /// Backtranslation in order to remove extra suggested words
            /*for(const QString& TranslatedWord : SuggestedTranslations)
            {
                ReverseTranslations = this->Knowledge.getPredictionsByWord(this->CurrSecondLangDic, TranslatedWord);
                if (ReverseTranslations.size() &&
                    TranslatedWord.split(" ").size() == 1 &&
                    SuggestedTranslations.size() > 1 &&
                    ReverseTranslations.contains(*CurrTokenIter) == false
                    )
                    SuggestedTranslations.removeOne(TranslatedWord);
            }*/

            /// Remove redundant suggestions
            ///TODO what about sequences?
            for(int i=0; i<SuggestedTranslations.size() - 1; i++)
                for(int j=i+1; j<SuggestedTranslations.size(); j++)
                    if (SuggestedTranslations[i] == SuggestedTranslations[j])
                        SuggestedTranslations.removeAt(j);


            if (SuggestedTranslations.size() == 1)
                this->add2TranslatedPhrase(*CurrTokenIter, SuggestedTranslations);
            else if (SuggestedTranslations.size() > 1)
            {
                /// check destination Language Model if it can remove some
                this->disambiguateByTranslationPath(&SuggestedTranslations);

                if (SuggestedTranslations.size() == 1)
                    this->add2TranslatedPhrase(*CurrTokenIter, SuggestedTranslations);
                if (SuggestedTranslations.size() > 1)
                    this->reserveTranslation(*CurrTokenIter, SuggestedTranslations);
                else{
                    wmaDebug<<"TIP-3: No translation remaining seems to be a bug"<<endl;
                }
            }
            else{
                wmaDebug<<"TIP-2: No translation remaining seems to be a bug"<<endl;
            }
        }
        else
            this->TranslatedPhrase.append(*CurrTokenIter);
    }

    return this->TranslatedPhrase;
}

void HLMT::add2TranslatedPhrase(const QString &_currWord, QStringList _suggestions, bool _disambiguate)
{
    if (_disambiguate)
        this->disambiguateOldReservations(_currWord, &_suggestions);
    if (this->IsExpanded)
    {
        this->IsExpanded = false;
        return;
    }
    if (this->TranslatedPhrase.hasReserved())
        this->reserveTranslation(_currWord, _suggestions, false);
    else
        this->TranslatedPhrase.append(_suggestions);
}

void HLMT::reserveTranslation(const QString &_currWord, QStringList _suggestions, bool _disambiguate)
{
    if (_disambiguate)
    {
        this->disambiguateOldReservations(_currWord, &_suggestions);
        if (this->IsExpanded)
        {
            this->IsExpanded = false;
            return;
        }
        if (this->TranslatedPhrase.hasReserved() == false)
        {
            this->disambiguateByTranslationPath(&_suggestions);

            if (_suggestions.size() == 1)
            {
                this->add2TranslatedPhrase(_currWord, _suggestions);
                return;
            }
        }
    }

    this->TranslatedPhrase.reserve(_suggestions);
}

void HLMT::disambiguateByTranslationPath(QStringList *_suggestedWords)
{
    if (TranslatedPhrase.isEmpty() || TranslatedPhrase.hasReserved())
        return;
/*    QStringList PredictedWords =
            Knowledge::instance().predictNextTokenByLM(
                this->TranslatedPhrase.suggestedWords(),this->OnlineLearn);
    int DeleteCount = 0;
    if (PredictedWords.size())
        for (const QString& SuggestedWord : *_suggestedWords)
            if(PredictedWords.contains(SuggestedWord) == false)
                DeleteCount++;
    if (DeleteCount < _suggestedWords->size())
        for (const QString& SuggestedWord : *_suggestedWords)
            if(PredictedWords.contains(SuggestedWord) == false)
                _suggestedWords->removeOne(SuggestedWord);*/
}

void HLMT::disambiguateOldReservations(const QString &, QStringList *_newSuggestions)
{
    this->IsExpanded = false;
    for (int i= this->TranslatedPhrase.countOfReserved() - 1; i>=0 && !this->IsExpanded; i--)
    {
        if (this->disambiguateReservationsFrom(i, _newSuggestions) == false ||
            this->TranslatedPhrase.countReservedWordsAt(i) > 1 )
            return;
    }
    while (this->TranslatedPhrase.countOfReserved() &&
           this->TranslatedPhrase.ReservedPhrase.first().size() == 1)
        this->TranslatedPhrase.append(TranslatedPhrase.ReservedPhrase.takeFirst());
}

bool HLMT::disambiguateReservationsFrom(int _index, QStringList *_newSuggestions)
{
/*    QStringList PhrasePart;
    QStringList PredictedWords;
    QStringList ReservedForDeletion;
    bool KeepReserved = true;
    bool HasAnySolution = false;

    for(const QString& ReservedWord : this->TranslatedPhrase.reservedAt(_index))
    {
        PhrasePart.clear();
        PhrasePart<<ReservedWord;
        for(int i= _index + 1; i< this->TranslatedPhrase.countOfReserved(); i++)
            PhrasePart<<this->TranslatedPhrase.reservedAt(i).first();
        PredictedWords = this->Knowledge.getPredictionsByPhrase(this->CurrSecondLangModel,
                                                                        PhrasePart);

        if (PredictedWords.size())
        {
            KeepReserved = false;
            for(const QString& PredictedWord : PredictedWords)
                if(_newSuggestions->contains(PredictedWord))
                {
                    KeepReserved = true;
                    HasAnySolution = true;
                    break;
                }

            if (KeepReserved == false)
                ReservedForDeletion.append(ReservedWord);
        }
        else if (this->TranslatedPhrase.countReservedWordsAt(_index) > 1)
                this->TranslatedPhrase.ReservedPhrase[_index].removeOne(ReservedWord);
    }

    if (ReservedForDeletion.size() ==
        this->TranslatedPhrase.countReservedWordsAt(_index))
    {
        if (this->disambiguateByReordering(_index, _newSuggestions))
            return true;
    }

    for(const QString& Word2Delete : ReservedForDeletion)
        if (this->TranslatedPhrase.countReservedWordsAt(_index) > 1)
            this->TranslatedPhrase.ReservedPhrase[_index].removeOne(Word2Delete);

    return HasAnySolution;*/
}

bool HLMT::disambiguateByReordering(int _index, const QStringList *_newSuggestions)
{
/*    if(_index < 1)
        return false;

    QStringList PredictedWords;

    if (_index == this->TranslatedPhrase.countOfReserved() - 1)
    {
        for(const QString& Word : *_newSuggestions)
        {
            PredictedWords = this->Knowledge.getPredictionsByWord(this->CurrSecondLangModel, Word);
            for(const QString& PredictedWord : PredictedWords)
                if(this->TranslatedPhrase.reservedAt(_index).contains(PredictedWord))
                {
                    this->TranslatedPhrase.ReservedPhrase.insert(_index, *_newSuggestions);
                    this->IsExpanded = true;
                    return true;
                }
        }
    }
    else
    {
        ETSError("Not implemented Yet");
    }

    return false;*/
}

QStringList HLMT::completeTranslationSequence(QStringList::Iterator &_flStart,
                                              QStringList::Iterator &_flEnd,
                                              const QString &_currTrans)
{
    QStringList SuggestedNextTokens;

    Knowledge::instance().predictNextTokenByDic("",this->OnlineLearn);
    Knowledge::instance().predictNextTokenByDic(TOKEN_START,this->OnlineLearn);

    for (QStringList::Iterator CCWordIter = _flStart; CCWordIter != _flEnd + 1; CCWordIter++)
        SuggestedNextTokens =
                Knowledge::instance().predictNextTokenByDic(*CCWordIter, this->OnlineLearn);

    //QString TranslationID = "#`" + QString::number(_flEnd - _flStart + 1) + "`#";

    SuggestedNextTokens =
            Knowledge::instance().predictNextTokenByDic(
                QString(TOKEN_COUNT_PATTERN).arg((quint32)(_flEnd - _flStart + 1)),
                this->OnlineLearn);

    QStringList CurrTransTokens = _currTrans.split(" ");CurrTransTokens.removeAll("");

    for(const QString& Token : CurrTransTokens)
        SuggestedNextTokens =
                Knowledge::instance().predictNextTokenByDic(Token, this->OnlineLearn);

    QString CurrTrans = _currTrans;
    while(true)
    {
        if (SuggestedNextTokens.size() == 1)
        {
            if (SuggestedNextTokens.first() == TOKEN_END)
                break;
            if (SuggestedNextTokens.first().startsWith(" "))
                return QStringList();/// seems to be buggy check it is caused by starting pattern
            else
            {
                CurrTrans += " " + SuggestedNextTokens.first();
                SuggestedNextTokens =
                        Knowledge::instance().predictNextTokenByDic(
                            SuggestedNextTokens.first(), this->OnlineLearn);
            }
        }
        else if (SuggestedNextTokens.size() > 1)
        {
            //When multiple patterns are stored then all of them must be reported
            QStringList SuggestedTranslationSequences;
            for (const QString& Token : SuggestedNextTokens)
            {
                if (Token == TOKEN_END)
                    SuggestedTranslationSequences.append(CurrTrans);
                else
                    SuggestedTranslationSequences.append(
                                this->completeTranslationSequence(_flStart, _flEnd, CurrTrans + " " + Token)) ;
            }
            return SuggestedTranslationSequences;
        }
        else
            break;
    }
    return QStringList()<<CurrTrans.trimmed();
}


