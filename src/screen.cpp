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
#include <QDebug>
#include "screen.h"
#include "easycfg.h"

EasyRandR::Screen::Screen(Window w, int scid): m_window(w), m_id(scid)
{
    m_resources = NULL;
    updateResources();
    
    m_info = NULL;
    updateInfo();
    QTextStream err(stderr);
    
    Display *dpy = XOpenDisplay(NULL);
    
    if ((dpy) && (XRRGetScreenSizeRange(dpy, m_window, &m_minWidth, &m_minHeight, &m_maxWidth, &m_maxHeight) != 1))
	err << "Error while getting the size of screen!\n";
    XCloseDisplay(dpy);
}

EasyRandR::Screen::~Screen()
{
    if (m_resources)
	XRRFreeScreenResources(m_resources);
    if (m_info)
	XRRFreeScreenConfigInfo(m_info);
}

Time EasyRandR::Screen::configTimestamp(void)
{
    if (m_resources)
	return m_resources->configTimestamp;
    else
	return 0;
}

/* Returns the available outputs for this screen */
QList< RROutput > EasyRandR::Screen::getOutputs(void)
{
    QList<RROutput> outs;
    
    if (m_resources)
	for (int i=0; i<m_resources->noutput; i++) {
	    outs.append(m_resources->outputs[i]);
	}
    
    return outs;
}

int EasyRandR::Screen::getOutputCount(void )
{
    if (m_resources)
	return m_resources->noutput;
    else
	return 0;
}

XRRScreenResources* EasyRandR::Screen::getResources(void )
{
    return m_resources;
}

bool EasyRandR::Screen::isInfoValid(void )
{
    return m_infoValid;
}

void EasyRandR::Screen::updateInfo(void )
{
    if (m_info)
	XRRFreeScreenConfigInfo(m_info);
    Display *dpy = XOpenDisplay(NULL);
    
    if (dpy) {
	m_info = XRRGetScreenInfo(dpy,m_window);
	XCloseDisplay(dpy);
    }
    else
	m_info = NULL;
    
    if (m_info)
	m_infoValid = true;
    else {
	m_infoValid = false;
	qDebug() << "Error updating screen info!";
    }
}

bool EasyRandR::Screen::isResValid(void )
{
    return m_resValid;
}

void EasyRandR::Screen::updateResources(void )
{
    if (m_resources)
	XRRFreeScreenResources(m_resources);
    Display *dpy = XOpenDisplay(NULL);
    if (dpy) {
#ifdef XRANDR_1_3_FOUND
	m_resources = XRRGetScreenResourcesCurrent(dpy,m_window);
#else
	resources = XRRGetScreenResources(dpy,window);
#endif
	XCloseDisplay(dpy);
    }
    else
	m_resources = NULL;
    
    if (m_resources)
	m_resValid = true;
    else {
	m_resValid = false;
	qDebug() << "Cannot update screen resources!";
    }
}

Time EasyRandR::Screen::timestamp(void )
{
    if (m_resources)
	return m_resources->timestamp;
    else
	return 0;
}

QList< RRCrtc > EasyRandR::Screen::getCrtcs(void )
{
    QList<RRCrtc> list;
    
    if (m_resources)
	for (int i=0; i<m_resources->ncrtc; i++)
	    list << m_resources->crtcs[i];

    return list;
}

QList< XRRModeInfo > EasyRandR::Screen::getModes(void )
{
    QList<XRRModeInfo> list;
    
    if (m_resources)
	for (int i=0; i<m_resources->nmode; i++)
	    list << m_resources->modes[i];

    return list;
}

bool EasyRandR::Screen::setSize(QSize s)
{
    if ((s.width() < m_minWidth) || (s.width() > m_maxWidth) ||
	(s.height() < m_minHeight) || (s.height() > m_maxHeight))
	return false;
    else {
	Time tmp = m_resources->configTimestamp;
	Display *dpy = XOpenDisplay(NULL);
	if (dpy) {
	    qDebug() << "XRRSetScreenSize(" << dpy << ',' << m_window << ',' << s.width() << ',' << s.height() << ',' << (int)((float)s.width())/96*25.4 << ',' << (int)((float)s.height())/96*25.4 << ")\n";
	    XRRSetScreenSize(dpy,m_window,s.width(),s.height(),(int)((float)s.width())/96*25.4,(int)((float)s.height())/96*25.4);
	    XCloseDisplay(dpy);
	    updateResources();
	    updateInfo();
	    qDebug() << "::setSize::" << s.width() << 'x' << s.height();
	    if (m_resources->configTimestamp == tmp) // If no change took place
		return false;
	    else
		return true;
	}
	else
	    return false;
    }
}

QSize EasyRandR::Screen::getSize ( void )
{
    QSize s;
    Display *dpy = XOpenDisplay(NULL);
    
    if (dpy) {
	s.setWidth(XDisplayWidth(dpy,m_id));
	s.setHeight(XDisplayHeight(dpy,m_id));
	XCloseDisplay(dpy);
	return s;
    }
    else
	return QSize();
}

uint EasyRandR::Screen::minHeight(void )
{
    return m_minHeight;
}

uint EasyRandR::Screen::maxHeight(void )
{
    return m_maxHeight;
}

uint EasyRandR::Screen::minWidth(void )
{
    return m_minWidth;
}

uint EasyRandR::Screen::maxWidth(void )
{
    return m_maxWidth;
}
