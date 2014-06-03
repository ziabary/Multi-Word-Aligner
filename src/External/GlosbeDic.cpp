/*************************************************************************
 * MWA - A Multi word alignerc
 * Copyright (C) 2012-2014  S.M.Mohammadzadeh <mehran.m@aut.ac.ir>
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
 @author S.M.Mohammadzadeh <mehran.m@aut.ac.ir>
 */


#include "GlosbeDic.h"

#include <iostream>
#include <curl/curl.h>
#include "External/JSON/JSONDocument.h"
#include "External/JSON/JSONObject.h"
#include "External/JSON/JSONArray.h"

using namespace std;

GlosbeDic* GlosbeDic::Instance = NULL;
QByteArray GlosbeDic::DownloadedJson;

GlosbeDic::GlosbeDic() :
    intfExternalDictionary()
{
    this->loadCache();
}

QStringList GlosbeDic::lookup(const QString &_word)
{
    /*this->Translations.clear();
    this->Stem.clear();
    this->Request = _word;

    switch(_dir)
    {
    case enuTranslationDir::En2De:
        this->FirstLangID  = "eng";
        this->SecondLangID = "deu";
        break;

    case enuTranslationDir::De2En:
        this->FirstLangID  = "deu";
        this->SecondLangID = "eng";
        break;

    default:
        std::cerr<<"glosbe.com invalid language combination"<<std::endl;
        return QStringList();
    }

    std::cout<<"glosbe.com Translating: "<<_word.toUtf8().constData()<<std::endl;
    this->Translations = this->checkCache(_word);
    if (this->Translations.size())
    {
        std::cout<<"\t found in cache. "<<std::endl;
        return this->Translations;
    }

    this->OriginalWord = _word;
    this->downloadURL("http://glosbe.com/gapi/translate?from="+
                      this->FirstLangID+"&dest="+
                      this->SecondLangID+"&format=json&phrase="+_word+"&pretty=true");
*/
    return this->Translations;
}

void GlosbeDic::storeTranslation(const QJsonArray &_array)
{
    for (QJsonValue ArrayIterValue : _array)
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
                    this->Translations.append(Translation.trimmed());
                    std::cout<<"\tGlosbe: "<<Translation.toUtf8().constData()<<std::endl;
                    this->add2Cache(this->OriginalWord, Translation);
                }
            }else if(ObjectIter.key().toLower() == "authors") {
            }else if(ObjectIter.key().toLower() == "meaningid") {
            }else if(ObjectIter.key().toLower() == "meanings") {
            }else
                cerr<<"Unkwon Tag2: "<<ObjectIter.key().toUtf8().constData();
        }
    }
}

QString GlosbeDic::downloadURL(const QString &_url)
{
    CURL * DownloadManager;
    CURLcode Result;

    this->DownloadedJson.clear();
     DownloadManager = curl_easy_init();
    if( DownloadManager)
    {
      curl_easy_setopt( DownloadManager, CURLOPT_URL, _url.toUtf8().constData());
      curl_easy_setopt( DownloadManager, CURLOPT_FOLLOWLOCATION, 1L);
      curl_easy_setopt(DownloadManager,CURLOPT_WRITEFUNCTION, this->delDataDownloaded);

      /* Perform the request, res will get the return code */
      Result = curl_easy_perform(DownloadManager);
      /* Check for errors */
      if(Result != CURLE_OK)
        fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(Result));

      this->processData();
      /* always cleanup */
      curl_easy_cleanup(DownloadManager);
    }
    return 0;
}

size_t GlosbeDic::delDataDownloaded(char *_data, size_t _size, size_t _nmemb, void *)
{
    DownloadedJson+=QByteArray::fromRawData(_data, _size*_nmemb);
    return _size*_nmemb;
}

void GlosbeDic::processData()
{
    QJsonParseError Error;

    QJsonDocument JSonDoc = QJsonDocument::fromJson(this->DownloadedJson, &Error);
    if (Error.error != QJsonParseError::NoError)
    {
        cerr<<(Error.errorString() + QString::number(Error.offset)).toUtf8().constData();
        return;
    }

    if (JSonDoc.isObject())
    {
        QJsonObject JsonObject = JSonDoc.object();

        for(auto ObjectIter = JsonObject.constBegin();
            ObjectIter != JsonObject.constEnd();
            ObjectIter++)
        {
            if(ObjectIter.key().toLower().startsWith("tuc")) {
                this->storeTranslation(ObjectIter.value().toArray());
            }else if(ObjectIter.key().toLower() == "result") {
            }else if(ObjectIter.key().toLower() == "dest") {
            }else if(ObjectIter.key().toLower() == "from") {
            }else if(ObjectIter.key().toLower() == "phrase") {
            }else
                cerr<<"Unkwon Tag1: "<<ObjectIter.key().toUtf8().constData();
        }
    }

    return ;
}

