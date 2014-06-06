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


#include "GlosbeDic.h"

#include <curl/curl.h>
#include "libQJSON/JSONDocument.h"
#include "libQJSON/JSONObject.h"
#include "libQJSON/JSONArray.h"
#include "Common.h"

GlosbeDic* GlosbeDic::Instance = NULL;

GlosbeDic::GlosbeDic() :
    intfExternalDictionary()
{}

QStringList GlosbeDic::lookup(const QString &_word)
{
    wmaDebug<<"[Glosbe] Looking Up: "<<wmaPrintable(_word)<<std::endl;
    QStringList Result;
    Result = this->checkCache(_word);
    if (Result.size())
    {
        wmaDebug<<"\t found in cache. "<<std::endl;
        return Result;
    }

    this->downloadURL(QString("http://glosbe.com/gapi/translate?format=json&pretty=false"
                              "&from=%1&dest=%2&phrase=%3").arg(
                          this->FirstLangID).arg(
                          this->SecondLangID).arg(_word),
                      _word,
                      &Result);

    return Result;
}

void GlosbeDic::configure(const QString &_configArgs)
{
    Q_UNUSED(_configArgs)
    this->FirstLangID = ISO639getAlpha3B(this->SourceLang.toAscii().constData());
    this->SecondLangID = ISO639getAlpha3B(this->TargetLang.toAscii().constData());
    this->loadCache();
}

void GlosbeDic::processData(const QByteArray& _buff, const QString& _word, void* _resultStorage)
{
    QJsonParseError Error;
    QStringList* Storage = (QStringList*)_resultStorage;

    QJsonDocument JSonDoc = QJsonDocument::fromJson(_buff, &Error);
    if (Error.error != QJsonParseError::NoError)
        throw ("[Glosbe] Json Parse Error" + Error.errorString() + QString::number(Error.offset));

    if (JSonDoc.isObject()){
        QJsonObject JsonObject = JSonDoc.object();

        for(auto ObjectIter = JsonObject.constBegin();
            ObjectIter != JsonObject.constEnd();
            ObjectIter++)
        {
            if(ObjectIter.key().toLower().startsWith("tuc")) {
                for (QJsonValue ArrayIterValue : ObjectIter.value().toArray())
                {
                    QJsonObject JsonObject = ArrayIterValue.toObject();

                    for(auto ObjectIter = JsonObject.constBegin();
                        ObjectIter != JsonObject.constEnd();
                        ObjectIter++)
                    {
                        if(ObjectIter.key().toLower().startsWith("phrase")) {
                            if (ObjectIter.value().toObject().value("language").toString() == this->SecondLangID)
                            {
                                QString Translation = ObjectIter.value().toObject().value("text").toString();
                                Storage->append(Translation.trimmed());
                                this->add2Cache(_word, Translation);
                            }
                        }else if(ObjectIter.key().toLower() == "authors") {
                        }else if(ObjectIter.key().toLower() == "meaningid") {
                        }else if(ObjectIter.key().toLower() == "meanings") {
                        }else
                            wmaDebug<<"[Glosbe] Unkwon JSON Tag2: "<<wmaPrintable(ObjectIter.key())<<std::endl;
                    }
                }
            }else if(ObjectIter.key().toLower() == "result") {
            }else if(ObjectIter.key().toLower() == "dest") {
            }else if(ObjectIter.key().toLower() == "from") {
            }else if(ObjectIter.key().toLower() == "phrase") {
            }else
                wmaDebug<<"[Glosbe] Unkwon JSON Tag1: "<<wmaPrintable(ObjectIter.key())<<std::endl;
        }
    }
}

