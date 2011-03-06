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


#ifndef OUTPUT_H
#define OUTPUT_H

#include <QObject>
#include <X11/extensions/Xrandr.h>

class Output : public QObject
{
    Q_OBJECT
    
public:
    explicit Output(Display *dpy, Window w, int oid, XRRScreenResources *scrres);
    virtual ~Output();
    
    bool isValid(void);
    void setScreenResources(XRRScreenResources *scrres);
    void updateInfo(void);
    QList<RROutput> clones(void);
    QList<RRCrtc> validCrtcs(void);
    RRCrtc crtc(void);
    Time timestamp(void);
    QString name(void);
    QList<RRMode> validModes(void);
    Connection connectionStatus(void);
    ulong heightmm(void);
    ulong widthmm(void);
    SubpixelOrder subpixelOrder(void);
    QList<RRMode> preferedModes(void);
    
private:
    Display* display;
    Window window;
    int id;
    XRRScreenResources *screenResources;
    bool valid;
    XRROutputInfo *info;
};

#endif // OUTPUT_H
