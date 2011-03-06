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


#ifndef SCREEN_H
#define SCREEN_H

#include <QObject>
#include <X11/extensions/Xrandr.h>

namespace EasyRandR {
    
class Screen: public QObject
{
    Q_OBJECT
    
public:
    explicit Screen(Display *dpy, Window w);
    virtual ~Screen();
    
    Time configTimestamp(void);
    Time timestamp(void);
    QList<RROutput> getOutputs(void);
    QList<RRCrtc> getCrtcs(void);
    QList<RRMode> getModes(void);
    XRRScreenResources *getResources(void);
    bool isInfoValid(void);
    bool isResValid(void);
    void updateInfo(void);
    void updateResources(void);

private:
    Display *display;
    Window window;
    bool infoValid;
    bool resValid;
    
    XRRScreenResources *resources;
    XRRScreenConfiguration *info;
    int minWidth, minHeight, maxWidth, maxHeight;
};

}
#endif // SCREEN_H
