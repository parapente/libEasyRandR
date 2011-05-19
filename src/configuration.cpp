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
#include <QRect>

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
	    QList<EasyRandR::Output *> m;
	    
	    window << RootWindow(display,i);

	    screens[i] = new EasyRandR::Screen(display,RootWindow(display,i),i);
	    
	    QList<RROutput> list = screens.value(i)->getOutputs();
	    for (int j=0; j<list.count(); j++)
		m.append(new EasyRandR::Output(display,window[i],list.at(j),screens.value(i)));
	    
	    outputs.append(m);
	}
    }
}

EasyRandR::Configuration::~Configuration()
{
    XCloseDisplay(display);
}

QMap<RROutput,EasyRandR::Output*> EasyRandR::Configuration::getOutputs(int screen)
{
    QMap<RROutput,EasyRandR::Output*> m;
    
    if (screens.value(screen)) {
	QList<RROutput> list = screens.value(screen)->getOutputs();
	for (int i=0; i<list.count(); i++)
	    m[list.at(i)] = outputs[screen].at(i);
    }

    return m;
}

QList< EasyRandR::Output* > EasyRandR::Configuration::getOutputList(int screen)
{
    QList<EasyRandR::Output*> l;
    if (screens.value(screen)) {
	QList<RROutput> list = screens.value(screen)->getOutputs();
	for (int i=0; i<list.count(); i++)
	    l << outputs[screen].at(i);
    }

    return l;
}

int EasyRandR::Configuration::getScreenCount(void )
{
    return ScreenCount(display);
}

QMap< int, EasyRandR::Screen* > EasyRandR::Configuration::getScreens(void )
{
    return screens;
}

QList< EasyRandR::Screen* > EasyRandR::Configuration::getScreenList(void )
{
    return screens.values();
}

int EasyRandR::Configuration::applyConfiguration(void )
{
    for (int i=0; i<outputs.count(); i++) {
	updateScreenSize(i, outputs[i]);
	for (int j=0; j<outputs[i].count(); j++)
	    outputs[i].at(j)->applySettings();
    }
}

void EasyRandR::Configuration::updateScreenSize(int screen, QList< EasyRandR::Output* > outputList)
{
    // TODO: Check for panning
    QRect screenRect;
    
    for (int i=0; i<outputList.count(); i++) {
	// We must take all rects of all outputs of screen[i] to check how much
	// space is needed for the screen
	CARD16 modewidth, modeheight;
	
	if (outputList.at(i)->modeChanged()) {
	    RRMode newmode = outputList.at(i)->newMode();
	    
	    // We need to find the width and height of this mode
	    QList<XRRModeInfo> modeList;
	    
	    modeList = screens.value(screen)->getModes();
	    for (int j=0; j<modeList.count(); j++) {
		if (modeList.at(i).id == newmode) {
		    modewidth = modeList.at(i).width;
		    modeheight = modeList.at(i).height;
		}
	    }
	}
	else {
	    modewidth = outputList.at(i)->width();
	    modeheight = outputList.at(i)->height();
	}
	
	QRect r(outputList.at(i)->x(), outputList.at(i)->y(), modewidth, modeheight);
	screenRect = screenRect.united(r);
    }
    
    // TODO: Look here also when checking for panning
    if (screens.value(screen)->getSize() != screenRect.size())
	screens.value(screen)->setSize(screenRect.size());
}
