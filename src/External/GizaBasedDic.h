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


#ifndef GIZABASEDDIC_H
#define GIZABASEDDIC_H

#include "Engine/intfExternalDictionary.hpp"

class GizaBasedDic : public intfExternalDictionary
{
    struct stuTranslation
    {
        QString Word;
        qreal   Prob;
        stuTranslation(const QString& _word = "", qreal _prob = 0)
        {
            this->Word = _word;
            this->Prob = _prob;
        }
    };

public:
    static inline GizaBasedDic* instance(){
        return Instance ? Instance : (Instance = new GizaBasedDic);
    }

    QStringList lookup(const QString &_word);
    bool configure(const QString &_configArgs);

private:
    GizaBasedDic();
    static GizaBasedDic* Instance;

    QHash<QString, stuTranslation> Dictionary;
};

#endif // CLSWORDREFERENCE_H
