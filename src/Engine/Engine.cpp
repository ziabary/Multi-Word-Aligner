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
    {NullDicAndStemmer::instance(), false},
    {CachedDictionary::instance(), true},
    {InteractiveDicAndStemmer::instance(), true},
    {GizaBasedDic::instance(), true},
    {GlosbeDic::instance(), true},
    {GoogleTranslate::instance(), true},
    {WordreferenceDic::instance(), true},
    {NULL,false}
};

static stuExternalComponent Stemmers[] = {
    {NullDicAndStemmer::instance(), false},
    {InteractiveDicAndStemmer::instance(), true},
    {WordreferenceStemmer::instance(),
     #ifdef USE_WN
     true
     #else
     false
     #endif
    },
    {WordreferenceStemmer::instance(), true},
    {NULL,false}
};

static intfBaseExternalComponent* getcomponentInstance(const QString& _dic, stuExternalComponent* _component){
    while(_component->Instance){
        if (_dic == _component->Instance->shortName() || _dic == _component->Instance->fullName())
            return _component->Instance;
        _component ++;
    }

    return NULL;
}

static QStringList getComponentNames(stuExternalComponent* _component){
    QStringList Names;
    while(_component->Instance){
        if (_component->Selectable)
            Names.append(_component->Instance->shortName() + "/" + _component->Instance->fullName());
        _component ++;
    }

    return Names;
}

/************************************************************************************************/
Engine* Engine::Instance = NULL;

Engine::Engine()
{
}

void Engine::initialize(QList<intfExternalDictionary*> _externDics,
                        intfExternalStemmer* _externStemmer,
                        const QString& _output,
                        const QString& _sourceLang,
                        const QString& _targetLang,
                        const QStringList& _externDicsArgs,
                        const QString& _externStemmerArgs)
{
    if (_externDics.isEmpty())
        this->ExternalDics.append(CachedDictionary::instance());
    else
        this->ExternalDics = _externDics;

    this->ExternalStemmer = _externStemmer ? _externStemmer : NullDicAndStemmer::instance();
    this->OutputDir = _output;

    if (_sourceLang.isEmpty())
        throw exEngine("Source language not specified");

    if (_targetLang.isEmpty())
        throw exEngine("Target language not specified");

    int i=0;
    foreach(intfExternalDictionary* Dic, this->ExternalDics){
        Dic->init(OutputDir + "/", _sourceLang, _targetLang, _externDicsArgs.isEmpty() ? "" : _externDicsArgs[i]);
        i++;
    }

    this->ExternalStemmer->init(OutputDir + "/", _sourceLang, _targetLang, _externStemmerArgs);

    Knowledge::instance().init(OutputDir + "/", _sourceLang, _targetLang);
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

    while(!SourceStream.atEnd() && ! TargetStream.atEnd()){
        SourceLine = SourceStream.readLine().trimmed();
        TargetLine = TargetStream.readLine().trimmed();

        ILA::instance().process(SourceLine, TargetLine);

        Knowledge::instance().save(this->OutputDir + "/");
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

QStringList Engine::lookupExternalDic(const QString &_token, bool _all)
{
    QStringList Result;
    foreach (intfExternalDictionary* ExtDic, this->ExternalDics){
        Result = ExtDic->lookup(_token);
        if (!_all && Result.size())
            break;
    }
    return Result;
}

