/* ==================================================
   File name: SndFile.hpp

   Purpose:	frontend sndfile library.

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

#include <sndfile.h>
#include <memory>
#include <iostream>

class CSndFile
{
    public:

        explicit CSndFile();
        ~CSndFile(); 
        bool Open(const char*);
        const int getChannels() const;
        const int getSampleRate() const;
        const long long getDuration() const; 
        const int getFrames() const;
        const long long getPosition() const;
        const char * getErrorText() const;
        void Close(); 
        int Read(float*, sf_count_t);
        void Seek(sf_count_t);

    protected:

        SNDFILE*    snd_file;
        SF_INFO     snd_info;
        unsigned long long position;
};

using CSndFilePtr = std::unique_ptr<CSndFile>;

