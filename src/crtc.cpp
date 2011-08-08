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

int EasyRandR::Crtc::getGammaSize(void) const
{
    Display *dpy = XOpenDisplay(NULL);
    int size;

    if (dpy) {
	size = XRRGetCrtcGammaSize(dpy,m_id);
	XCloseDisplay(dpy);
    }
    else
	size = -1;
    
    return size;
}

XRRCrtcGamma* EasyRandR::Crtc::getGamma(void) const
{
    XRRCrtcGamma* gamma = NULL;
    quint64 value;
    
    int size = getGammaSize();
    
    if (size == -1) // Error getting gamma size
	return gamma;

    Display *dpy = XOpenDisplay(NULL);
    if (dpy)
	gamma = XRRGetCrtcGamma(dpy,m_id);
    XCloseDisplay(dpy);
    return gamma;
}

int EasyRandR::Crtc::setGamma(QList<quint16> red, QList<quint16> green, QList<quint16> blue)
{
    XRRCrtcGamma* crtcGamma = NULL;

    int size = getGammaSize();
    
    if (size == -1) // Error getting gamma size
	return -1;
    
    Display *dpy = XOpenDisplay(NULL);
    if (!dpy)
	return -2;
    
    crtcGamma = XRRAllocGamma(size);
    
    if (!crtcGamma)
	return -3;
    
    crtcGamma->red = new unsigned short int[size];
    crtcGamma->green = new unsigned short int[size];
    crtcGamma->blue = new unsigned short int[size];

    for (int i=0; i<size; i++)
	crtcGamma->red[i] = red[i];
    for (int i=0; i<size; i++)
	crtcGamma->green[i] = green[i];
    for (int i=0; i<size; i++)
	crtcGamma->blue[i] = blue[i];
    
    XRRSetCrtcGamma(dpy,m_id,crtcGamma);
    XCloseDisplay(dpy);
}

int EasyRandR::Crtc::setTransformationMatrix(EasyRandR::Crtc::Matrix transformation, QString filter, QList<XFixed> parameters)
{
    Display *dpy = XOpenDisplay(NULL);
    XFixed* params;
    int num_params;
    
    if (dpy) {
	num_params = parameters.count();
	params = new XFixed[num_params];
	for (int i=0; i<num_params; i++)
	    params[i] = parameters.at(i);
	XTransform data;
	for (int i=0; i<3; i++)
	    for (int j=0; j<3; j++)
		data.matrix[i][j] = transformation.data(i,j);
	XRRSetCrtcTransform(dpy, m_id, &data, filter.toUtf8().data(), params, num_params);
	XCloseDisplay(dpy);
	delete params;
	return 0;
    }
    else
	return -1;
}

EasyRandR::Crtc::TransformAttributes EasyRandR::Crtc::getTransformationMatrix(void)
{
    Display *dpy = XOpenDisplay(NULL);
    XRRCrtcTransformAttributes** attributes;
    EasyRandR::Crtc::TransformAttributes ret_attribs;

    if (dpy) {
	XRRGetCrtcTransform(dpy, m_id, attributes); // TODO: Check return value
	ret_attribs.setAttributes(*attributes);
	XFree(*attributes);
	XCloseDisplay(dpy);
    }
    
    return ret_attribs;
}

EasyRandR::Crtc::Matrix::Matrix()
{
    for (int i=0; i<3; i++)
	for (int j=0; j<3; j++)
	    m_matrix[i][j] = 0;
}

XFixed EasyRandR::Crtc::Matrix::data(uint i, uint j)
{
    if (i>3 || j>3)
	return 0;
    return m_matrix[i][j];
}

void EasyRandR::Crtc::Matrix::setData(uint i, uint j, XFixed value)
{
    if (!(i>3 || j>3))
	m_matrix[i][j] = value;
}

QString EasyRandR::Crtc::TransformAttributes::currentFilter(void)
{
    return m_currentFilter;
}

QList< XFixed > EasyRandR::Crtc::TransformAttributes::currentParams(void)
{
    return m_currentParams;
}

EasyRandR::Crtc::Matrix EasyRandR::Crtc::TransformAttributes::currentTransform(void)
{
    return m_currentTransform;
}

QString EasyRandR::Crtc::TransformAttributes::pendingFilter(void)
{
    return m_pendingFilter;
}

QList< XFixed > EasyRandR::Crtc::TransformAttributes::pendingParams(void)
{
    return m_pendingParams;
}

EasyRandR::Crtc::Matrix EasyRandR::Crtc::TransformAttributes::pendingTransform(void)
{
    return m_pendingTransform;
}

void EasyRandR::Crtc::TransformAttributes::setAttributes(XRRCrtcTransformAttributes* attributes)
{
    m_currentFilter = attributes->currentFilter;
    m_pendingFilter = attributes->pendingFilter;
    
    for (int i=0; i<3; i++)
	for (int j=0; j<3; j++) {
	    m_currentTransform.setData(i, j, attributes->currentTransform.matrix[i][j]);
	    m_pendingTransform.setData(i, j, attributes->pendingTransform.matrix[i][j]);
	}
    for (int i=0; i<attributes->currentNparams; i++)
	m_currentParams.append(attributes->currentParams[i]);
    for (int i=0; i<attributes->pendingNparams; i++)
	m_pendingParams.append(attributes->pendingParams[i]);
}
