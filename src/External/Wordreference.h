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


#ifndef CLSWORDREFERENCE_H
#define CLSWORDREFERENCE_H

#include "Engine/intfExternalDictionary.hpp"
#include "Engine/intfExternalStemmer.hpp"
#include "libQJSON/JSONObject.h"

class clsWordreferenceBase : virtual public intfBaseExternalComponent
{
protected:
    void processData(const QByteArray &_buff, const QString &_word, void *_resultStorage);
    void add2Dic(const QString &_word, const QString& _translation, QStringList *_storage);
    void configure(const QString &_configArgs);

    QString Dir;
};

/**
 * @brief The WordreferenceDic class
 */
class WordreferenceDic : public clsWordreferenceBase, intfExternalDictionary
{
public:
    static inline WordreferenceDic* instance(){
        return Instance ? Instance : (Instance = new WordreferenceDic);
    }

    QStringList lookup(const QString &_word);

private:
    WordreferenceDic(){}
    static WordreferenceDic* Instance;
};
/**
 * @brief The WordreferenceStemmer class
 */
class WordreferenceStemmer : public clsWordreferenceBase, intfExternalStemmer
{
public:
    static inline WordreferenceStemmer* instance(){
        return Instance ? Instance : (Instance = new WordreferenceStemmer);
    }

    QString getStem(const QString &_word);

private:
    WordreferenceStemmer(){}
    static WordreferenceStemmer* Instance;
};

#endif // CLSWORDREFERENCE_H
