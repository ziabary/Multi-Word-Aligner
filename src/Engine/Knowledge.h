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

#ifndef KNOWLEDGE_H
#define KNOWLEDGE_H

#include <QHash>
#include <QString>
#include "libASM/clsASM.h"
#include "External/CachedDictionary.h"

class Knowledge
{
public:
    static inline Knowledge& instance(){
        return Instance ? *Instance : *(Instance = new Knowledge);
    }

    void init(const QString &_baseDir, const QString _sourceLang, const QString _targetLang);

    quint32 getIDByToken(const QString& _token, bool _addIfNotExist = true);
    QString getTokenByID(quint32 _id);

    QStringList predictNextTokenByDic(const QString& _token, clsASM::enuLearningLevel _learningLevel);
    void add2SequenceDic(const QStringList &_flWords, const QStringList &_slWords);

    QStringList predictNextTokenByLM(const QString& _token, clsASM::enuLearningLevel _learninglevel);
    void add2LM(const QString _phrase);

    QStringList lookupDic(const QString& _word);
    void add2Dic(const QString _word, const QStringList _translations);
    void save(const QString& _baseDir);
    void load(const QString& _baseDir);

private:
    Knowledge();
    QStringList predictNextByASM(clsASM* _asm, const QString _token, clsASM::enuLearningLevel _learningLevel);

private:
    static Knowledge* Instance;

    quint32 MaxWordID;
    QHash<quint32, QString> ID2token;
    QHash<QString,quint32> Token2ID;

    clsASM* SequenceDic;
    clsASM* LM;
    CachedDictionary* WordDic;

    QString SeqDicFile;
    QString LMFile;
    QString WordIDFile;

    quint32 LastIDInFile;
};

#endif // KNOWLEDGE_H
