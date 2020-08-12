/* ==================================================
   File name: Tooltips.cxx

   Purpose:	Implementa el control tooltip

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

#include "Tooltips.hpp"

void createTooltip()
{
    Fl_Tooltip* tooltip = new Fl_Tooltip;
    tooltip->size(10);
    tooltip->delay(0.5);
    tooltip->color(218);
    tooltip->textcolor(94);
}


void setTooltip(Fl_Widget* btn, const char* tip)
{
    btn->tooltip(tip);
}



