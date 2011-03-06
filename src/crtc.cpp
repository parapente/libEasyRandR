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

Crtc::Crtc(Display* dpy, XRRScreenResources* res, RRCrtc crtc): display(dpy),
								screenResources(res),
								id(crtc)
{
    info = NULL;
    updateInfo();
}

bool Crtc::isValid(void)
{
    return valid;
}

void Crtc::setScreenResources(XRRScreenResources* res)
{
    screenResources = res;
}

void Crtc::updateInfo(void)
{
    if (info)
	XRRFreeCrtcInfo(info);
    info = XRRGetCrtcInfo(display, screenResources, id);
    if (info)
	valid = true;
    else
	valid = false;
}

uint Crtc::height(void )
{
    return info->height;
}

uint Crtc::width(void )
{
    return info->width;
}

RRMode Crtc::mode(void )
{
    return info->mode;
}

Rotation Crtc::rotation(void )
{
    return info->rotation;
}

Rotation Crtc::supportedRotations(void )
{
    return info->rotations;
}

QList< RROutput > Crtc::connectedTo(void )
{
    QList<RROutput> list;
    for (int i=0; i<info->noutput; i++)
	list << info->outputs[i];
    return list;
}

Time Crtc::timestamp(void )
{
    return info->timestamp;
}

int Crtc::x(void )
{
    return info->x;
}

int Crtc::y(void )
{
    return info->y;
}
