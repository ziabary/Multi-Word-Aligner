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


#include "GoogleTranslate.h"

#include <iostream>
#include <curl/curl.h>
#include "libQJSON/JSONDocument.h"
#include "libQJSON/JSONObject.h"
#include "libQJSON/JSONArray.h"

using namespace std;

GoogleTranslate* GoogleTranslate::Instance = NULL;

GoogleTranslate::GoogleTranslate() :
    intfExternalDictionary()
{}

QStringList GoogleTranslate::lookup(const QString &_word)
{
    wmaDebug<<"[Google] Looking Up: "<<wmaPrintable(_word)<<std::endl;
    QStringList Result;
    Result = this->checkCache(_word);
    if (Result.size())
    {
        wmaDebug<<"\t found in cache. "<<std::endl;
        return Result;
    }

    this->downloadURL(QString("http://translate.google.com/translate_a/t?client=t&hl=es&sl="
                              "&ie=UTF-8&oe=UTF-8&multires=1&oc=2&otf=2&ssel=0&tsel=0&sc=1"
                              "&sl=%1&tl=%2&q=%3").arg(
                          this->FirstLangID).arg(
                          this->SecondLangID).arg(
                          _word),
                      _word,
                      &Result);

    return Result;
}

void GoogleTranslate::add2Dic(const QString& _word, const QString &_translation, QStringList* _storage)
{
    _storage->append(_translation.trimmed());
    std::cout<<"\tGoogle: "<<wmaPrintable(_translation)<<std::endl;
    this->add2Cache(_word, _translation);
}

void GoogleTranslate::configure(const QString &_configArgs)
{
    Q_UNUSED(_configArgs)
    this->FirstLangID = ISO639getAlpha2(this->SourceLang.toAscii().constData());
    this->SecondLangID = ISO639getAlpha2(this->TargetLang.toAscii().constData());
    this->loadCache();
}

void GoogleTranslate::processData(const QByteArray& _buff, const QString& _word, void* _resultStorage)
{
    QJsonParseError Error;
    QStringList* Storage = (QStringList*)_resultStorage;
    QByteArray DownloadedJson = _buff;

    DownloadedJson.replace(",,",",\"\",");
    DownloadedJson.replace(",,",",\"\",");
    DownloadedJson.replace(",,",",\"\",");
    QJsonDocument JSonDoc = QJsonDocument::fromJson(DownloadedJson, &Error);

    if (Error.error != QJsonParseError::NoError)
        throw ("[Google] Json Parse Error" + Error.errorString() + QString::number(Error.offset));

    if (JSonDoc.isArray()){
        QJsonArray JsonArray = JSonDoc.array();
        if (JsonArray.size())
        {
            if (JsonArray.at(0).toArray().size() &&
                JsonArray.at(0).toArray().at(0).toArray().size())
                this->add2Dic(_word,
                              JsonArray.at(0).toArray().at(0).toArray().at(0).toString(),
                              Storage);
        }
        if (JsonArray.size() > 1 && JsonArray.at(1).isArray())
            for (QJsonValue ArrayIterValue : JsonArray.at(1).toArray())
            {
                QJsonArray JsonArray = ArrayIterValue.toArray();

                if (JsonArray.size() > 1)
                {
                    JsonArray = JsonArray.at(1).toArray();
                    for (QJsonValue ArrayInnerIterValue : JsonArray)
                        this->add2Dic(_word,
                                      ArrayInnerIterValue.toString(),
                                      Storage);
                }
                else
                    wmaDebug<<"[Google] Unkwon Tag2: "<<std::endl;
            }
    }

    return ;
}

