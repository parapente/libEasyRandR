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
#include "crtc.h"

EasyRandR::Crtc::Crtc(EasyRandR::Screen* scr, RRCrtc crtc): m_screen(scr), m_id(crtc)
{
    m_info = NULL;
    updateInfo();
}

EasyRandR::Crtc::~Crtc()
{
    if (m_info)
	XRRFreeCrtcInfo(m_info);
}


bool EasyRandR::Crtc::isValid(void) const
{
    return m_valid;
}

void EasyRandR::Crtc::updateInfo(void)
{
    if (m_info)
	XRRFreeCrtcInfo(m_info);
    Display *dpy = XOpenDisplay(NULL);
    
    if (dpy)
	m_info = XRRGetCrtcInfo(dpy, m_screen->getResources(), m_id);
    else
	m_info = NULL;
    
    XCloseDisplay(dpy);
    
    if (m_info)
	m_valid = true;
    else {
	m_valid = false;
	qDebug() << "Error updating crtc info!";
    }
}

uint EasyRandR::Crtc::height(void) const
{
    if (m_info)
	return m_info->height;
    else
	return 0;
}

uint EasyRandR::Crtc::width(void) const
{
    if (m_info)
	return m_info->width;
    else
	return 0;
}

RRMode EasyRandR::Crtc::mode(void) const
{
    if (m_info)
	return m_info->mode;
    else
	return 0;
}

Rotation EasyRandR::Crtc::rotation(void) const
{
    if (m_info)
	return m_info->rotation;
    else
	return 0;
}

Rotation EasyRandR::Crtc::supportedRotations(void) const
{
    if (m_info)
	return m_info->rotations;
    else
	return 0;
}

QList< RROutput > EasyRandR::Crtc::connectedTo(void) const
{
    QList<RROutput> list;
    
    if (m_info)
	for (int i=0; i<m_info->noutput; i++)
	    list << m_info->outputs[i];

    return list;
}

Time EasyRandR::Crtc::timestamp(void) const
{
    if (m_info)
	return m_info->timestamp;
    else
	return 0;
}

uint EasyRandR::Crtc::x(void) const
{
    if (m_info)
	return m_info->x;
    else
	return 0;
}

uint EasyRandR::Crtc::y(void) const
{
    if (m_info)
	return m_info->y;
    else
	return 0;
}

int EasyRandR::Crtc::setCrtcConfig(int x, int y, RRMode mode, Rotation rotation, QList< RROutput > outputs)
{
    int ret;
    
    RROutput *outs = new RROutput[4];

    qDebug() << "Setting crtc:" << x << y << mode << rotation << outputs;
    for (int i=0; i<outputs.count(); i++)
	outs[i] = outputs.at(i);
    updateInfo();
    
    Display *dpy = XOpenDisplay(NULL);
    if (dpy) {
	if (mode == None) {// If we want to disable Crtc...
	    qDebug() << "XRRSetCrtcConfig(" << dpy << ',' << m_screen->getResources() << ',' << m_id << ',' << timestamp() << ',' << x << ',' << y << ',' << mode << ',' << rotation << ",NULL,0)\n";
	    ret = XRRSetCrtcConfig(dpy,m_screen->getResources(),m_id,timestamp(),x,y,mode,rotation,NULL,0);
	}
	else {
	    qDebug() << "XRRSetCrtcConfig(" << dpy << ',' << m_screen->getResources() << ',' << m_id << ',' << timestamp() << ',' << x << ',' << y << ',' << mode << ',' << rotation << ',' << outs << ',' << outputs.count() << ")\n";
	    ret = XRRSetCrtcConfig(dpy,m_screen->getResources(),m_id,timestamp(),x,y,mode,rotation,outs,outputs.count());
	}
    }
    else
	ret = -1; // TODO: Check validity of the return value
    
    XCloseDisplay(dpy);
    delete outs;
    return ret;
}

QList< RROutput > EasyRandR::Crtc::possibleOutputs(void) const
{
    QList<RROutput> list;
    
    if (m_info)
	for (int i=0; i<m_info->npossible; i++)
	    list << m_info->possible[i];
    
    return list;
}
