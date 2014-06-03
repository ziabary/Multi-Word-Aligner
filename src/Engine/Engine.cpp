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


#include "Engine.h"

/**********************************/
#include "External/CachedDictionary.h"
#include "External/NullDicAndStemmer.h"
#include "External/InteractiveDicAndStemmer.h"
/**********************************/


static stuExternalComponent Dictionaries[] = {
    {"null", "NULL", NullDicAndStemmer::instance(), false},
    {"cd", "cached", CachedDictionary::instance(), true},
    {"in", "interactive", InteractiveDicAndStemmer::instance(), true},
    {"","",NULL,false}
};

static stuExternalComponent Stemmers[] = {
    {"null", "NULL", NullDicAndStemmer::instance(), false},
    {"in", "interactive", InteractiveDicAndStemmer::instance(), true},
    {"","",NULL,false}
};

static intfBaseExternalComponent* getcomponentInstance(const QString& _dic, stuExternalComponent* _component){
    while(_component->Instance){
        if (_dic == _component->ShortName || _dic == _component->FullName)
            return _component->Instance;
        _component ++;
    }

    return NULL;
}

static QStringList getComponentNames(stuExternalComponent* _component){
    QStringList Names;
    while(_component->Instance){
        if (_component->Selectable)
            Names.append(_component->ShortName + "/" + _component->FullName);
        _component ++;
    }

    return Names;
}

/************************************************************************************************/
Engine* Engine::Instance = NULL;

Engine::Engine()
{
}

bool Engine::initialize(intfExternalDictionary* _externDic,
                        intfExternalStemmer* _externStemmer,
                        const QString& _output,
                        const QString& _sourceLang,
                        const QString& _targetLang)
{
    this->ExternalDic = _externDic ? _externDic : CachedDictionary::instance();
    this->ExternalStemmer = _externStemmer ? _externStemmer : NullDicAndStemmer::instance();
    this->OutputDir = _output;

    if (!this->ExternalDic->init(OutputDir + "/", _sourceLang, _targetLang) ||
        !this->ExternalStemmer->init(OutputDir + "/", _sourceLang, _targetLang))
        return false;
    return true;
}

intfExternalDictionary* Engine::getDicInstance(const QString& _dic)
{
    return dynamic_cast<intfExternalDictionary*>(getcomponentInstance(_dic, Dictionaries));
}

intfExternalStemmer* Engine::getStemmerInstance(const QString& _stemmer)
{
    return dynamic_cast<intfExternalStemmer*>(getcomponentInstance(_stemmer, Stemmers));
}

QStringList Engine::validStemmers()
{
    return getComponentNames(Dictionaries);
}

QStringList Engine::validDics()
{
    return getComponentNames(Stemmers);
}

