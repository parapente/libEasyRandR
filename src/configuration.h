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


#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <QObject>
#include <X11/extensions/randrproto.h>
#include <X11/extensions/Xrandr.h>
#include <X11/Xproto.h>
#include "screen.h"
#include "output.h"
#include <QMap>

namespace EasyRandR{
    
class Configuration : public QObject
{
    Q_OBJECT

public:
    explicit Configuration(QObject* parent = 0);
    virtual ~Configuration();
    
    QList<RROutput> getOutputList();
    Output *getOutput(RROutput output);
    
private:
    bool valid;
    int ver_major, ver_minor;
    int eventBase, errorBase;

    Display *display;
    Window window;

    EasyRandR::Screen *screen;
    QMap<int,Output*> outputs;
};

}
#endif // CONFIGURATION_H
