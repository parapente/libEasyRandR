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


#include "configuration.h"
#include <X11/Xlib.h>

EasyRandR::Configuration::Configuration(QObject* parent): QObject(parent)
{
    valid = false;
    display = XOpenDisplay(NULL);
    
    // If we can open a connection to the X server
    if (display) {
	// Check if we can use the RandR Extension
	valid = XRRQueryExtension(display, &eventBase, &errorBase);
    }
    
    if (valid) {
	// Get version supported by the XServer
	XRRQueryVersion(display, &ver_major, &ver_minor);
	int count;
	
	count = ScreenCount(display);
	for (int i=0; i<count; i++) {
	    window << RootWindow(display,i);

	    screens[i] = new EasyRandR::Screen(display,RootWindow(display,i),i);
	}
    }
}

EasyRandR::Configuration::~Configuration()
{
    XCloseDisplay(display);
}

QMap<RROutput,EasyRandR::Output*> EasyRandR::Configuration::getOutputs(int screen)
{
    if (screens.value(screen)) {
	QList<RROutput> list = screens.value(screen)->getOutputs();
	QMap<RROutput,EasyRandR::Output*> m;
	for (int i=0; i<list.count(); i++)
	    m[list.at(i)] = new EasyRandR::Output(display,window[screen],list.at(i),screens.value(screen));
	return m;
    }
    else
	return QMap<RROutput,EasyRandR::Output*>();
}

QList< EasyRandR::Output* > EasyRandR::Configuration::getOutputList(int screen)
{
    if (screens.value(screen)) {
	QList<RROutput> list = screens.value(screen)->getOutputs();
	QList<EasyRandR::Output*> l;
	for (int i=0; i<list.count(); i++)
	    l << new EasyRandR::Output(display,window[screen],list.at(i),screens.value(screen));
	return l;
    }
    else
	return QList<EasyRandR::Output*>();
}

int EasyRandR::Configuration::getScreenCount(void )
{
    return ScreenCount(display);
}
