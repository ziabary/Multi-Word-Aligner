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

#ifndef INTFBASEEXTERNAL_HPP
#define INTFBASEEXTERNAL_HPP

#include <QStringList>
#include <QHash>
#include <QFile>
#include <QTextStream>

class intfBaseExternalComponent
{
public:
    intfBaseExternalComponent(){}

protected:
    bool init(const QString& _type,
              const QString& _baseDir,
              const QString& _sourceLang,
              const QString& _targetLang){
        this->SourceLang = _sourceLang;
        this->TargetLang = _targetLang;
        this->BaseDir = _baseDir;
        this->CacheExtension = ".c" + _type;
        return true;
    }

    virtual void loadCache()
    {
        QFile File(this->BaseDir +this->SourceLang+"2"+this->TargetLang+ CacheExtension);
        if (File.open(QFile::ReadOnly))
        {
            QTextStream Dictionary(&File);
            Dictionary.setCodec("UTF-8");
            QStringList Words;

            while (!Dictionary.atEnd())
            {
                Words = Dictionary.readLine().trimmed().split(";:;");
                if (Words.size() == 2)
                    this->OldEntries.insertMulti(Words.first(), Words.last());
            }
        }
    }

    virtual void add2Cache(const QString& _flWord, const QString& _translation)
    {
        QFile File(this->BaseDir +this->SourceLang+"2"+this->TargetLang+"."+this->CacheExtension);
        if (File.open(QFile::Append))
        {
            QTextStream Dictionary(&File);
            Dictionary.setCodec("UTF-8");
            Dictionary<<_flWord << ";:;" << _translation<<endl;
        }
        this->OldEntries.insertMulti(_flWord, _translation);
    }

    QStringList checkCache(const QString& _word){
        return this->OldEntries.values(_word);
    }

protected:
    QString SourceLang;
    QString TargetLang;

private:
    QString BaseDir;
    QString CacheExtension;
    QHash<QString, QString> OldEntries;
};


struct stuExternalComponent{
    QString ShortName;
    QString FullName;
    intfBaseExternalComponent* Instance;
    bool Selectable;
};

#endif // INTFBASEEXTERNAL_HPP
