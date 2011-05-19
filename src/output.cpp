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
#include "output.h"
#include <QPoint>

EasyRandR::Output::Output(Display* dpy, Window w, RROutput oid, Screen *scr): display(dpy),
									    window(w),
									    outputId(oid),
									    screen(scr)
{
    info = NULL;
    pcrtc = NULL;
    m_positionChanged = m_modeChanged =  m_rotationChanged = m_outputsChanged = false;
    m_newmode = m_newrotation = m_newx = m_newy = 0;
    updateInfo();
    if (info && screen->isResValid() && (info->crtc!=0)) {
	pcrtc = new Crtc(display,screen,info->crtc);
    }
}

EasyRandR::Output::~Output()
{
    if (info)
	XRRFreeOutputInfo(info);
}

void EasyRandR::Output::updateInfo(void)
{
    if (info)
	XRRFreeOutputInfo(info);
    info = XRRGetOutputInfo(display,screen->getResources(),outputId);
    if (info)
	valid = true;
    else {
	qDebug() << "Can't get info!";
	valid = false;
    }
}

bool EasyRandR::Output::isValid(void )
{
    return valid;
}

RRCrtc EasyRandR::Output::crtc(void )
{
    if (info)
	return info->crtc;
    else
	return 0;
}

Time EasyRandR::Output::timestamp(void )
{
    if (info)
	return info->timestamp;
    else
	return 0;
}

QList< RRCrtc > EasyRandR::Output::validCrtcs(void )
{
    QList<RRCrtc> list;
    
    if (info)
	for (int i=0; i<info->ncrtc; i++)
	    list << info->crtcs[i];

    return list;
}

QList< RROutput > EasyRandR::Output::clones(void )
{
    QList<RROutput> list;

    if (info)
	for (int i=0; i<info->nclone; i++)
	    list << info->clones[i];

    return list;
}

QString EasyRandR::Output::name(void )
{
    if (info)
	return QString::fromUtf8(info->name);
    else
	return QString();
}

Connection EasyRandR::Output::connectionStatus(void )
{
    if (info)
	return info->connection;
    else
	return RR_UnknownConnection;
}

QMap<RRMode,QString> EasyRandR::Output::validModes(void )
{
    QList<RRMode> list;
    QMap<RRMode,QString> map;
    
    if (info) {
	// Get the valid modes for the output
	for (int i=0; i<info->nmode; i++)
	    list << info->modes[i];
	
	// Get the valid modes for the screen
	QList<XRRModeInfo> mlist;
	mlist = screen->getModes();
	
	// Create the map of modes that contains keys in the form of RRMode,Modename
	for (int i=0; i<list.count(); i++) {
	    QString Modename;
	    for (int j=0; j<mlist.count(); j++)
		if (list[i] == mlist[j].id) {
		    // Calculate Refresh Rate
		    float rr = (float) mlist[i].dotClock / (mlist[i].hTotal*mlist[i].vTotal);
		    Modename = QString::fromUtf8(mlist[i].name) + "@" + QString::number(rr,'f',1);
		}
	    map.insert(list[i],Modename);
	}
    }

    return map;
}

ulong EasyRandR::Output::heightmm(void )
{
    if (info)
	return info->mm_height;
    else
	return 0;
}

ulong EasyRandR::Output::widthmm(void )
{
    if (info)
	return info->mm_width;
    else
	return 0;
}

SubpixelOrder EasyRandR::Output::subpixelOrder(void )
{
    if (info)
	return info->subpixel_order;
    else
	return 0;
}

QList< RRMode > EasyRandR::Output::preferedModes(void )
{
    QList<RRMode> list;
    
    if (info)
	for (int i=0; i<info->npreferred; i++)
	    list << info->modes[i];

    return list;
}

RRMode EasyRandR::Output::currentMode(void )
{
    if (pcrtc)
	return pcrtc->mode();
    else
	return 0;
}

uint EasyRandR::Output::height(void )
{
    if (pcrtc)
	return pcrtc->height();
    else
	return 0;
}

uint EasyRandR::Output::width(void )
{
    if (pcrtc)
	return pcrtc->width();
    else
	return 0;
}

int EasyRandR::Output::x(void )
{
    if (pcrtc)
	return pcrtc->x();
    else
	return 0;
}

int EasyRandR::Output::y(void )
{
    if (pcrtc)
	return pcrtc->y();
    else
	return 0;
}

Rotation EasyRandR::Output::currentRotation(void )
{
    if (pcrtc)
	return pcrtc->rotation();
    else
	return RR_Rotate_0;
}

Rotation EasyRandR::Output::validRotations(void )
{
    if (pcrtc)
	return pcrtc->supportedRotations();
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
	if (pcrtc->possibleOutputs().indexOf(outputs[i]) == -1)
	    ret = false;

    if (ret) {
	m_newoutputs = outputs;
	m_outputsChanged = true;
    }
    
    return ret;
}

int EasyRandR::Output::applySettings(void )
{
    if (pcrtc) {
	if (!m_positionChanged) {
	    m_newx = pcrtc->x();
	    m_newy = pcrtc->y();
	}
	
	if (!m_modeChanged)
	    m_newmode = pcrtc->mode();
	
	if (!m_rotationChanged)
	    m_newrotation = pcrtc->rotation();
	
	if (!m_outputsChanged)
	    m_newoutputs = pcrtc->connectedTo();
	
	return pcrtc->setCrtcConfig(m_newx, m_newy, m_newmode, m_newrotation, m_newoutputs);
    }
    else
	return -1; // TODO: Find a more appropriate return value
}

RROutput EasyRandR::Output::id(void )
{
    return outputId;
}

RRMode EasyRandR::Output::newMode(void )
{
    if (m_modeChanged)
	return m_newmode;
    else
	return pcrtc->mode();
}

int EasyRandR::Output::newx(void )
{
    if (m_positionChanged)
	return m_newx;
    else
	return pcrtc->x();
}

int EasyRandR::Output::newy(void )
{
    if (m_positionChanged)
	return m_newy;
    else
	return pcrtc->y();
}

bool EasyRandR::Output::modeChanged(void )
{
    return m_modeChanged;
}

bool EasyRandR::Output::outputsChanged(void )
{
    return m_outputsChanged;
}

bool EasyRandR::Output::positionChanged(void )
{
    return m_positionChanged;
}

bool EasyRandR::Output::rotationChanged(void )
{
    return m_rotationChanged;
}
