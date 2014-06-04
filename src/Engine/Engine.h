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

#ifndef ENGINE_H
#define ENGINE_H

#include <QString>
class intfExternalDictionary;
class intfExternalStemmer;

class Engine
{
public:
    static inline Engine& instance(){
        return Instance ? *Instance : *(Instance = new Engine);
    }

    void initialize(intfExternalDictionary* _externDic,
                    intfExternalStemmer* _externStemmer,
                    const QString& _output,
                    const QString &_sourceLang,
                    const QString &_targetLang, const QString &_externDicArgs, const QString &_externStemmerArgs);

    intfExternalDictionary* getDicInstance(const QString& _dic);
    intfExternalStemmer* getStemmerInstance(const QString& _dic);
    QStringList validStemmers();
    QStringList validDics();

private:
    Engine();
    static Engine* Instance;

    intfExternalDictionary* ExternalDic;
    intfExternalStemmer*    ExternalStemmer;
    QString                 OutputDir;
};

#endif // ENGINE_H
