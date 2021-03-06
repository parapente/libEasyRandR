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
#include <QMap>
#include <QVector>
#include <X11/extensions/randrproto.h>
#include <X11/extensions/Xrandr.h>
#include <X11/Xproto.h>
#include "screen.h"
#include "output.h"

namespace EasyRandR{
    
/**
 * @brief The heart of libEasyRandR.
 * 
 * You need to create one configuration object to be able to handle all
 * the other objects of the library.
 **/
class Configuration : public QObject
{
    Q_OBJECT

public:
    explicit Configuration(QObject* parent = 0);
    virtual ~Configuration();

    /**
     * @brief Returns a mapping of the outputs.
     * 
     * The QMap returned will use as key the Output id and every key will have as
     * value an object
     * 
     * @param screen is the number of the screen
     *
     * @return QMap<RROutput,EasyRandR::Output*>
     **/
    QMap<RROutput,Output*> getOutputs(int screen) const;
    
    /**
     * @brief Get a simple list of all outputs of a screen
     *
     * @param screen is the number of the screen
     * @return QList<Output*>
     **/
    QList<Output*> getOutputList(int screen) const;
    
    /**
     * @brief Get the number of virtual framebuffers available
     *
     * @return int
     **/
    int getScreenCount(void) const;
    
    /**
     * @brief Get a mapping of the screens
     * 
     * @return QMap<int,Screen*>
     **/
    QMap<int,Screen*> getScreens(void) const;
    
    /**
     * @brief Get a simple list of all available screens
     * 
     * @return QList<Screen*>
     **/
    QList<Screen*> getScreenList(void) const;
    
    int applyConfiguration(void);
    int setPrimaryOutput(RROutput output, int screen) const;
    RROutput getPrimaryOutput(int screen) const;
    
private:
    bool m_valid;
    int m_ver_major, m_ver_minor;
    int m_eventBase, m_errorBase;

    QList<Window> m_window;

    QVector<QList<EasyRandR::Output*> > m_outputs;
    QMap<int,Screen*> m_screens;
    
    void updateScreenSize(int screen, QList< EasyRandR::Output* > outputList);
};

}
#endif // CONFIGURATION_H
