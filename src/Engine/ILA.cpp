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

#include "ILA.h"
#include "Knowledge.h"
#include "Engine.h"
#include "HLMT.h"

ILA* ILA::Instance = NULL;

ILA::ILA()
{
}

void ILA::process(const QString &_flPhrase, const QString &_slPhrase)
{
    this->Reservations.clear();

    /****************************************************/
    //Preparation Step before algorithm
    QStringList FLPhrase = _flPhrase.split(" ");
    FLPhrase.removeAll("");
    QStringList SLPhrase = _slPhrase.split(" ");
    SLPhrase.removeAll("");

    /****************************************************/
    //1- SET CurrFLWord = First Word Of FLPhrase
    //2- SET CurrSLWord = First Word Of SLPhrase
    //3- SET SL2Reserve = CurrSLWord
    //4- SET StartingCheckPoint = CurrSLWord
    //5- SET Accepted = false
    //6- SET Skipped = false

    auto CurrFLWord = FLPhrase.begin();
    auto CurrSLWord = SLPhrase.begin();
    QList<QStringList::Iterator> SL2Reserve = QList<QStringList::Iterator>()<<CurrSLWord;
    auto StartingCheckPoint = CurrSLWord;
    bool Accepted = false;
    bool Skipped = false;

    /****************************************************/
    QStringList MySuggestions;
    QStringList ExDicSuggestions;
    QStringList PredictedWords;

    QString     SLWordStem;
    QStringList PhrasePart;

    auto        NextFLWord = CurrFLWord;
    auto        NextSLWord = CurrSLWord;
    auto        CCSLWord   = CurrSLWord;
    bool        ExDicChecked = false;
    bool        KnowledgeChecked = false;

    wmaDebug<<"******************************************************"<<endl;
    wmaDebug<<wmaPrintable(_flPhrase)<<endl;
    wmaDebug<<wmaPrintable(_slPhrase)<<endl;
    wmaDebug<<"++++++++++++++++++++++++++++++++++++++++++++++++++++++"<<endl;

    do{
        do{
            //40- PhrasePart = CurrFLWord
            PhrasePart = QStringList()<<*CurrFLWord;

            //41- IF there are more words in FLPhrase
            //41.1- NextFLWord = Next Word after CurrFLWord in FLPhrase
            //41.2- PredictedWords = getPredictionByKnowledge(PhrasePart)
            //41.3- If PredictedWords contains NextFLWord
            //41.3.1- PhrasePart = PhrasePart + NextFLWord
            //41.3.2- If there are more words in FLPhrase
            //41.3.2.1- NextFLWord = Next Word after NextFLWord in FLPhrase
            //41.3.2.2- goto 11.2
            //41.3.3- PredictedWords = getPredictionByKnowledge(PhrasePart)
            //41.3.4- IF PredictedWords contains CurrSLWord
            //41.3.4.1- PhrasePart = PhrasePart + CurrSLWord
            //41.3.4.2- NextSLWord = Next Word after CurrSLWord in SLPhrase
            //41.3.4.3- PredictedWords = getPredictionByKnowledge(PhrasePart)
            //41.3.4.4- If PredictedWords contains NextSLWord
            //41.3.4.4.1- PhrasePart = PhrasePart + NextSLWord
            //41.3.4.4.2- If there are more words in SLPhrase
            //41.3.4.4.2.1- NextFLWord = Next Word after NextFLWord in FLPhrase
            //41.3.4.4.2.2- goto 41.3.4.3
            //41.3.4.5- IF Skipped
            //41.3.4.5.1- Reserve CurrFLWord with SL2Reserve
            //41.3.4.6- If SL2Reserve = CurrSLWord
            //41.3.4.6.1- Accepted = true
            //41.3.4.8- goto 100

            if (CurrFLWord + 1 != FLPhrase.end())
            {
                NextFLWord = CurrFLWord + 1;
                bool IsFirstTime = true;
                bool FoundSequence = false;
                do{
                    PredictedWords = this->getPredictionByKnowledge(PhrasePart, IsFirstTime);
                    IsFirstTime = false;
                    if (PredictedWords.contains(*NextFLWord))
                    {
                        FoundSequence = true;
                        PhrasePart += *NextFLWord;
                        if (NextFLWord + 1 != FLPhrase.end())
                        {
                            NextFLWord = NextFLWord + 1;
                            continue;
                        }
                    }
                    break;
                }while (true);

                if (FoundSequence)
                {
                    bool CanBeAccepted = (SL2Reserve.size() == 1);
                    PredictedWords = this->getPredictionByKnowledge(PhrasePart);
                    QString TID = QString(TOKEN_COUNT_PATTERN).arg(PhrasePart.size());
                    if (PredictedWords.contains(TID))
                    {
                        PhrasePart += TID;
                        NextSLWord = CurrSLWord;
                        do{
                            PredictedWords = this->getPredictionByKnowledge(PhrasePart);
                            if (PredictedWords.contains(*NextSLWord))
                            {
                                PhrasePart += *NextSLWord;
                                if (NextSLWord + 1 != SLPhrase.end())
                                {
                                    NextSLWord = NextSLWord + 1;
                                    SL2Reserve += NextSLWord;
                                    continue;
                                }
                            }
                            break;

                        }while(true);

                        PredictedWords = this->getPredictionByKnowledge(PhrasePart);
                        if (PredictedWords.contains(TOKEN_END))
                        {
                            if (Skipped)
                                this->reserve(CurrFLWord, NextFLWord, SL2Reserve, CanBeAccepted);
                            if (CanBeAccepted)
                                Accepted = true;

                            CurrFLWord = NextFLWord;
                            CurrSLWord = NextSLWord;
                            break;// goto 100;
                        }
                    }
                }
            }

            //10- MySuggestions = translateByKnowledge(CurrFLWord)
            if (KnowledgeChecked == false)
            {
                MySuggestions = Knowledge::instance().lookupDic(*CurrFLWord);
                        //HLMT::instance().translate(*CurrFLWord, WMAConfigs.OnlineLearn).wholeSuggestedWords();
                KnowledgeChecked = true;
            }

            //11- If MySuggestions contains CurrSLWord
            //11.1- If Skipped
            //11.1.1- Reserve CurrFLWord with SL2Reserve
            //11.2- If SL2Reserve = CurrSLWord
            //11.2.1- Accepted = true
            //11.3- goto 100
            if (MySuggestions.contains(*CurrSLWord))
            {
                if (Skipped)
                    this->reserve(CurrFLWord, SL2Reserve, SL2Reserve.size() == 1);
                if(SL2Reserve.size() == 1)
                    Accepted = true;
                break; //goto 100
            }

            //20- If isEmpty(SLWordStem)
            //20.1- SLWordStem = getStem(CurrSLWord)
            if (SLWordStem.isEmpty())
                SLWordStem = Engine::instance().ExternalStemmer->getStem(*CurrSLWord, true);

            //21- if MySuggestions contains SLWordStem
            //21.1- Add CurrSLWord to KnowledgeBase for currFLWord
            //21.2- IF Skipped
            //21.2.1- Reserve CurrFLWord with SL2Reserve
            //21.3- If SL2Reserve = CurrSLWord
            //21.3.1- Accepted = true
            //21.4- goto 100
            if (MySuggestions.contains(SLWordStem))
            {
                if (Skipped)
                    this->reserve(CurrFLWord, SL2Reserve, SL2Reserve.size() == 1);
                if(SL2Reserve.size() == 1)
                    Accepted = true;
                break; //goto 100
            }

            //25 ..... TODO
            const clsTranslatedPhrase& TranslatedPhrase = HLMT::instance().translate(*CurrFLWord,WMAConfigs.OnlineLearn);
            if (TranslatedPhrase.isMultiWord())
            {
                CCSLWord = CurrSLWord;
                bool AllConsumed = false;
                int WordCount = 0;
                QList<QStringList> WholeSuggestions = TranslatedPhrase.wholeSuggestions();

                if (WholeSuggestions.size() == 1)
                {
                    AllConsumed = true;
                    QStringList AcceptedStringList = WholeSuggestions.first();
                    for (const QString& Suggestion : AcceptedStringList)
                    {
                        QStringList SuggestionWords = Suggestion.split(" ");
                        SuggestionWords.removeAll("");
                        for(const QString& Word : SuggestionWords)
                        {
                            if (CCSLWord != SLPhrase.end())
                            {
                                if (Word != *CCSLWord)
                                {
                                    AllConsumed = false;
                                    break;
                                }
                                if (CCSLWord > CurrSLWord)
                                    SL2Reserve.append(CCSLWord);
                            }
                            CCSLWord++;
                            WordCount++;
                        }
                        if (AllConsumed)
                        {

                            if (Skipped)
                                this->reserve(CurrFLWord, SL2Reserve, SL2Reserve.size() == WordCount);
                            if (SL2Reserve.size() == WordCount)
                                Accepted = true; //defenetly

                            CurrSLWord = CCSLWord - 1;
                            break; //goto 100 by next goto
                        }
                    }
                }
                else if (TranslatedPhrase.wholeSuggestions().size() > 1)
                {
                    wmaDebug<<"Seems like a bug having multiple words for single word"<<endl;
                    continue;
                }
                if (AllConsumed)
                    break; // goto 100
            }

            //30- If isEmpty(ExDicSuggestions)
            //30.1- ExDicSuggestions = translateByExternalDic(CurrFLWord)
            if (ExDicChecked == false)
            {
                ExDicSuggestions = Engine::instance().lookupExternalDic(*CurrFLWord);
                ExDicChecked = true;
            }

            //31- if ExDicSuggestions contains CurrSLWord
            //31.1- Add CurrSLWord to KnowledgeBase for CurrFLWord
            //31.2- IF Skipped
            //31.2.1- Reserve CurrFLWord with SL2Reserve
            //31.3- If SL2Reserve = CurrSLWord
            //31.3.1- Accepted = true
            //31.4- goto 100
            if (ExDicSuggestions.contains(*CurrSLWord))
            {
                Knowledge::instance().add2Dic(*CurrFLWord, QStringList()<<*CurrSLWord);
                if (Skipped)
                    this->reserve(CurrFLWord, SL2Reserve, SL2Reserve.size() == 1);
                if(SL2Reserve.size() == 1)
                    Accepted = true;
                break; //goto 100
            }

            //32- if ExDicSuggestions contains SLWordStem
            //32.1- Add SLWord to KnowledgeBase for CurrFLWord
            //32.2- IF Skipped
            //32.2.1- Reserve CurrFLWord with SL2Reserve
            //32.3- If SL2Reserve = CurrSLWord
            //32.3.1- Accepted = true
            //32.4- goto 100
            if (ExDicSuggestions.contains(SLWordStem))
            {
                Knowledge::instance().add2Dic(*CurrFLWord, QStringList()<<*CurrSLWord);
                if (Skipped)
                    this->reserve(CurrFLWord, SL2Reserve, SL2Reserve.size() == 1);
                if(SL2Reserve.size() == 1)
                    Accepted = true;
                break; //goto 100
            }

            //35- foreach ... TODO
            bool WordCombinationAccepted= false;
            for(const QString& Suggestion : ExDicSuggestions)
            {
                QStringList SuggestionWords = Suggestion.split(" ");
                SuggestionWords.removeAll("");
                if (SuggestionWords.size() > 1)
                {
                    int i=0;
                    for(const QString& SuggestedWord : SuggestionWords)
                        if ((CurrSLWord + i) != SLPhrase.end() &&
                            SuggestedWord == *(CurrSLWord+i))
                            i++;
                        else
                            break;
                    if (i == SuggestionWords.size())
                    {
                        WordCombinationAccepted = true;
                        this->add2SequenceDic(*CurrFLWord, Suggestion);
                        if (Skipped)
                            this->reserve(CurrFLWord, SL2Reserve, SL2Reserve.size() == 1);
                        if(SL2Reserve.size() == 1)
                            Accepted = true;
                        CurrSLWord += i-1;
                        break; //goto 100
                    }
                }
            }
            if (WordCombinationAccepted)
                break;//goto 100

            //45... TODO
            if (CurrFLWord->toLower() == CurrSLWord->toLower() )
            {
                if (Skipped)
                    this->reserve(CurrFLWord, SL2Reserve, SL2Reserve.size() == 1);
                if(SL2Reserve.size() == 1)
                    Accepted = true;
                break; //goto 100;
            }

            //50- if There are more SLWords
            //50.1- CurrSLWord = Next SLWord in SLPhrase
            //50.2- Skipped = true
            //50.3- SL2Reserve = SL2Reserve + CurrSLWord
            //50.3- goto 2
            if (CurrSLWord+1 != SLPhrase.end())
            {
                CurrSLWord++;
                Skipped = true;
                SL2Reserve.append(CurrSLWord);
                continue;
            }

            //51- Reserve CurrFLWord with SL2Reserve
            this->reserve(CurrFLWord, SL2Reserve, false);
            break;
        }while(true);

        //100- If Accepted == true
        //100.1- If there are more word in SLPhrase after StartingCheckPoint
        //100.1.1- StartingCheckPoint = Next Word in SLPhrase after CurrSLWord
        //100.2- goto 150
        if(Accepted)
        {
            if (StartingCheckPoint + 1 == SLPhrase.end())
                break;
            else
                StartingCheckPoint = CurrSLWord + 1;
        }

        //110- if There are more FLwords
        //110.1- set CurrFLWord = Next Word after CurrFLWord in FLPhrase
        //110.2- Accepted = false;
        //110.3- clear(MySuggestions)
        //110.4- clear(ExDicSuggestions)
        //110.5- clear(PredictedWords)
        //110.6- clear(SLWordStem)
        //110.7- clear(PhrasePart)
        //110.8- clear(SL2Reserve)
        //110.9- set CurrSLWord = StartingCheckPoint
        //110.10- goto 10
        if (CurrFLWord + 1 != FLPhrase.end())
        {
            CurrFLWord++;
            MySuggestions.clear();
            ExDicSuggestions.clear();
            ExDicChecked = false;
            KnowledgeChecked = false;
            PredictedWords.clear();
            SLWordStem.clear();
            PhrasePart.clear();
            SL2Reserve.clear();
            Accepted = false;
            CurrSLWord = StartingCheckPoint;
            SL2Reserve.append(CurrSLWord);
            continue;
        }
        break;
    }while(true);

    //150-add FLPhrase and SLPhrase to language model regions.
    Knowledge::instance().add2LM(_slPhrase);
    //Knowledge::instance().add2LM(_flPhrase);//Removed temporrily TODO use it for 2 way translation

    if (this->Reservations.size())
    {
#ifdef QT_DEBUG
        wmaDebug<<"++++++++++++++++++++++++++++++++++++++++++++++++++++++"<<endl;
        foreach(const stuReservation& Reserved, this->Reservations){
            wmaDebug<<wmaPrintable(Reserved.FirstLangTokens);
            wmaDebug<<" : \n\t";
            foreach(const stuReservedTranslation& SLT, Reserved.SecondLangTokens)
                wmaDebug<<wmaPrintable((*SLT.Iter))<<" ";
            wmaDebug<<"\n";
        }
#endif
        bool HasAnyChanges;
        do
        {
            //200- HasAnyChanges = false
            HasAnyChanges = false;
            //201- for each reservation in reverse order
            //201.1- if ReservedTranslation was accpeted
            //201.1.1- for each Reservation except current
            //201.1.1.1- Mark As cleared Translated words of the same sequence
            //201.1.2- goto 200.1.1
            //202- goto 200
            for (int Curr=this->Reservations.size() - 1; Curr>=0; Curr--)
            {
                if (this->Reservations[Curr].WasAccepted)
                    for(int New=0; New< this->Reservations.size(); New++)
                    {
                        if (Curr==New)
                            continue;
                        for(int AWIter = 0; AWIter < this->Reservations[Curr].SecondLangTokens.size(); AWIter++)
                            for(int WordIter=0; WordIter<this->Reservations[New].SecondLangTokens.size(); WordIter++)
                                if (this->Reservations[New].SecondLangTokens[WordIter].Iter ==
                                    this->Reservations[Curr].SecondLangTokens[AWIter].Iter)
                                {
                                    this->Reservations[New].SecondLangTokens[WordIter].IsCleared = true;
                                    break;
                                }
                    }
            }

            //210- for each reservation
            //210.1- If first ReservedTranslation word is maked as cleared
            //210.1.1- Remove whole consequtive marked words
            //210.1.2- HasAnyChanges = true
            //211- goto 210
            for(int CurrIter=0; CurrIter< this->Reservations.size(); CurrIter++){
                if (this->Reservations[CurrIter].SecondLangTokens.size() &&
                    this->Reservations[CurrIter].SecondLangTokens[0].IsCleared)
                    for(int WordIter=0; WordIter<this->Reservations[CurrIter].SecondLangTokens.size(); WordIter++)
                        if (this->Reservations[CurrIter].SecondLangTokens[WordIter].IsCleared)
                        {
                            this->Reservations[CurrIter].SecondLangTokens.removeAt(WordIter);
                            HasAnyChanges = true;
                            WordIter--;
                        }
            }
            //212- If HasAnyChanges == true
            //212.1- goto 200
        }while(HasAnyChanges);

        //220- for each reservation
        //220.1- if CurrentReservedTranslation is not translation of input
        //220.1.1- for each reservation from next to current
        //220.1.1.1- if NewReservedTranslation or CurrentReservedTranslation starts with
        //           the other or NewReservedTranslation has prior words than Current
        //220.1.1.1.1- merge current reservation and new reservation. mark it as sequence
        //220.1.1.1.2- goto 220.1.1
        //221- goto 220
        for(int CurrIter=0; CurrIter< this->Reservations.size() - 1; CurrIter++)
        {
            if (this->areSameTranslations(this->Reservations[CurrIter].FirstLangTokens,
                                      this->Reservations[CurrIter].SecondLangTokens) == false)
            {
                int FirstIdenticalWordIndex = 0;
                for(int NewIter=CurrIter + 1; NewIter< this->Reservations.size(); NewIter++)
                {
                    if (this->Reservations[CurrIter].SecondLangTokens.size() > 0)
                    {
                        if (this->Reservations[NewIter].SecondLangTokens.isEmpty() ||
                            this->Reservations[CurrIter].SecondLangTokens[FirstIdenticalWordIndex].Iter >=
                            this->Reservations[NewIter].SecondLangTokens[0].Iter)
                        {
                            FirstIdenticalWordIndex = this->merge(CurrIter, NewIter, FirstIdenticalWordIndex);
                            NewIter--;
                        }
                        else
                        {
                            if (this->Reservations[NewIter].AcceptedSequence &&
                                    this->Reservations[CurrIter].AcceptedSequence &&
                                    this->Reservations[CurrIter].SecondLangTokens.first().Iter ==
                                    this->Reservations[NewIter].SecondLangTokens.first().Iter)
                                continue;
                            bool SameStart = true;
                            for(int WordIter= 0;
                                WordIter < qMin(this->Reservations[NewIter].SecondLangTokens.size(),
                                                this->Reservations[CurrIter].SecondLangTokens.size() -
                                                FirstIdenticalWordIndex);
                                WordIter++)
                            {
                                if(this->Reservations[CurrIter].SecondLangTokens[WordIter + FirstIdenticalWordIndex].Iter !=
                                   this->Reservations[NewIter].SecondLangTokens[WordIter].Iter)
                                {
                                    SameStart = false;
                                    break;
                                }
                            }
                            if (SameStart)
                            {
                                this->merge(CurrIter, NewIter, FirstIdenticalWordIndex);
                                NewIter--;
                            }
                        }
                    }
                }
            }
        }

        //230- for each reservation
        //230.1- if ReservedTranslation is empty
        //230.1.1- for each reservation from next to current
        //230.1.1.1- if NewSelectedReservedTranslation is empty
        //230.1.1.1.1- merge current reservation and new reservation
        //230.1.1.1.2- goto 230.1.1
        //230.1.2- if there are reservers before this
        //230.1.2.1- add sequence to dic by merging current reservation with previous
        //230.1.3- if there are reservers after this
        //230.1.3.1- merge current reservation with next. mark it as sequence
        //231- goto 230
        for(int CurrIter=0; CurrIter< this->Reservations.size(); CurrIter++){
            if (this->Reservations[CurrIter].SecondLangTokens.isEmpty()){
                for(int NewIter=CurrIter + 1; NewIter< this->Reservations.size(); NewIter++)
                    if (this->Reservations[NewIter].SecondLangTokens.isEmpty())
                        this->merge(CurrIter, NewIter,0);
                    else
                        break;
                if (CurrIter > 0)
                {
                    this->add2SequenceDic(
                            this->Reservations[CurrIter - 1].FirstLangTokens + " " +
                        this->Reservations[CurrIter].FirstLangTokens,
                        this->Reservations[CurrIter - 1].SecondLangTokens);
                }

                if (CurrIter + 1 < this->Reservations.size())
                    this->merge(CurrIter, CurrIter + 1, 0);
            }
        }


#ifdef QT_DEBUG
        wmaDebug<<"++++++++++++++++++++++++++++++++++++++++++++++++++++++"<<endl;
        foreach(const stuReservation& Reserved, this->Reservations){
            wmaDebug<<wmaPrintable(Reserved.FirstLangTokens)<<" : \n\t";
            foreach(const stuReservedTranslation& SLT, Reserved.SecondLangTokens)
                wmaDebug<<wmaPrintable((*SLT.Iter))<<" ";
            wmaDebug<<"\n";
        }
#endif

        //240- for each reservation
        //240.1- if ReservedTranslation is marked as sequence
        //240.1.1- if Translation has words
        //240.1.1.1- Add sequence to Sequence dictionary
        for(int CurrIter=0; CurrIter< this->Reservations.size(); CurrIter++)
        {
            if (this->Reservations[CurrIter].IsSequence)
            {
                if (this->Reservations[CurrIter].SecondLangTokens.size())
                    this->add2SequenceDic(this->Reservations[CurrIter].FirstLangTokens,
                                          this->Reservations[CurrIter].SecondLangTokens);
                else
                {
                    wmaDebug<<"Seems to be a bug"<<endl;
                }
            }
            else
            {
                if (this->Reservations[CurrIter].SecondLangTokens.size() > 1)
                    this->add2SequenceDic(this->Reservations[CurrIter].FirstLangTokens,
                                          this->Reservations[CurrIter].SecondLangTokens);
            }

        }
    }/**/
}

void ILA::reserve(QStringList::Iterator &_firstLangToken,
                  const QList<QStringList::Iterator> &_secondLangTokens,
                  bool _isAccepted)
{
    wmaDebug<<"Reserved:["<<(_isAccepted ? "acc" : "res")<<"] "<<_firstLangToken->toUtf8().data()<<" --> ";
    QList<stuReservedTranslation> SecondLangReserved;
    for(QStringList::Iterator WordIter : _secondLangTokens)
    {
        SecondLangReserved.append(stuReservedTranslation(WordIter));
        wmaDebug<<WordIter->toUtf8().constData()<<" ";
    }
    wmaDebug<<std::endl;

    this->Reservations.append(stuReservation(_firstLangToken, SecondLangReserved, _isAccepted));
}

void ILA::reserve(QStringList::Iterator &_flwStart, QStringList::Iterator &_flwEnd, const QList<QStringList::Iterator> &_secondLangTokens, bool _isAccepted)
{
    QString FLWords;
    for (auto WordIter = _flwStart; WordIter<= _flwEnd; WordIter++)
        FLWords += *WordIter + " ";

    wmaDebug<<"Reserved:[seq] "<<wmaPrintable(FLWords)<<" --> ";
    QList<stuReservedTranslation> SecondLangReserved;

    for(QStringList::Iterator WordIter : _secondLangTokens)
    {
        SecondLangReserved.append(stuReservedTranslation(WordIter));
        wmaDebug<<WordIter->toUtf8().data()<<" ";
    }
    wmaDebug<<std::endl;

    this->Reservations.append(stuReservation(_flwStart, SecondLangReserved, _isAccepted, FLWords));
}

void ILA::add2SequenceDic(const QString &_firstLangTokens, const QList<stuReservedTranslation> &_secondLangTokens)
{
    QString Translation;
    for(const stuReservedTranslation& RTIter: _secondLangTokens)
        Translation += *RTIter.Iter + " ";

    this->add2SequenceDic(_firstLangTokens, Translation);
}

void ILA::add2SequenceDic(const QString &_firstLangTokens, const QString &_translation)
{
    QStringList FLWords = _firstLangTokens.split(" ");
    FLWords.removeAll("");
    if (FLWords.size() > 5)
    {
        wmaDebug<<"Phrase is to large to be added to sequence dic: "<<
                   _firstLangTokens.toUtf8().constData()<<
                   " #`"<<FLWords.size()<<"`# "<<
                   _translation.toUtf8().constData()<<std::endl;
    }
    else
    {
        QStringList SLWords = _translation.split(" ");
        SLWords.removeAll("");
        Knowledge::instance().add2SequenceDic(FLWords, SLWords);

        wmaDebug<<"Added to sequence dic: "<<wmaPrintable(_firstLangTokens)<<" = "<<
                wmaPrintable(_translation)<<std::endl;
    }
}

bool ILA::areSameTranslations(const QString &_flToken, const QList<stuReservedTranslation> &_suggestedTranslation)
{
    const clsTranslatedPhrase& TranslatedPhrase = HLMT::instance().translate(_flToken, WMAConfigs.OnlineLearn);

    if (TranslatedPhrase.isEmpty() && TranslatedPhrase.hasReserved() == false)
        return false;

    auto Reservation = _suggestedTranslation.begin();
    bool AllConsumed = true;
    QList<QStringList> WholeSuggestions = TranslatedPhrase.wholeSuggestions();

    for (const QStringList& SuggestionPart : WholeSuggestions)
    {
        for (const QString& Suggestion : SuggestionPart)
        {
            auto OldReservation = Reservation;
            QStringList SuggestionWords = Suggestion.split(" ");
            SuggestionWords.removeAll("");
            for(const QString& Word : SuggestionWords)
            {
                if (Reservation != _suggestedTranslation.end() &&
                    Word != *Reservation->Iter)
                {
                    AllConsumed = false;
                    break;
                }
                Reservation++;
            }
            if (AllConsumed)
                break;
            else
                Reservation = OldReservation;
        }
    }
    if (AllConsumed &&
        Reservation == _suggestedTranslation.end())
        return true;
    else
        return false;
}

QStringList ILA::getPredictionByKnowledge(const QStringList &_phrasePart, bool _isFirstRequest)
{
    if (_isFirstRequest) {
        Knowledge::instance().predictNextTokenByDic("",
                                                    WMAConfigs.OnlineLearn ?
                                                        clsASM::LearningFull : clsASM::AwardAndPunishment);
        Knowledge::instance().predictNextTokenByDic(TOKEN_START,
                                                    WMAConfigs.OnlineLearn ?
                                                        clsASM::LearningFull : clsASM::AwardAndPunishment);
    }
    return Knowledge::instance().predictNextTokenByDic(_phrasePart.last(),
                                                       WMAConfigs.OnlineLearn ?
                                                           clsASM::LearningFull : clsASM::AwardAndPunishment);
}

quint16 ILA::merge(quint16 _currIter, quint16 _nextIter, quint16 _firstIdenticalWordIdx)
{
    this->Reservations[_currIter].FirstLangTokens =
        this->Reservations[_currIter].FirstLangTokens + " " +
       this->Reservations[_nextIter].FirstLangTokens;

    int FirstIdenticalWordIndex=0;
    if (this->Reservations[_currIter].SecondLangTokens.size() &&
        this->Reservations[_nextIter].SecondLangTokens.size() &&
        this->Reservations[_currIter].SecondLangTokens[_firstIdenticalWordIdx].Iter >
        this->Reservations[_nextIter].SecondLangTokens[0].Iter )
    {
        while (this->Reservations[_nextIter].SecondLangTokens.size() > FirstIdenticalWordIndex &&
               this->Reservations[_nextIter].SecondLangTokens[FirstIdenticalWordIndex].Iter !=
               this->Reservations[_currIter].SecondLangTokens[0].Iter)
        {
            this->Reservations[_currIter].SecondLangTokens.insert(
                    FirstIdenticalWordIndex + _firstIdenticalWordIdx,
                    this->Reservations[_nextIter].SecondLangTokens[FirstIdenticalWordIndex]);
            FirstIdenticalWordIndex++;
        }
    }
    else if (this->Reservations[_currIter].SecondLangTokens.size() <
        this->Reservations[_nextIter].SecondLangTokens.size())
    {
        this->Reservations[_currIter].SecondLangTokens.append(
                    this->Reservations[_nextIter].SecondLangTokens.mid(
                        this->Reservations[_currIter].SecondLangTokens.size() - _firstIdenticalWordIdx));
    }

    this->Reservations[_currIter].IsSequence = true;
    this->Reservations.removeAt(_nextIter);

    return FirstIdenticalWordIndex + _firstIdenticalWordIdx;
}


