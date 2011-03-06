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
	window = XDefaultRootWindow(display);
	
	// Check if we can use the RandR Extension
	valid = XRRQueryExtension(display, &eventBase, &errorBase);
    }
    
    if (valid) {
	// Get version supported by the XServer
	XRRQueryVersion(display, &ver_major, &ver_minor);
	screen = new EasyRandR::Screen(display, window);
	
	QList<RROutput> outs;
	outs = screen->getOutputs();
	for (int i=0; i<outs.count(); i++) {
	    outputs.insert(outs[i], new Output(display, window, outs[i], screen->getResources()));
	}
    }
}

EasyRandR::Configuration::~Configuration()
{
    XCloseDisplay(display);
}


// Returns a list with the ids of all outputs available
QList< RROutput > EasyRandR::Configuration::getOutputList()
{
    return screen->getOutputs();
}

// Return a pointer to the output with id 'output'. If an output with such id
// don't exist, it will return a 0
EasyRandR::Output* EasyRandR::Configuration::getOutput(RROutput output)
{
    return outputs.value(output);
}
