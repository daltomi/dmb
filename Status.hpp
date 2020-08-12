/* ==================================================
   File name: Status.hpp

   Purpose:	inline class

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


enum class ST
{
    READING=0,
    PAUSE,      
    PLAYING,
    ENDSONG,    // output_cb() [ paComplete ]
    SEEK_LOCK,
    SEEK_UNLOCK,
    FOWARD,
    BACKWARD
};


class CStatus
{
    private:

        ST status;

        constexpr static const char* cStatus[3]= {
        /*READING   */    "Reading..",
        /*PAUSE     */    "Pause...",
        /*PLAYING   */    "Playing..."
        };

    public:


        explicit CStatus()
        {  }
        
        void set(const ST status) 
        {
            if ( status != this->status )
            {
                this->status = status;

                // Actualiza label se es un valor de cStatus
                if ( status <= ST::PLAYING )
                {
                    const unsigned int index = (unsigned int) status;
                    uiInst::Get().lbStatus->label(cStatus[index] );
                }
            }
        }

        const ST get() const
        {
            return status;
        }
};


