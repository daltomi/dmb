/* ==================================================
   File name: SndFile.cxx

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
#include "SndFile.hpp"


CSndFile::CSndFile() : snd_file(NULL), 
                       position(0)
{ }


CSndFile::~CSndFile() 
{ 
#ifdef DEBUG
    std::cerr << "CSndFile destroy" << std::endl;
#endif
    Close(); 
}


bool CSndFile::Open(const char* file_name)
{
    snd_file = sf_open(file_name, SFM_READ, &snd_info);
    return ( NULL == snd_file ) ? false : true;
}


const int CSndFile::getChannels() const 
{
    return snd_info.channels;
}


const int CSndFile::getSampleRate() const
{
    return snd_info.samplerate;
}


const long long CSndFile::getDuration() const
{
    return getFrames() / getSampleRate();
}


const int CSndFile::getFrames() const
{
    return snd_info.frames;
}


const long long CSndFile::getPosition() const
{
    return (position / getSampleRate());
}


void CSndFile::Close() 
{
    if ( snd_file != NULL )
    {
        sf_close(snd_file);
    }
}


const char* CSndFile::getErrorText() const
{
    return sf_strerror(snd_file);
}


int CSndFile::Read(float* out, sf_count_t frames_per_buffer)
{
    position += frames_per_buffer;
    return sf_readf_float(snd_file, out, frames_per_buffer );
}


void CSndFile::Seek(sf_count_t count)
{
    if ( static_cast<unsigned>(count) != position )
    {
        const unsigned long long newpos = count * getSampleRate();
        position = sf_seek(snd_file, newpos, SEEK_SET);
    }
}

