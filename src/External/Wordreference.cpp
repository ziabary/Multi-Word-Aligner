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


#include "Wordreference.h"

#include <iostream>
#include <curl/curl.h>
#include "libQJSON/JSONDocument.h"

using namespace std;
WordreferenceDic* WordreferenceDic::Instance = NULL;
WordreferenceStemmer* WordreferenceStemmer::Instance = NULL;

QStringList WordreferenceDic::lookup(const QString &_word)
{
    wmaDebug<<"[WordReference] Looking Up: "<<wmaPrintable(_word)<<std::endl;
    QStringList Result;
    Result = this->checkCache(_word);
    if (Result.size())
    {
        wmaDebug<<"\t found in cache. "<<std::endl;
        return Result;
    }

    this->downloadURL(QString("http://api.wordreference.com/6582d/json/%1/%2").arg(
                          Dir).arg(
                          _word),
                      _word,
                      &Result);

    return Result;
}

QString WordreferenceStemmer::getStem(const QString &_word, bool _reverseOrder)
{
    wmaDebug<<"[WordReference] Looking Up: "<<wmaPrintable(_word)<<std::endl;
    QStringList Result;
    Result.append(_word);
    Result = this->checkCache(_word);
    if (Result.size())
    {
        wmaDebug<<"\t found in cache. "<<std::endl;
        return Result.first();
    }

    this->downloadURL(QString("http://api.wordreference.com/6582d/json/%1/%2").arg(
                          _reverseOrder ? this->ReverseDir : this->Dir).arg(
                          _word),
                      _word,
                      &Result);

    return Result.first();
}

void clsWordreferenceBase::processData(const QByteArray &_buff, const QString &_word, void *_resultStorage)
{
    QJsonParseError Error;
    QStringList*  Storage = (QStringList*)_resultStorage;
    bool Full = Storage->isEmpty();
    Storage->clear();
    QJsonDocument JSonDoc = QJsonDocument::fromJson(_buff, &Error);

    if (Error.error != QJsonParseError::NoError)
        throw ("[Wordreference] Json Parse Error" + Error.errorString() + QString::number(Error.offset));

    if (JSonDoc.isObject()){
        QJsonObject JsonObject = JSonDoc.object();

        for(auto ObjectIter = JsonObject.constBegin();
            ObjectIter != JsonObject.constEnd();
            ObjectIter++)
        {
            if (ObjectIter.key().toLower() == "error") {
                throw ("[Wordreference] Error: " + ObjectIter.value().toString());
            }else if(ObjectIter.key().toLower().startsWith("term")) {
                if (ObjectIter.value().toObject().contains("PrincipalTranslations")){
                    if (!Full){
                        QString Stem = ObjectIter.value().toObject().value(
                                    "PrincipalTranslations").toObject().begin().value().toObject().value(
                                    "OriginalTerm").toObject().value("term").toString();
                        Storage->append(Stem);
                        this->add2Cache(_word, Stem);
                        return;
                    }

                    for (QJsonValue ObjectIterValue :
                         ObjectIter.value().toObject().value("PrincipalTranslations").toObject())
                    {
                        QString OriginalTerm =
                                ObjectIterValue.toObject().value(
                                    "OriginalTerm").toObject().value("term").toString();

                        QJsonObject TranslationObject = ObjectIterValue.toObject();
                        for(auto TranslationIter = TranslationObject.constBegin();
                            TranslationIter != TranslationObject.constEnd();
                            TranslationIter++)
                        {
                            if (TranslationIter.key().endsWith("Translation")) {
                                QString Translation = TranslationIter.value().toObject().value("term").toString();

                                wmaDebug<<"WordReference: "<<wmaPrintable(_word)<<
                                           " --> "<<wmaPrintable(Translation)<<std::endl;

                                QStringList OriginalTermInnerTerms = OriginalTerm.split(",");
                                for(const QString Term : OriginalTermInnerTerms)
                                {
                                    if (Term.contains(" ") ||
                                            (Term.contains("-") && _word.contains("-") == false))
                                            continue;

                                    Storage->append(Translation.remove(" ").split(","));
                                }
                            }
                        }
                    }
                }
            }else if(ObjectIter.key().toLower() == "original") {
/*                for(auto Inner1ObjectIter = ObjectIter.value().toObject().constBegin();
                    Inner1ObjectIter != ObjectIter.value().toObject().constEnd();
                    Inner1ObjectIter++)
                {
                    if(Inner1ObjectIter.key().toLower() == "compounds") {
                        ContinueCompound = this->storeTranslation(
                                    Inner1ObjectIter.value().toObject(),
                                    _request);
                    }else
                        qWarning()<<"Unkwon Tag1-1: "<<Inner1ObjectIter.key();
                }*/
            }else if(ObjectIter.key().toLower() == "lines") {
            }else if(ObjectIter.key().toLower() == "end") {
            }else if(ObjectIter.key().toLower() == "response") {
            }else if(ObjectIter.key().toLower() == "url") {
            }else
                wmaDebug<<"Unkwon Tag1: "<<wmaPrintable(ObjectIter.key())<<std::endl;
        }
    }

    return ;
}

void clsWordreferenceBase::configure(const QString &_configArgs)
{
    Q_UNUSED(_configArgs)
    this->Dir = QString(ISO639getAlpha2(wmaPrintable(this->SourceLang))) +
                QString(ISO639getAlpha2(wmaPrintable(this->TargetLang)));
}

