// generated by Fast Light User Interface Designer (fluid) version 1.0302

#ifndef uidmb_hpp
#define uidmb_hpp
#include <FL/Fl.H>
/**
   fl_filename_name
*/
#include <FL/filename.H>
#include <memory>
#include "Singleton.hpp"
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Button.H>
extern void onCmdAction(Fl_Button*, long);
#include <FL/Fl_Box.H>
#include <FL/Fl_Slider.H>
extern void onCmdSlider(Fl_Slider*, void*);

class uidmb : public Fl_Double_Window {
  void _uidmb();
public:
  uidmb(int X, int Y, int W, int H, const char *L = 0);
  uidmb(int W, int H, const char *L = 0);
  uidmb();
  Fl_Button *btnPlayPause;
  Fl_Button *btnFoward;
  Fl_Button *btnBackward;
  Fl_Button *btnQuit;
  Fl_Box *lbFiles;
  Fl_Box *lbStatus;
  Fl_Slider *progressBar;
  int handle(int);;
};
using uiInst = Singleton<uidmb>;
enum { PLAYPAUSE=0, FOWARD, BACKWARD, QUIT}; 
#endif
