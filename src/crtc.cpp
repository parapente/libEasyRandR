/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) 2011  Θεόφιλος Ιντζόγλου <int.teo@gmail.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#include "crtc.h"

EasyRandR::Crtc::Crtc(Display* dpy, EasyRandR::Screen* scr, RRCrtc crtc): display(dpy),
								screen(scr),
								id(crtc)
{
    info = NULL;
    updateInfo();
}

EasyRandR::Crtc::~Crtc()
{
    if (info)
	XRRFreeCrtcInfo(info);
}


bool EasyRandR::Crtc::isValid(void)
{
    return valid;
}

void EasyRandR::Crtc::updateInfo(void)
{
    if (info)
	XRRFreeCrtcInfo(info);
    info = XRRGetCrtcInfo(display, screen->getResources(), id);
    if (info)
	valid = true;
    else
	valid = false;
}

uint EasyRandR::Crtc::height(void )
{
    if (info)
	return info->height;
    else
	return 0;
}

uint EasyRandR::Crtc::width(void )
{
    if (info)
	return info->width;
    else
	return 0;
}

RRMode EasyRandR::Crtc::mode(void )
{
    if (info)
	return info->mode;
    else
	return 0;
}

Rotation EasyRandR::Crtc::rotation(void )
{
    if (info)
	return info->rotation;
    else
	return 0;
}

Rotation EasyRandR::Crtc::supportedRotations(void )
{
    if (info)
	return info->rotations;
    else
	return 0;
}

QList< RROutput > EasyRandR::Crtc::connectedTo(void )
{
    QList<RROutput> list;
    
    if (info)
	for (int i=0; i<info->noutput; i++)
	    list << info->outputs[i];

    return list;
}

Time EasyRandR::Crtc::timestamp(void )
{
    if (info)
	return info->timestamp;
    else
	return 0;
}

uint EasyRandR::Crtc::x(void )
{
    if (info)
	return info->x;
    else
	return 0;
}

uint EasyRandR::Crtc::y(void )
{
    if (info)
	return info->y;
    else
	return 0;
}

int EasyRandR::Crtc::setCrtcConfig(int x, int y, RRMode mode, Rotation rotation, QList< RROutput > outputs )
{
    int ret;
    
    RROutput *outs = new RROutput[4];
    
    for (int i=0; i<outputs.count(); i++)
	outs[i] = outputs.at(i);
    updateInfo();
    ret = XRRSetCrtcConfig(display,screen->getResources(),id,timestamp(),x,y,mode,rotation,outs,outputs.count());
    
    delete outs;
    return ret;
}

QList< RROutput > EasyRandR::Crtc::possibleOutputs ( void )
{
    QList<RROutput> list;
    
    if (info)
	for (int i=0; i<info->npossible; i++)
	    list << info->possible[i];
    
    return list;
}
