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


#ifndef CRTC_H
#define CRTC_H

#include <QObject>
#include <X11/extensions/Xrandr.h>

namespace EasyRandR {
    
class Crtc : public QObject
{
public:
    explicit Crtc(Display *dpy, XRRScreenResources *res, RRCrtc crtc);
    
    bool isValid(void);
    void setScreenResources(XRRScreenResources *res);
    void updateInfo(void);
    uint width(void);
    uint height(void);
    RRMode mode(void);
    Rotation rotation(void);
    Rotation supportedRotations(void);
    QList<RROutput> connectedTo(void);
    Time timestamp(void);
    int x(void);
    int y(void);
    
private:
    Display *display;
    XRRScreenResources *screenResources;
    RRCrtc id;
    XRRCrtcInfo *info;
    bool valid;
};

}
#endif // CRTC_H
