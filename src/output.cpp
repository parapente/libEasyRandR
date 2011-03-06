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


#include "output.h"
#include "screen.h"

EasyRandR::Output::Output(Display* dpy, Window w, int oid, XRRScreenResources *scrres): display(dpy),
									    window(w),
									    id(oid),
									    screenResources(scrres)
{
    info = NULL;
    updateInfo();
}

EasyRandR::Output::~Output()
{
    if (info)
	XRRFreeOutputInfo(info);
}

void EasyRandR::Output::setScreenResources(XRRScreenResources* scrres)
{
    screenResources = scrres;
}

void EasyRandR::Output::updateInfo(void)
{
    if (info)
	XRRFreeOutputInfo(info);
    info = XRRGetOutputInfo(display,screenResources,id);
    if (info)
	valid = true;
    else
	valid = false;
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
	return QString(QByteArray(info->name,info->nameLen));
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

QList< RRMode > EasyRandR::Output::validModes(void )
{
    QList<RRMode> list;
    
    if (info)
	for (int i=0; i<info->nmode; i++)
	    list << info->modes[i];

    return list;
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
