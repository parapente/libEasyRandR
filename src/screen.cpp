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

#include <QTextStream>
#include "screen.h"
#include "easycfg.h"

EasyRandR::Screen::Screen(Display* dpy, Window w): display(dpy), window(w)
{
    resources = NULL;
    updateResources();
    
    info = NULL;
    updateInfo();
    QTextStream err(stderr);
    
    if (XRRGetScreenSizeRange(display, window, &minWidth, &minHeight, &maxWidth, &maxHeight) != 1)
	err << "Error while getting the size of screen!\n";
}

EasyRandR::Screen::~Screen()
{
    if (resources)
	XRRFreeScreenResources(resources);
    if (info)
	XRRFreeScreenConfigInfo(info);
}

Time EasyRandR::Screen::configTimestamp(void)
{
    if (resources)
	return resources->configTimestamp;
    else
	return 0;
}

/* Returns the available outputs for this screen */
QList< RROutput > EasyRandR::Screen::getOutputs(void)
{
    QList<RROutput> outs;
    
    if (resources)
	for (int i=0; i<resources->noutput; i++) {
	    outs.append(resources->outputs[i]);
	}
    
    return outs;
}

XRRScreenResources* EasyRandR::Screen::getResources(void )
{
    return resources;
}

bool EasyRandR::Screen::isInfoValid(void )
{
    return infoValid;
}

void EasyRandR::Screen::updateInfo(void )
{
    if (info)
	XRRFreeScreenConfigInfo(info);
    info = XRRGetScreenInfo(display,window);
    if (info)
	infoValid = true;
    else
	infoValid = false;
}

bool EasyRandR::Screen::isResValid(void )
{
    return resValid;
}

void EasyRandR::Screen::updateResources(void )
{
    if (resources)
	XRRFreeScreenResources(resources);
#ifdef XRANDR_1_3_FOUND
    resources = XRRGetScreenResourcesCurrent(display,window);
#else
    resources = XRRGetScreenResources(display,window);
#endif
    if (resources)
	resValid = true;
    else
	resValid = false;
}

Time EasyRandR::Screen::timestamp(void )
{
    if (resources)
	return resources->timestamp;
    else
	return 0;
}

QList< RRCrtc > EasyRandR::Screen::getCrtcs(void )
{
    QList<RRCrtc> list;
    
    if (resources)
	for (int i=0; i<resources->ncrtc; i++)
	    list << resources->crtcs[i];

    return list;
}

QList< XRRModeInfo > EasyRandR::Screen::getModes(void )
{
    QList<XRRModeInfo> list;
    
    if (resources)
	for (int i=0; i<resources->nmode; i++)
	    list << resources->modes[i];

    return list;
}