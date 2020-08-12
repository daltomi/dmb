/* ==================================================
   File name: dmb.hpp

   Purpose:	Main App

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

#include "uidmb.hpp"
#include "SndFile.hpp"
#include "PortAudio.hpp"
#include "Singleton.hpp"
#include "Tooltips.hpp"
#include "Status.hpp"
#include <memory>
#include <string>
#include <sstream>
#include <thread>


struct Playlist
{
    const char** files;
    unsigned long size;
    unsigned long current;

    Playlist() : current(1)// argv[1]
    { }

    void foward() 
    { 
        if ( ++current  > size)
        {
            current = 1;
        }
    }

    void backward() 
    {
        if ( --current  == 0 )
        {
            current = size;
        }
    }
};

Playlist playlist;

CStatus status;
CStatus stJump;

CSndFilePtr snd_file;

static long long progress_value = 0;



void updateLabelFiles(unsigned int);
void center_on_screen();
void setupTooltip();
void updateTooltip();
void updateProgressBar(void*);
int output_cb(const void*, 
              void*,
              unsigned long,
              const PaStreamCallbackTimeInfo *,
              PaStreamCallbackFlags, void*);

bool PlayNextFile();
void PlayThread();
bool validFoward();
bool validBackward();
void updateHMS();
void continuePlaylist();
const bool NotSizeEqualOneOrPaused();

static const char cErrParam[] = {
    "\nFaltan parámetros\n \
     Ej: dmb *.ogg\n \
     Ej: dmb song1.ogg song2.flac\n\n"
};

