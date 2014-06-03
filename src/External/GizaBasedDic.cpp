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


#include "GizaBasedDic.h"

#include <iostream>

GizaBasedDic* GizaBasedDic::Instance = NULL;

GizaBasedDic::GizaBasedDic()
{
}

QStringList GizaBasedDic::lookup(const QString &_word)
{
    QStringList Out;

    /*for(const stuTranslation& Translation : this->Dictionaries[_dir].values(_word))
    {
        if (Translation.Prob > .1)
            Out<<Translation.Word;
    }*/
    return Out;
}

bool GizaBasedDic::configure(const QString &_configArgs)
{
 /*   QFile File(_baseDir);
    QTextStream Text(&File);
    Text.setCodec("UTF-8");
    if (File.open(QFile::ReadOnly) == false)
    {
        qCritical("Unable to open %s file", qPrintable(_baseDir));
        return false;
    }

    QString Line;
    QStringList LineParts;
    int i = 0;
    QString FLWord;
    QString SLWord;
    qreal Count;


    QHash<QString, qreal> Summations;

    while(!File.atEnd())
    {
        i++;
        Line = Text.readLine().trimmed();
        LineParts = Line.split("#");

        if (LineParts.size() < 3 || LineParts.first().trimmed().toDouble() <= 0)
        {
            qWarning("Invalid combination at line: %d", i);
            continue;
        }

        FLWord = LineParts.at(1).trimmed();
        SLWord = LineParts.last().trimmed();
        Count = LineParts.first().trimmed().toDouble();

        this->Dictionaries[_sourceLang].insertMulti(FLWord, stuTranslation(SLWord, Count));
        Summations.insert(FLWord, Summations.value(FLWord) + Count);
    }

    for(auto DicIter = this->Dictionaries[_sourceLang].begin();
        DicIter != this->Dictionaries[_sourceLang].end();
        DicIter++)
    {
        qreal A = Summations.value(DicIter.key());
        DicIter.value().Prob /= Summations.value(DicIter.key());
    }*/
}
