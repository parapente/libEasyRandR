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

#include <QDebug>
#include <QRect>
#include <X11/Xlib.h>
#include "configuration.h"

EasyRandR::Configuration::Configuration(QObject* parent): QObject(parent)
{
    m_valid = false;
    Display *display = XOpenDisplay(NULL);
    
    // If we can open a connection to the X server
    if (display) {
	// Use the default error handler
	XSetErrorHandler(NULL);
	// Check if we can use the RandR Extension
	m_valid = XRRQueryExtension(display, &m_eventBase, &m_errorBase);
    }
    
    if (m_valid) {
	// Get version supported by the XServer
	XRRQueryVersion(display, &m_ver_major, &m_ver_minor);
	qDebug() << "RandR version " << m_ver_major << "." << m_ver_minor << " supported";
	int count;
	
	count = ScreenCount(display);
	for (int i=0; i<count; i++) {
	    QList<EasyRandR::Output *> m;
	    
	    m_window << RootWindow(display,i);

	    m_screens[i] = new EasyRandR::Screen(RootWindow(display,i),i);
	    
	    QList<RROutput> list = m_screens.value(i)->getOutputs();
	    for (int j=0; j<list.count(); j++)
		m.append(new EasyRandR::Output(m_window[i], list.at(j), m_screens.value(i)));
	    
	    m_outputs.append(m);
	}
	XCloseDisplay(display);
    }
}

EasyRandR::Configuration::~Configuration()
{
}

QMap<RROutput,EasyRandR::Output*> EasyRandR::Configuration::getOutputs(int screen) const
{
    QMap<RROutput,EasyRandR::Output*> m;
    
    if (m_screens.value(screen)) {
	QList<RROutput> list = m_screens.value(screen)->getOutputs();
	for (int i=0; i<list.count(); i++)
	    m[list.at(i)] = m_outputs[screen].at(i);
    }

    return m;
}

QList< EasyRandR::Output* > EasyRandR::Configuration::getOutputList(int screen) const
{
    QList<EasyRandR::Output*> l;
    if (m_screens.value(screen)) {
	QList<RROutput> list = m_screens.value(screen)->getOutputs();
	for (int i=0; i<list.count(); i++)
	    l << m_outputs[screen].at(i);
    }

    return l;
}

int EasyRandR::Configuration::getScreenCount(void) const
{
    Display *dpy = XOpenDisplay(NULL);
    
    if (dpy)
	return ScreenCount(dpy);
    else
	return 0;
}

QMap< int, EasyRandR::Screen* > EasyRandR::Configuration::getScreens(void) const
{
    return m_screens;
}

QList< EasyRandR::Screen* > EasyRandR::Configuration::getScreenList(void) const
{
    return m_screens.values();
}

int EasyRandR::Configuration::applyConfiguration(void)
{
    for (int i=0; i<m_outputs.count(); i++) { // Screens
	for (int j=0; j<m_outputs[i].count(); j++) // Outputs
	    m_outputs[i].at(j)->off();
	updateScreenSize(i, m_outputs[i]);
	for (int j=0; j<m_outputs[i].count(); j++) {// Outputs
	    m_outputs[i].at(j)->applySettings();
	}
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
	    
	    modeList = m_screens.value(screen)->getModes();
	    for (int j=0; j<modeList.count(); j++) {
		qDebug() << "Compare" << modeList.at(j).id << "with" << newmode;
		if (modeList.at(j).id == newmode) {
		    qDebug() << "Success!";
		    modewidth = modeList.at(j).width;
		    modeheight = modeList.at(j).height;
		}
	    }
	}
	else {
	    modewidth = outputList.at(i)->width();
	    modeheight = outputList.at(i)->height();
	}

	qDebug() << (int) modewidth << 'x' << (int) modeheight << '+' << outputList.at(i)->newx() << '+' << outputList.at(i)->newy();
	QRect r(0, 0, outputList.at(i)->newx()+modewidth, outputList.at(i)->newy()+modeheight);
	if ((outputList.at(i)->newRotation() & RR_Rotate_90) || (outputList.at(i)->newRotation() & RR_Rotate_270)) {
	    // The crtc has to be rotated so the height becomes width and the width becomes height
	    int tmp = r.width();
	    r.setWidth(r.height());
	    r.setHeight(tmp);
	}
	screenRect = screenRect.united(r);
	qDebug() << "ScreenRect size" << screenRect.width() << 'x' << screenRect.height();
    }
    
    // TODO: Look here also when checking for panning
    qDebug() << "Current Screen -->" << m_screens.value(screen)->getSize().width() << 'x' << m_screens.value(screen)->getSize().height();
    if (m_screens.value(screen)->getSize() != screenRect.size()) {
	qDebug() << "Changing screen size:";
	if (m_screens.value(screen)->setSize(screenRect.size()))
	    qDebug() << "Success!";
	else
	    qDebug() << "Failed!";
    }
}
