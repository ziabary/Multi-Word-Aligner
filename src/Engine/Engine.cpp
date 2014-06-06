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


#include "Engine.h"

#include "Knowledge.h"
#include "ILA.h"

/**********************************/
#include "External/CachedDictionary.h"
#include "External/NullDicAndStemmer.h"
#include "External/InteractiveDicAndStemmer.h"
#include "External/GizaBasedDic.h"
#include "External/GlosbeDic.h"
#include "External/GoogleTranslate.h"
#include "External/WordnetStemmer.h"
#include "External/Wordreference.h"
/**********************************/

static stuExternalComponent Dictionaries[] = {
    {"null", "NULL", NullDicAndStemmer::instance(), false},
    {"cd", "cached", CachedDictionary::instance(), true},
    {"in", "interactive", InteractiveDicAndStemmer::instance(), true},
    {"gz", "giza", GizaBasedDic::instance(), true},
    {"gl", "glosbe", GlosbeDic::instance(), true},
    {"gg", "google", GoogleTranslate::instance(), true},
    {"wr", "wordreference", WordreferenceDic::instance(), true},
    {"","",NULL,false}
};

static stuExternalComponent Stemmers[] = {
    {"null", "NULL", NullDicAndStemmer::instance(), false},
    {"in", "interactive", InteractiveDicAndStemmer::instance(), true},
    {"wn", "wordnet", WordreferenceStemmer::instance(),
     #ifdef USE_WN
     true
     #else
     false
     #endif
    },
    {"wr", "wordreference", WordreferenceStemmer::instance(), true},
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

void Engine::initialize(intfExternalDictionary* _externDic,
                        intfExternalStemmer* _externStemmer,
                        const QString& _output,
                        const QString& _sourceLang,
                        const QString& _targetLang,
                        const QString& _externDicArgs,
                        const QString& _externStemmerArgs)
{
    this->ExternalDic = _externDic ? _externDic : CachedDictionary::instance();
    this->ExternalStemmer = _externStemmer ? _externStemmer : NullDicAndStemmer::instance();
    this->OutputDir = _output;

    if (_sourceLang.isEmpty())
        throw exEngine("Source language not specified");

    if (_targetLang.isEmpty())
        throw exEngine("Target language not specified");

    this->ExternalDic->init(OutputDir + "/", _sourceLang, _targetLang, _externDicArgs);
    this->ExternalStemmer->init(OutputDir + "/", _sourceLang, _targetLang, _externStemmerArgs);

    Knowledge::instance().init();
}

void Engine::process(const QString &_sourceFile, const QString &_targetFile)
{
    QFile SourceFile(_sourceFile);
    if (!SourceFile.open(QFile::ReadOnly))
        throw exEngine("Unable to open source file: <" + _sourceFile + "> for reading");
    QFile TargetFile(_targetFile);
    if (!TargetFile.open(QFile::ReadOnly))
        throw exEngine("Unable to open target file: <" + _targetFile + "> for reading");

    QTextStream SourceStream(&SourceFile);
    SourceStream.setCodec("UTF-8");

    QTextStream TargetStream(&TargetFile);
    TargetStream.setCodec("UTF-8");

    QString SourceLine;
    QString TargetLine;

    Knowledge::instance().load(this->OutputDir);

    while(!SourceStream.atEnd() && ! TargetStream.atEnd()){
        SourceLine = SourceStream.readLine().trimmed();
        TargetLine = TargetStream.readLine().trimmed();

        ILA::instance().process(SourceLine, TargetLine);

        Knowledge::instance().save(this->OutputDir);
    }

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
    return getComponentNames(Stemmers);
}

QStringList Engine::validDics()
{
    return getComponentNames(Dictionaries);
}

