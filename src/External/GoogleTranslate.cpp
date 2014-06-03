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


#include "GoogleTranslate.h"

#include <iostream>
#include <curl/curl.h>
#include "External/JSON/JSONDocument.h"
#include "External/JSON/JSONObject.h"
#include "External/JSON/JSONArray.h"

using namespace std;

GoogleTranslate* GoogleTranslate::Instance = NULL;
QByteArray GoogleTranslate::DownloadedJson;

GoogleTranslate::GoogleTranslate() :
    intfExternalDictionary()
{
    this->loadCache();
}

QStringList GoogleTranslate::lookup(const QString &_word)
{
    /*this->Translations.clear();
    this->Stem.clear();
    this->Request = _word;

    switch(_dir)
    {
    case enuTranslationDir::En2De:
        this->FirstLangID  = "en";
        this->SecondLangID = "de";
        break;

    case enuTranslationDir::De2En:
        this->FirstLangID  = "de";
        this->SecondLangID = "en";
        break;

    default:
        std::cerr<<"glosbe.com invalid language combination"<<std::endl;
        return QStringList();
    }

    std::cout<<"Google Translating: "<<_word.toUtf8().constData()<<std::endl;
    this->Translations = this->checkCache(_word);
    if (this->Translations.size())
    {
        std::cout<<"\t found in cache. "<<std::endl;
        return this->Translations;
    }

    this->OriginalWord = _word;
    this->downloadURL("http://translate.google.com/translate_a/t?client=t&hl=es&sl="+
                      this->FirstLangID+"&tl="+
                      this->SecondLangID+"&ie=UTF-8&oe=UTF-8&multires=1&oc=2&otf=2&ssel=0&tsel=0&sc=1&q="+_word);
*/
    return this->Translations;
}

void GoogleTranslate::storeTranslation(const QJsonArray &_array)
{
    for (QJsonValue ArrayIterValue : _array)
    {
        QJsonArray JsonArray = ArrayIterValue.toArray();

        if (JsonArray.size() > 1)
        {
            JsonArray = JsonArray.at(1).toArray();
            for (QJsonValue ArrayInnerIterValue : JsonArray)
                this->add2Dic(ArrayInnerIterValue.toString());
        }
        else
            cerr<<"Unkwon Tag2: "<<endl;
    }
}

QString GoogleTranslate::downloadURL(const QString &_url)
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

size_t GoogleTranslate::delDataDownloaded(char *_data, size_t _size, size_t _nmemb, void *)
{
    DownloadedJson+=QByteArray::fromRawData(_data, _size*_nmemb);
    return _size*_nmemb;
}

void GoogleTranslate::add2Dic(const QString &_translation)
{
    this->Translations.append(_translation.trimmed());
    std::cout<<"\tGoogle: "<<_translation.toUtf8().constData()<<std::endl;
    this->add2Cache(this->OriginalWord, _translation);
}

void GoogleTranslate::processData()
{
    QJsonParseError Error;

    this->DownloadedJson.replace(",,",",\"\",");
    this->DownloadedJson.replace(",,",",\"\",");
    this->DownloadedJson.replace(",,",",\"\",");
    QJsonDocument JSonDoc = QJsonDocument::fromJson(this->DownloadedJson, &Error);
    if (Error.error != QJsonParseError::NoError)
    {
        cerr<<(Error.errorString() + "(" + QString::number(Error.offset)+ ")").toUtf8().constData()<<endl;
        return;
    }

    std::cout<<this->DownloadedJson.constData()<<std::endl;
    if (JSonDoc.isArray())
    {
        QJsonArray JsonArray = JSonDoc.array();
        if (JsonArray.size())
        {
            if (JsonArray.at(0).toArray().size() &&
                JsonArray.at(0).toArray().at(0).toArray().size())
                this->add2Dic(JsonArray.at(0).toArray().at(0).toArray().at(0).toString());
        }
        if (JsonArray.size() > 1 && JsonArray.at(1).isArray())
            this->storeTranslation(JsonArray.at(1).toArray());
    }

    return ;
}

