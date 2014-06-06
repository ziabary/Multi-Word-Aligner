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


#include "GizaBasedDic.h"

#include <iostream>

GizaBasedDic* GizaBasedDic::Instance = NULL;

GizaBasedDic::GizaBasedDic()
{
}

QStringList GizaBasedDic::lookup(const QString &_word)
{
    QStringList Out;

    for(const stuTranslation& Translation : this->Dictionary.values(_word))
        if (Translation.Prob > .1)
            Out<<Translation.Word;
    return Out;
}

void GizaBasedDic::configure(const QString &_configArgs)
{
    if (_configArgs.split(";").size() < 2)
        throw exExternDic("Giza configuration not specified (GizaFilePath;MinProbToAccept)");

    bool Ok;
    qreal MinProb = _configArgs.split(";").last().toFloat(&Ok);
    if (!Ok || MinProb <=0)
        throw exExternDic("Invalid minimum probalbilty specified. ( 0<MinProb<1 )");

    QFile File(_configArgs.split(";").first());
    QTextStream Text(&File);
    Text.setCodec("UTF-8");
    if (File.open(QFile::ReadOnly) == false)
        throw exExternDic("Unable to open GizaFile: <"+_configArgs +">");

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
            wmaDebug<<"Invalid combination at line:"<<i<<std::endl;
            continue;
        }

        FLWord = LineParts.at(1).trimmed();
        SLWord = LineParts.last().trimmed();
        Count = LineParts.first().trimmed().toDouble();

        this->Dictionary.insertMulti(FLWord, stuTranslation(SLWord, Count));
        Summations.insert(FLWord, Summations.value(FLWord) + Count);
    }

    for(auto DicIter = this->Dictionary.begin();
        DicIter != this->Dictionary.end();
        DicIter++)
        DicIter.value().Prob /= Summations.value(DicIter.key());
}
