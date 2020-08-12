/* ==================================================
   File name: dmb.cxx

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

#include "dmb.hpp"


/*
 * - Actualiza el widget Fl_Box labelFiles.
 * */
void updateLabelFiles(unsigned int current)
{
    std::ostringstream ss;
    
    if ( ST::FOWARD == stJump.get() )
    {
        ss << "> " << current <<  " / " << playlist.size << " >" << '\n';
    }
    else
    {
        ss << "< " << current <<  " / " << playlist.size << " <" << '\n';
    }
    uiInst::Get().lbFiles->copy_label(ss.str().c_str() );
}


/*
 * - Progress bar tooltip.
 * - Duración del archivo en hms.
 * */
void updateHMS(const long duration)
{
    static std::string tooltip = "00:00:00";

    std::ostringstream ss;

    const unsigned int second = duration % 60;
    const unsigned int minute = (duration / 60) % 60;
    const unsigned int hour   = (duration / 60) / 60;

    ( hour < 10 ) ? ss << "0" << hour << ":" : ss << hour << ":";
    ( minute < 10 ) ? ss << "0" << minute << ":" : ss << minute << ":";
    ( second < 10 ) ? ss << "0" << second : ss << second;
    
    tooltip = "Duration: " + ss.str();

    setTooltip(uiInst::Get().progressBar, tooltip.c_str() );
}


void center_on_screen()
{
    int x, y, w, h;
    Fl::screen_xywh(x, y, w, h);
    uiInst::Get().position( w / 2, h / 2);
}


/*
 * - Valores por defectos al iniciar
 *   el programa.
 * */
void setupTooltip()
{
    createTooltip();

    setTooltip(uiInst::Get().btnQuit, "Salir");

    const char* tooltip = playlist.files[playlist.current];
    setTooltip(uiInst::Get().btnPlayPause, fl_filename_name(tooltip) );
    
    if ( playlist.size > 2 )
    {
        tooltip = playlist.files[playlist.current + 1];
        setTooltip(uiInst::Get().btnFoward, fl_filename_name(tooltip) );

        tooltip = playlist.files[playlist.size];
        setTooltip(uiInst::Get().btnBackward, fl_filename_name(tooltip) );
    }
    else
    {
        setTooltip(uiInst::Get().btnBackward, "<ninguno>");
        setTooltip(uiInst::Get().btnFoward, "<ninguno>"); 
    }
};



/*
 * - Cada vez que que se reproduce otro archivo.
 * */
void updateTooltip()
{
    // No es se actualiza si existe un solo archivo. 
    if ( 1 == playlist.size )
    {
        return;
    }

    const char* tooltip;

    tooltip = playlist.files[playlist.current];
    setTooltip(uiInst::Get().btnPlayPause, fl_filename_name(tooltip) );

    if ( NotSizeEqualOneOrPaused() )
    {
        if ( playlist.current + 1 > playlist.size )
        {
            tooltip = playlist.files[1];
        }
        else
        {
            tooltip = playlist.files[playlist.current  + 1];
        }

        setTooltip(uiInst::Get().btnFoward, fl_filename_name(tooltip) );

        if ( playlist.current - 1 == 0 )
        {
            tooltip = playlist.files[playlist.size];
        }
        else
        {
            tooltip = playlist.files[playlist.current - 1];
        }

        setTooltip(uiInst::Get().btnBackward, fl_filename_name(tooltip) );
    }
}




/*
 * - Se llama cada vez que PortAudio necesita más datos.
 * */
int output_cb(const void* input, 
              void* output,
              unsigned long frames_per_buffer,
              const PaStreamCallbackTimeInfo *time_info,
              PaStreamCallbackFlags flags, void*)
{
    if ( 0 != snd_file->Read((float*)output, frames_per_buffer) )
    {
        progress_value = snd_file->getPosition();

        return paContinue;
    }
   
    status.set(ST::ENDSONG);

    progress_value = 0;
    
    return paComplete;
}



/*
 * - Se actualiza la barra de progreso
 *   fuera de output_cb().
 * */
void updateProgressBar(void*)
{
    if ( status.get() != ST::SEEK_LOCK )
    {
        uiInst::Get().progressBar->value(progress_value);
    }

    Fl::repeat_timeout(0.100, updateProgressBar);
}
    


/*
 * - Intenta reproducir un archivo.
 * */
bool PlayNextFile()
{
    const unsigned int current = playlist.current;

    status.set(ST::READING);

    snd_file = CSndFilePtr(new CSndFile() );

    if ( false == snd_file->Open(playlist.files[current] ) )
    {
        std::cout << "No se pudo abrir el archivo de audio ";
        std::cout << playlist.files[current] << std::endl;
        std::cout << snd_file->getErrorText() << std::endl;
       
        return false;
    }
    

    if ( false == PortAudioInst::Get().OpenDevice(snd_file, 
                                                   output_cb, 
                                                   NULL, NULL
                                                  ) 
       )
    {

        return false;
    }

    uiInst::Get().progressBar->maximum( snd_file->getDuration() );

    updateHMS(snd_file->getDuration() );

    PortAudioInst::Get().Play();

    return true;
}




/*
 * - Se encarga de reproducir todos los archivos
 *   de la playlist.
 * */
void PlayThread()
{
    while( playlist.current <= playlist.size )
    {
        updateLabelFiles( playlist.current );
        
        if ( true == PlayNextFile() )
        {
            status.set(ST::PLAYING);

            PortAudioInst::Get().Wait();
          
            PortAudioInst::Get().CloseDevice();
        }
        else
        {
            // Si falló el único archivo, termina.
            if ( playlist.size == 1 )
            {
                exit(-1);
            }

            continuePlaylist();
        }
        
        /*
         * - En este momento se llega por alguna de
         *   las siguientes situaciones:
         * a) el archivo de audio llegó a su fin,
         * b) el usuario presionó alguno de los 
         *    botones foward/backward.
         * */

        // - Se procede en caso (a)
        if ( ST::ENDSONG == status.get() )
        {
            continuePlaylist();
        }

        updateTooltip();
    };

    exit(0);
}



/*
 * - Determina la dirección de reproducción
 *   en la playlist.
 * */
void continuePlaylist()
{
    if ( ST::FOWARD == stJump.get() )
    {
        playlist.foward();
    }
    else
    {
        playlist.backward();
    }
}


const bool NotSizeEqualOneOrPaused()
{
    return  !( (1 == playlist.size) || PortAudioInst::Get().isPaused() );
}





/*
 * Extern.
 * Ver uidmb.hpp, archivo de Fluid
 * */
void onCmdAction(Fl_Button* button, long id)
{
    switch(id)
    {
        case PLAYPAUSE:
            {
                if ( status.get() == ST::READING )
                {
                    break;
                }

                const bool pause = PortAudioInst::Get().togglePause();

                if ( pause ) 
                {
                    status.set(ST::PAUSE);
                    uiInst::Get().btnPlayPause->label("@>");
                }
                else
                {
                    status.set(ST::PLAYING);
                    uiInst::Get().btnPlayPause->label("@||");  
                }
                break;
            }
        case FOWARD:
            {
                if ( status.get() == ST::READING )
                {
                    break;
                }

                if ( NotSizeEqualOneOrPaused() )
                {
                    stJump.set(ST::FOWARD);
                    playlist.foward();
                    PortAudioInst::Get().StopFast();
                }

                break;
            }
        case BACKWARD:
            {
                if ( status.get() == ST::READING )
                {
                    break;
                }

                if ( NotSizeEqualOneOrPaused() )
                {
                    stJump.set(ST::BACKWARD);
                    playlist.backward();
                    PortAudioInst::Get().StopFast();
                }
                break;
            }

        case QUIT:
            PortAudioInst::Get().StopFast();
            exit(0);
            break;

        default:
            std::cout << "Evento de control desconocido" << std::endl;
    };
}




void onCmdSlider(Fl_Slider* slider, void*)
{
    if ( PortAudioInst::Get().isPaused() )
    {
        return ;
    }
            
    PortAudioInst::Get().togglePause();
    
    snd_file->Seek( slider->value() );
    
    PortAudioInst::Get().togglePause();

    status.set(ST::SEEK_UNLOCK);
}


/*
 * - Extern.
 * - Virtual Fl_Double_Window class.
 * - Permite mover la ventana con el mouse.  
 * */
int uidmb::handle(int evt)
{
    static const int cw = w() / 2;
    static const int ch = h() / 2;

    if ( Fl::event_state(FL_BUTTON1) )
    {
        switch(evt)
        {
            case FL_PUSH:
                status.set(ST::SEEK_LOCK);
                break;
            case FL_DRAG:
            {
                const int x = this->x() + (Fl::event_x() - cw);
                const int y = this->y() + (Fl::event_y() - ch);
                this->position(x, y);

                // - Para contrarrestar FL_PUSH.
                status.set(ST::SEEK_UNLOCK);

                return 1;
            }
        }
    }

    return Fl_Double_Window::handle(evt);
}





/* MAIN */
int main(int argc, char* argv[])
{
    if ( argc == 1 )
    {
        std::cout << cErrParam << std::endl;
        exit(-1);
    }

    playlist.size = argc - 1;
    playlist.files = (const char**)(argv);

    stJump.set(ST::FOWARD);

    std::thread th(PlayThread);
    th.detach();

    center_on_screen();

    setupTooltip();

    uiInst::Get().show();

    Fl::add_timeout(1.0, updateProgressBar); 

    return Fl::run();
}


