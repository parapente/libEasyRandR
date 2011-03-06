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


#include "output.h"
#include "screen.h"

Output::Output(Display* dpy, Window w, int oid, XRRScreenResources *scrres): display(dpy),
									    window(w),
									    id(oid),
									    screenResources(scrres)
{
    info = NULL;
    updateInfo();
}

Output::~Output()
{
    if (info)
	XRRFreeOutputInfo(info);
}

void Output::setScreenResources(XRRScreenResources* scrres)
{
    screenResources = scrres;
}

void Output::updateInfo(void)
{
    if (info)
	XRRFreeOutputInfo(info);
    info = XRRGetOutputInfo(display,screenResources,id);
    if (info)
	valid = true;
    else
	valid = false;
}

bool Output::isValid(void )
{
    return valid;
}

RRCrtc Output::crtc(void )
{
    return info->crtc;
}

Time Output::timestamp(void )
{
    return info->timestamp;
}

QList< RRCrtc > Output::validCrtcs(void )
{
    QList<RRCrtc> list;
    
    for (int i=0; i<info->ncrtc; i++)
	list << info->crtcs[i];
    return list;
}

QList< RROutput > Output::clones(void )
{
    QList<RROutput> list;
    
    for (int i=0; i<info->nclone; i++)
	list << info->clones[i];
    return list;
}

QString Output::name(void )
{
    return QString(QByteArray(info->name,info->nameLen));
}

Connection Output::connectionStatus(void )
{
    return info->connection;
}

QList< RRMode > Output::validModes(void )
{
    QList<RRMode> list;
    
    for (int i=0; i<info->nmode; i++)
	list << info->modes[i];
    return list;
}

ulong Output::heightmm(void )
{
    return info->mm_height;
}

ulong Output::widthmm(void )
{
    return info->mm_width;
}

SubpixelOrder Output::subpixelOrder(void )
{
    return info->subpixel_order;
}

QList< RRMode > Output::preferedModes(void )
{
    QList<RRMode> list;
    
    for (int i=0; i<info->npreferred; i++)
	list << info->modes[i];
    return list;
}
