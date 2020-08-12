/* ==================================================
   File name: PortAudio.hpp

   Purpose:	frontend portaudio library.

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
#include <portaudio.h>
#include <memory>
#include <cstring>
#include <iostream>
#include "Singleton.hpp"
#include "SndFile.hpp"

class CPortAudio
{
    public:

        explicit CPortAudio();
        ~CPortAudio();

        bool Play();
        void Stop();
        void StopFast();
        bool togglePause();
        bool isPaused() const;
        bool OpenDevice(CSndFilePtr&, 
                        PaStreamCallback*, 
                        PaStreamFinishedCallback*,
                        void*
                        );
        void CloseDevice() const;
        void Wait() const;
        bool isActive() const;
        bool isStopped() const;

    protected:
        
        PaError err;
        PaStream* paStream;
        PaStreamParameters outputParam;
        bool bPause;

        void Initialize();
        void Terminate();
        bool checkError();
        const char* getErrorText() const;
};

using CPortAudioPtr = std::unique_ptr<CPortAudio>;

using PortAudioInst = Singleton<CPortAudio>;

