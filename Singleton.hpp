/* ==================================================
   File name: Singleton.hpp

   Purpose:	singleton 

   Copyright (c) 2013, daltomi <daltomi@disroot.org>

   This file is part of dmb.

    dmb is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    dmb is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with dmb.  If not, see <http://www.gnu.org/licenses/>.
  ==================================================
*/

#pragma once
#include <memory>

template<typename T>
class Singleton
{
    public:
        template<typename... Arguments>
        static T& Get(Arguments... argv) {
            static std::unique_ptr<T> instance = std::unique_ptr<T>( new T(argv...) );
            return *instance.get();
        }
 
    protected:
        Singleton() = delete;
        ~Singleton() = delete;
};

