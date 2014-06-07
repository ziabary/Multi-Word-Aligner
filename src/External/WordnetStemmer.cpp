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


#include "WordnetStemmer.h"

#ifdef USE_WN
#include <iostream>
#include "wn.h"

WordnetStemmer* WordnetStemmer::Instance = NULL;


WordnetStemmer::WordnetStemmer()
{}

QString WordnetStemmer::getStem(const QString &_word, bool _reverseDir)
{
    if (_reverseDir)
        return _word; // Wordnet does not support reverse dir stemming

    QString Stem;
    char Word[_word.toUtf8().size()];
    if (re_wninit())
        throw exExternStemmer("Unable to reinit wordnet");

    strcpy(Word, wmaPrintable(_word));

    for(int i=1; i<5; i++){
       char* Morphed = morphword(Word, i);
       if (Morphed){
           if (Stem.size() && Stem != Morphed){
               wmaDebug<<"What to do in stemmer: "<<wmaPrintable(Stem)<<" - "<<Morphed<<std::endl;
           }else
               Stem = Morphed;
       }
    }

    if (Stem.isEmpty()) {
        wmaDebug<<"WordnetStemmer: "<<wmaPrintable(_word)<<" -> "<<wmaPrintable(_word.toLower())<<std::endl;
        return _word.toLower();
    }else{
        wmaDebug<<"WordnetStemmer: "<<wmaPrintable(_word)<<" -> "<<wmaPrintable(Stem)<<std::endl;
        return Stem;
    }
}

void WordnetStemmer::configure(const QString &_configArgs)
{
    Q_UNUSED(_configArgs)
    if (wninit())
        throw exExternStemmer("Unable to initialize wordnet");
}
#endif
