/* ==================================================
   File name: PortAudio.cxx

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
#include "PortAudio.hpp"

CPortAudio::CPortAudio() : 
                        paStream(NULL), 
                        bPause(false)
{
    Initialize();
}


CPortAudio::~CPortAudio()
{ 
#ifdef DEBUG
    std::cout << "CPortAudio destroy" << std::endl;
#endif

    Stop();
    CloseDevice();
    Terminate();
}


void CPortAudio::Initialize()
{
    err = Pa_Initialize();

    checkError();
}


void CPortAudio::Terminate()
{
    err = Pa_Terminate();

    checkError();
}


bool CPortAudio::Play()
{
    if ( NULL != paStream )
    {
        err = Pa_StartStream(paStream);
    }
    /*
     * Como checkError retorna TRUE en 
     * caso de error, hacemos un
     * NOT para que se adapte al
     * nombre de esta func.
     * */
    return !checkError();
}


void CPortAudio::CloseDevice() const
{
    Pa_CloseStream(paStream);
}


/*
 * Espera a que termine el stream.
 * Tambien espera si el estado de
 * pausa esta activo.
 * */
void CPortAudio::Wait() const
{
    do{
        Pa_Sleep(50);
    }while ( bPause == true || isActive() == 1 );
}


bool CPortAudio::isActive() const
{
   if ( NULL != paStream)
   {
       return Pa_IsStreamActive(paStream);
   }

   return false;
}


bool CPortAudio::isStopped() const
{
   return Pa_IsStreamStopped(paStream);
}

bool CPortAudio::isPaused() const
{
    return bPause;
}

void CPortAudio::Stop()
{
    if ( NULL != paStream && !isStopped() )
    {
        err = Pa_StopStream(paStream);

        checkError();
    }
}

void CPortAudio::StopFast()
{
    if ( NULL != paStream && !isStopped() )
    {
        err = Pa_AbortStream(paStream);

        checkError();
    }
}

bool CPortAudio::togglePause()
{
    if ( !bPause )
    {
        Stop();
    }
    else
    {
        Play();
    }
    return ( (bPause = !bPause) );
}


bool CPortAudio::OpenDevice(CSndFilePtr& snd_file,
                            PaStreamCallback* output_cb, 
                            PaStreamFinishedCallback* end_cb,
                            void* data
                            )
{
    memset(&outputParam, 0, sizeof(PaStreamParameters) );
    outputParam.device = Pa_GetDefaultOutputDevice();
    outputParam.channelCount = snd_file->getChannels();
    outputParam.sampleFormat = paFloat32;
    outputParam.hostApiSpecificStreamInfo = 0;
    outputParam.suggestedLatency = 0.5f;
    
    err = Pa_OpenStream(&paStream,
                       0,
                       &outputParam,
                       snd_file->getSampleRate(),
                       paFramesPerBufferUnspecified,
                       paNoFlag,
                       output_cb,
                       data);

    if( false == checkError()  && end_cb != NULL )
    {
        err = Pa_SetStreamFinishedCallback(paStream, end_cb);
    }

#ifdef DEBUG
    std::cout << "CPortAudio::OpenDevice, suggested latency: " << outputParam.suggestedLatency;
    std::cout << ", output latency: " << Pa_GetStreamInfo(paStream)->outputLatency << std::endl;
#endif

    /*
     * Como checkError retorna TRUE en 
     * caso de error, hacemos un
     * NOT para que se adapte al
     * nombre de esta func.
     * */
    return !checkError();
}


/*
 * Si existe un error retorna TRUE
 * */
bool CPortAudio::checkError()
{
    if ( err )
    {
        std::cerr << "Error number: " << err << std::endl;
        std::cerr << getErrorText() << std::endl;
        return true;
    }

    return false;

}


const char* CPortAudio::getErrorText() const
{
    return Pa_GetErrorText(err);
}
