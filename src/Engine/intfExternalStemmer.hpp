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

#ifndef INTFEXTERNALSTEMMER_H
#define INTFEXTERNALSTEMMER_H

#include "intfBaseExternal.hpp"

class intfExternalStemmer : virtual public intfBaseExternalComponent
{
public:
    intfExternalStemmer(){}

    virtual QString getStem(const QString& _word) = 0;

    virtual bool init(const QString& _baseDir, const QString& _sourceLang, const QString& _targetLang){
        return intfBaseExternalComponent::init("stem",_baseDir, _sourceLang, _targetLang);
    }
};

#endif // INTFEXTERNALSTEMMER_H
