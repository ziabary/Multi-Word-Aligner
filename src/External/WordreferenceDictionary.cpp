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


#include "WordreferenceDictionary.h"

#include <iostream>
#include <curl/curl.h>
#include "External/JSON/JSONDocument.h"

using namespace std;
WordreferenceDictionary* WordreferenceDictionary::Instance = NULL;
QByteArray WordreferenceDictionary::DownloadedJson;

WordreferenceDictionary::WordreferenceDictionary()
{
}

QStringList WordreferenceDictionary::lookup(const QString &_word)
{
    /*this->Translations.clear();
    this->Stem.clear();
    this->Request = _word;
    this->JustStem = false;
    QString Dir = QString(enuTranslationDir::toStr(_dir)).remove("2").toLower();

    std::cout<<"WordReference.com Translating: "<<_word.toUtf8().constData()<<std::endl;
    this->downloadURL("http://api.wordreference.com/6582d/json/" + Dir + "/" + _word);
*/
    return this->Translations;
}

QString WordreferenceDictionary::getStem(const QString &_word)
{
/*    this->Translations.clear();
    this->Request = _word;
    this->JustStem = true;
    this->Stem.clear();
    QString Dir;
    if (this->JustStem)
    {
        Dir = QString(enuTranslationDir::toStr(_dir)).split("2").first().toLower();
        Dir+="fr";
    }
    else
        Dir = QString(enuTranslationDir::toStr(_dir)).remove("2").toLower();

    std::cout<<"WordReference.com retrive Stem for: "<<_word.toUtf8().constData()<<std::flush;
    this->downloadURL("http://api.wordreference.com/6582d/json/" + Dir + "/" + _word);

    std::cout<<" --> " <<this->Stem.toUtf8().constData()<<std::endl;*/
    return this->Stem;
}

void WordreferenceDictionary::storeTranslation(const QJsonObject &_object)
{
    if (JustStem)
    {
        this->Stem = _object.begin().value().toObject().value("OriginalTerm").toObject().value("term").toString();
        return;
    }

    for (QJsonValue ObjectIterValue : _object)
    {
        QString OriginalTerm =
                ObjectIterValue.toObject().value("OriginalTerm").toObject().value("term").toString();

        QJsonObject TranslationObject = ObjectIterValue.toObject();
        for(auto TranslationIter = TranslationObject.constBegin();
            TranslationIter != TranslationObject.constEnd();
            TranslationIter++)
        {
            if (TranslationIter.key().endsWith("Translation"))
            {
                QString Translation = TranslationIter.value().toObject().value("term").toString();

                std::cout<<"WordReference: "<<OriginalTerm.toUtf8().constData()
                        <<" --> "<<Translation.toUtf8().constData()<<std::endl;

                QStringList OriginalTermInnerTerms = OriginalTerm.split(",");
                for(const QString Term : OriginalTermInnerTerms)
                {
                    if (Term.contains(" ") ||
                            (Term.contains("-") && Request.contains("-") == false))
                            continue;

                    this->Translations.append(Translation.remove(" ").split(","));
                }
            }
        }
    }
}

QString WordreferenceDictionary::downloadURL(const QString &_url)
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

size_t WordreferenceDictionary::delDataDownloaded(char *_data, size_t _size, size_t _nmemb, void *)
{
    DownloadedJson+=QByteArray::fromRawData(_data, _size*_nmemb);
    return _size*_nmemb;
}

void WordreferenceDictionary::processData()
{
    QJsonParseError Error;

    QJsonDocument JSonDoc = QJsonDocument::fromJson(this->DownloadedJson, &Error);
    if (Error.error != QJsonParseError::NoError)
    {
        cerr<<(Error.errorString() + QString::number(Error.offset)).toUtf8().constData()<<endl;
        return;
    }

    if (JSonDoc.isObject())
    {
        QJsonObject JsonObject = JSonDoc.object();

        for(auto ObjectIter = JsonObject.constBegin();
            ObjectIter != JsonObject.constEnd();
            ObjectIter++)
        {
            if (ObjectIter.key().toLower() == "error") {
                return ;
            }else if(ObjectIter.key().toLower().startsWith("term")) {
                if (ObjectIter.value().toObject().contains("PrincipalTranslations"))
                    this->storeTranslation(ObjectIter.value().toObject().value("PrincipalTranslations").toObject());
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
                cerr<<"Unkwon Tag1: "<<ObjectIter.key().toUtf8().constData()<<endl;
        }
    }

    return ;
}

