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

#include "Knowledge.h"
#include <QStringList>
#include "Common.h"

Knowledge* Knowledge::Instance = NULL;

void Knowledge::init()
{
    this->MaxWordID=0;
    this->getIDByToken(TOKEN_START); //Special Token used for start
    this->getIDByToken(TOKEN_END); //Special Token used for end

    this->ASM = new clsASM(); //TODO Maybe its good to enable configuration
}

quint32 Knowledge::getIDByToken(const QString &_token)
{
    if (_token.isEmpty())
        return 0;
    quint32 ID = this->Token2ID.value(_token);
     if(!ID){
         this->MaxWordID++;
         this->Token2ID.insert(_token, MaxWordID);
         this->ID2token.insert(MaxWordID, _token);
     }
     return MaxWordID;
}

QString Knowledge::getTokenByID(quint32 _id)
{
    return this->ID2token.value(_id);
}

QStringList Knowledge::predictNextTokens(const QString &_token)
{
    QStringList PredictedTokens;
    const std::unordered_set<ColID_t>& Prediction =
            this->ASM->executeOnce(this->getIDByToken(_token));
    for(ColID_t Predicted : Prediction)
        PredictedTokens.append(this->getTokenByID(Predicted));
}

void Knowledge::save(const QString &_baseDir)
{

}

void Knowledge::load(const QString &_baseDir)
{

}

Knowledge::Knowledge()
{}

