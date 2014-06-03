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


#include "WordnetStemmer.h"

#include <iostream>
//#include "wn.h"

WordnetStemmer* WordnetStemmer::Instance = NULL;


WordnetStemmer::WordnetStemmer()
{
//    wninit(); // TODO rais exception if failed
}


QString WordnetStemmer::getStem(const QString &_word)
{
 /*   QString Stem;

    if (re_wninit())
    {
        std::cout<<"WordnetStemmer: "<<_word.toUtf8().constData()<<" -> "<<"ERROR"<<std::endl;
        return "ERROR";
    }

    strcpy(this->Word, _word.toUtf8().constData());

    for(int i=1; i<5; i++)
    {
       char* Morphed = morphword(this->Word, i);
       if (Morphed)
       {
           if (Stem.size() && Stem != Morphed)
           {
               std::cerr <<"What to do in stemmer: "<<Stem.toUtf8().constData()<<" - "<<Morphed<<std::endl;
           }
           else
               Stem = Morphed;
       }
    }

    if (Stem.isEmpty())
    {
        std::cout<<"WordnetStemmer: "<<_word.toUtf8().constData()<<" -> "<<_word.toLower().toUtf8().constData()<<std::endl;
        return _word.toLower();
    }
    else
    {
        std::cout<<"WordnetStemmer: "<<_word.toUtf8().constData()<<" -> "<<Stem.toUtf8().constData()<<std::endl;
        return Stem;
    }*/
}
