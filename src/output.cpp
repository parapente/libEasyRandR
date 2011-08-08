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
#include <QPoint>
#include "output.h"

EasyRandR::Output::Output(Window w, RROutput oid, Screen *scr): m_window(w), m_outputId(oid), m_screen(scr)
{
    m_info = NULL;
    m_pcrtc = NULL;
    m_positionChanged = m_modeChanged =  m_rotationChanged = m_outputsChanged = false;
    m_newmode = m_newrotation = m_newx = m_newy = 0;
    updateInfo();
    if (m_info && m_screen->isResValid() && (m_info->crtc!=0)) {
	m_pcrtc = new Crtc(m_screen,m_info->crtc);
    }
}

EasyRandR::Output::~Output()
{
    if (m_info)
	XRRFreeOutputInfo(m_info);
}

void EasyRandR::Output::updateInfo(void)
{
    if (m_info)
	XRRFreeOutputInfo(m_info);
    Display *dpy = XOpenDisplay(NULL);
    
    if (dpy)
	m_info = XRRGetOutputInfo(dpy,m_screen->getResources(),m_outputId);
    else
	m_info = NULL;
    
    if (m_info)
	m_valid = true;
    else {
	qDebug() << "Can't get info!";
	m_valid = false;
    }
}

bool EasyRandR::Output::isValid(void) const
{
    return m_valid;
}

RRCrtc EasyRandR::Output::crtcId(void) const
{
    if (m_info)
	return m_info->crtc;
    else
	return 0;
}

Time EasyRandR::Output::timestamp(void) const
{
    if (m_info)
	return m_info->timestamp;
    else
	return 0;
}

QList< RRCrtc > EasyRandR::Output::validCrtcs(void) const
{
    QList<RRCrtc> list;
    
    if (m_info)
	for (int i=0; i<m_info->ncrtc; i++)
	    list << m_info->crtcs[i];

    return list;
}

QList< RROutput > EasyRandR::Output::clones(void) const
{
    QList<RROutput> list;

    if (m_info)
	for (int i=0; i<m_info->nclone; i++)
	    list << m_info->clones[i];

    return list;
}

QString EasyRandR::Output::name(void) const
{
    if (m_info)
	return QString::fromUtf8(m_info->name);
    else
	return QString();
}

Connection EasyRandR::Output::connectionStatus(void) const
{
    if (m_info)
	return m_info->connection;
    else
	return RR_UnknownConnection;
}

QMap<RRMode,QString> EasyRandR::Output::validModes(void) const
{
    QList<RRMode> list;
    QMap<RRMode,QString> map;
    
    if (m_info) {
	// Get the valid modes for the output
	for (int i=0; i<m_info->nmode; i++)
	    list << m_info->modes[i];
	
	// Get the valid modes for the screen
	QList<XRRModeInfo> mlist;
	mlist = m_screen->getModes();
	
	// Create the map of modes that contains keys in the form of RRMode,Modename
	for (int i=0; i<list.count(); i++) {
	    QString Modename;
	    for (int j=0; j<mlist.count(); j++)
		if (list[i] == mlist[j].id) {
		    // Calculate Refresh Rate
		    float rr = (float) mlist[j].dotClock / (mlist[j].hTotal*mlist[j].vTotal);
		    Modename = QString::fromUtf8(mlist[j].name) + "@" + QString::number(rr,'f',1);
		}
	    map.insert(list[i],Modename);
	}
    }

    return map;
}

ulong EasyRandR::Output::heightmm(void) const
{
    if (m_info)
	return m_info->mm_height;
    else
	return 0;
}

ulong EasyRandR::Output::widthmm(void) const
{
    if (m_info)
	return m_info->mm_width;
    else
	return 0;
}

SubpixelOrder EasyRandR::Output::subpixelOrder(void) const
{
    if (m_info)
	return m_info->subpixel_order;
    else
	return 0;
}

QList< RRMode > EasyRandR::Output::preferedModes(void) const
{
    QList<RRMode> list;
    
    if (m_info)
	for (int i=0; i<m_info->npreferred; i++)
	    list << m_info->modes[i];

    return list;
}

RRMode EasyRandR::Output::currentMode(void) const
{
    if (m_pcrtc)
	return m_pcrtc->mode();
    else
	return 0;
}

uint EasyRandR::Output::height(void) const
{
    if (m_pcrtc)
	return m_pcrtc->height();
    else
	return 0;
}

uint EasyRandR::Output::width(void) const
{
    if (m_pcrtc)
	return m_pcrtc->width();
    else
	return 0;
}

int EasyRandR::Output::x(void) const
{
    if (m_pcrtc)
	return m_pcrtc->x();
    else
	return 0;
}

int EasyRandR::Output::y(void) const
{
    if (m_pcrtc)
	return m_pcrtc->y();
    else
	return 0;
}

Rotation EasyRandR::Output::currentRotation(void) const
{
    if (m_pcrtc)
	return m_pcrtc->rotation();
    else
	return RR_Rotate_0;
}

Rotation EasyRandR::Output::validRotations(void) const
{
    if (m_pcrtc)
	return m_pcrtc->supportedRotations();
    else
	return 0;
}

void EasyRandR::Output::setPos ( uint x, uint y )
{
    m_newx = x;
    m_newy = y;
    m_positionChanged = true;
}

bool EasyRandR::Output::setMode ( RRMode mode )
{
    if (validModes().contains(mode)) {
	m_newmode = mode;
	m_modeChanged = true;
	return true;
    }
    else
	return false;
}

bool EasyRandR::Output::setRotation ( Rotation rotation )
{
    if ((rotation & validRotations()) == rotation) {
	m_newrotation = rotation;
	m_rotationChanged = true;
	return true;
    }
    else
	return false;
}

bool EasyRandR::Output::setOutputs ( QList< RROutput > outputs )
{
    bool ret = true;
    for (int i=0; i<outputs.count(); i++)
	if (m_pcrtc->possibleOutputs().indexOf(outputs[i]) == -1)
	    ret = false;

    if (ret) {
	m_newoutputs = outputs;
	m_outputsChanged = true;
    }
    
    return ret;
}

int EasyRandR::Output::applySettings(void )
{
    if (m_pcrtc) {
	if (!m_positionChanged) {
	    m_newx = m_pcrtc->x();
	    m_newy = m_pcrtc->y();
	}
	
	if (!m_modeChanged)
	    m_newmode = m_pcrtc->mode();
	
	if (!m_rotationChanged)
	    m_newrotation = m_pcrtc->rotation();
	
	if (!m_outputsChanged)
	    m_newoutputs = m_pcrtc->connectedTo();
	
	return m_pcrtc->setCrtcConfig(m_newx, m_newy, m_newmode, m_newrotation, m_newoutputs);
    }
    else
	return -1; // TODO: Find a more appropriate return value
}

RROutput EasyRandR::Output::id(void) const
{
    return m_outputId;
}

RRMode EasyRandR::Output::newMode(void) const
{
    if (m_modeChanged)
	return m_newmode;
    else
	return m_pcrtc->mode();
}

int EasyRandR::Output::newx(void) const
{
    if (m_positionChanged)
	return m_newx;
    else
	return m_pcrtc->x();
}

int EasyRandR::Output::newy(void) const
{
    if (m_positionChanged)
	return m_newy;
    else
	return m_pcrtc->y();
}

Rotation EasyRandR::Output::newRotation(void) const
{
    if (m_rotationChanged)
	return m_newrotation;
    else
	return m_pcrtc->rotation();
}

bool EasyRandR::Output::modeChanged(void) const
{
    return m_modeChanged;
}

bool EasyRandR::Output::outputsChanged(void) const
{
    return m_outputsChanged;
}

bool EasyRandR::Output::positionChanged(void) const
{
    return m_positionChanged;
}

bool EasyRandR::Output::rotationChanged(void) const
{
    return m_rotationChanged;
}

void EasyRandR::Output::off(void )
{
    if (m_pcrtc) {
	// Keep current settings as new to be able to recover
	if (!m_modeChanged) {
	    m_newmode = m_pcrtc->mode();
	    m_modeChanged = true;
	}
	if (!m_outputsChanged) {
	    m_newoutputs = m_pcrtc->connectedTo();
	    m_outputsChanged = true;
	}
	if (!m_positionChanged) {
	    m_newx = m_pcrtc->x();
	    m_newy = m_pcrtc->y();
	    m_positionChanged = true;
	}
	if (!m_rotationChanged) {
	    m_newrotation = m_pcrtc->rotation();
	    m_rotationChanged = true;
	}
	
	m_pcrtc->setCrtcConfig(m_pcrtc->x(), m_pcrtc->y(), None, m_pcrtc->rotation(), m_pcrtc->connectedTo());
    }
}

void EasyRandR::Output::on(void )
{
    // Settings are already saved as new so we just need to apply them
    applySettings();
}

uint EasyRandR::Output::maxX(void) const
{
    uint maxPos = 0;

    if (m_info) {
	maxPos = m_screen->maxWidth();
	
	// Get the valid modes for the screen
	QList<XRRModeInfo> mlist;
	mlist = m_screen->getModes();
	
	// Create the map of modes that contains keys in the form of RRMode,Modename
	for (int i=0; i<mlist.count(); i++) {
	    if (mlist[i].id == newMode())
		maxPos -= mlist[i].width;
	}    
    }

    return maxPos;
}

uint EasyRandR::Output::maxY(void) const
{
    uint maxPos = 0;

    if (m_info) {
	maxPos = m_screen->maxHeight();
	
	// Get the valid modes for the screen
	QList<XRRModeInfo> mlist;
	mlist = m_screen->getModes();
	
	// Create the map of modes that contains keys in the form of RRMode,Modename
	for (int i=0; i<mlist.count(); i++) {
	    if (mlist[i].id == newMode())
		maxPos -= mlist[i].height;
	}    
    }

    return maxPos;
}

EasyRandR::Screen* const EasyRandR::Output::screen(void) const
{
    return m_screen;
}

EasyRandR::Crtc* const EasyRandR::Output::crtc(void) const
{
    return m_pcrtc;
}

QList< EasyRandR::OutputProperty > EasyRandR::Output::listOutputProperties(void) const
{
    int num_properties;
    Display* dpy = XOpenDisplay(NULL);
    Atom* property;
    QList<EasyRandR::OutputProperty> prop_list;
    
    if (dpy) {
	property = XRRListOutputProperties(dpy, m_outputId, &num_properties);
	XCloseDisplay(dpy);
	for (int i=0; i<num_properties; i++) {
	    EasyRandR::OutputProperty tmp(m_outputId, property[i]);
	    prop_list.append(tmp);
	}
    }
    
    return prop_list;
}
