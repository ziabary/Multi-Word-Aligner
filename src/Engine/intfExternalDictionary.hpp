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

#ifndef INTFEXTERNALDICTIONARY_H
#define INTFEXTERNALDICTIONARY_H

#include "intfBaseExternal.hpp"

MWA_ADD_EXCEPTION_HANDLER(exExternDic, exExternComponent)

class intfExternalDictionary : virtual public intfBaseExternalComponent
{
public:
    intfExternalDictionary(){}

    virtual QStringList lookup(const QString& _word) = 0;

    void init(const QString& _baseDir, const QString& _sourceLang, const QString& _targetLang, const QString& _configArgs){
         intfBaseExternalComponent::init("dic",_baseDir, _sourceLang, _targetLang);
         this->configure(_configArgs);
    }

private:
    quint32 LastInsertID;
};

#endif // INTFEXTERNALDICTIONARY_H
