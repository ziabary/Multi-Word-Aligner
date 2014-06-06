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

#ifndef EXMWABASE_HPP
#define EXMWABASE_HPP

#include <QString>
#include <Common.h>
#include <stdexcept>

class exMWABase: public std::exception
{
  public:
    /**
     * @brief Base Exception constructor.
     *
     * @param _message Message to be shown when calling what()
     * @param _line Line Number where the exception occured Defaults to 0.
     **/
    exMWABase(const QString& _message = "", quint32 _line = 0) throw (){
        this->Message = _line ? QString::number(_line) + ": " + _message : _message;
    }

    /**
     * @brief A method to show Exception message
     *
     * @return QString Exception message
     **/
    QString what(){
        return this->Message;
    }

  protected:
    QString Message;
};

#define MWA_ADD_EXCEPTION_HANDLER(_name,_base) \
    class _name : public _base{\
    public: _name (const QString& _message = "", int _line = 0) : \
            _base (_message, _line){ \
    this->Message.append(">;"#_name);\
    }};

#endif // EXMWABASE_HPP
