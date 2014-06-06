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

class Knowledge
{
public:
    static inline Knowledge& instance(){
        return Instance ? *Instance : *(Instance = new Knowledge);
    }

    void init();

    quint32 getIDByToken(const QString& _token);
    QString getTokenByID(quint32 _id);

    QStringList predictNextTokens(const QString& _token);

    void save(const QString& _baseDir);
    void load(const QString& _baseDir);

private:
    Knowledge();
    static Knowledge* Instance;

    quint32 MaxWordID;
    QHash<quint32, QString> ID2token;
    QHash<QString,quint32> Token2ID;

    clsASM* ASM;
};

#endif // KNOWLEDGE_H
