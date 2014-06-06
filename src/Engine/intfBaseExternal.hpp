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

#ifndef INTFBASEEXTERNAL_HPP
#define INTFBASEEXTERNAL_HPP

#include <QStringList>
#include <QHash>
#include <QFile>
#include <QTextStream>
#include <curl/curl.h>
#include "exMWABase.hpp"
#include "Common.h"
#include "ISO639.h"

MWA_ADD_EXCEPTION_HANDLER(exExternComponent, exMWABase)

class intfBaseExternalComponent
{
public:
    intfBaseExternalComponent(){}

protected: //Virtuals
    virtual void processData(const QByteArray& _buff, const QString& _word, void* _resultStorage){
        Q_UNUSED(_buff);
        Q_UNUSED(_word);
        Q_UNUSED(_resultStorage);
        throw exExternComponent("processData() must be implemented in subclasses");
    }

    virtual void configure(const QString& _configArgs){Q_UNUSED(_configArgs)}

protected:
    void init(const QString& _type,
              const QString& _baseDir,
              const QString& _sourceLang,
              const QString& _targetLang){
        this->SourceLang = _sourceLang.toLower();
        this->TargetLang = _targetLang.toLower();
        this->BaseDir = _baseDir;
        this->CacheExtension = ".c" + _type;
        if (!ISO639isValid(this->SourceLang.toAscii().constData()) ||
            !ISO639isValid(this->TargetLang.toAscii().constData()))
            throw exExternComponent("Invalid Source or Target language");
    }

    void loadCache()
    {
        QFile File(this->BaseDir +this->SourceLang+"2"+this->TargetLang+ CacheExtension);
        if (File.open(QFile::ReadOnly))
        {
            QTextStream Dictionary(&File);
            Dictionary.setCodec("UTF-8");
            QStringList Words;

            while (!Dictionary.atEnd())
            {
                Words = Dictionary.readLine().trimmed().split(";:;");
                if (Words.size() == 2)
                    this->OldEntries.insertMulti(Words.first(), Words.last());
            }
        }
    }

    void add2Cache(const QString& _flWord, const QString& _translation)
    {
        QFile File(this->BaseDir +this->SourceLang+"2"+this->TargetLang+"."+this->CacheExtension);
        if (File.open(QFile::Append))
        {
            QTextStream Dictionary(&File);
            Dictionary.setCodec("UTF-8");
            Dictionary<<_flWord << ";:;" << _translation<<endl;
        }
        this->OldEntries.insertMulti(_flWord, _translation);
    }

    QStringList checkCache(const QString& _word) const {
        return this->OldEntries.values(_word);
    }

    QString downloadURL(const QString &_url, const QString& _input, void* _resultStorage) {
        CURL * DownloadManager;
        CURLcode Result;

        QByteArray Storage;
        DownloadManager = curl_easy_init();
        if( DownloadManager)
        {
          curl_easy_setopt(DownloadManager, CURLOPT_URL, wmaPrintable(_url));
          curl_easy_setopt(DownloadManager, CURLOPT_FOLLOWLOCATION, 1L);
          curl_easy_setopt(DownloadManager,CURLOPT_WRITEFUNCTION, this->delDataDownloaded);
          curl_easy_setopt(DownloadManager, CURLOPT_WRITEDATA, &Storage);

          Result = curl_easy_perform(DownloadManager);
          if(Result != CURLE_OK)
              throw exExternComponent("lookup failed:" + QString(curl_easy_strerror(Result)));

          this->processData(Storage, _input, _resultStorage);

          curl_easy_cleanup(DownloadManager);
        }
        return "";
    }

private:
    static size_t delDataDownloaded(char *_data, size_t _size, size_t _nMemb, void * _storage){
        ((QByteArray*)_storage)->append(QByteArray::fromRawData(_data, _size*_nMemb));
        return _size * _nMemb;
    }

protected:
    QString SourceLang;
    QString TargetLang;

private:
    QString BaseDir;
    QString CacheExtension;
    QHash<QString, QString> OldEntries;
};


struct stuExternalComponent{
    QString ShortName;
    QString FullName;
    intfBaseExternalComponent* Instance;
    bool Selectable;
};

#endif // INTFBASEEXTERNAL_HPP
