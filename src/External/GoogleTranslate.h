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

#ifndef GOOGLETRANSLATE_H
#define GOOGLETRANSLATE_H

#include "Engine/intfExternalDictionary.hpp"
#include "Engine/intfExternalStemmer.hpp"
#include "External/JSON/JSONObject.h"

class GoogleTranslate : public intfExternalDictionary
{
public:
    static inline GoogleTranslate* instance(){
        return Instance ? Instance : (Instance = new GoogleTranslate);
    }

    QStringList lookup(const QString &_word);

private:
    void storeTranslation(const QJsonArray &_array);
    QString downloadURL(const QString &_url);
    void processData();

    static size_t delDataDownloaded(char *_data, size_t _size, size_t _nmemb, void *);

    void add2Dic(const QString& _translation);

private:
    GoogleTranslate();
    static GoogleTranslate* Instance;

    QStringList Translations;
    QString     Stem;
    QString Request;
    static QByteArray DownloadedJson;
    QString FirstLangID;
    QString SecondLangID;
    QString OriginalWord;
};

#endif // GOOGLETRANSLATE_H
