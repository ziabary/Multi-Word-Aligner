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


#ifndef WORDNETSTEMMER_H
#define WORDNETSTEMMER_H

#ifdef USE_WN
#include "Engine/intfExternalStemmer.hpp"

class WordnetStemmer : public intfExternalStemmer
{
public:
    static inline WordnetStemmer* instance(){
        return Instance ? Instance : (Instance = new WordnetStemmer);
    }

    QString     getStem(const QString &_word);

private:
    void configure(const QString &_configArgs);

private:
    WordnetStemmer();
    static WordnetStemmer* Instance;
};

#else
#include "NullDicAndStemmer.h"
class WordnetStemmer : NullDicAndStemmer{

};

#endif // USE_WN
#endif // WORDNETSTEMMER_H
