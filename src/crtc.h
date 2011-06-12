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
#include "screen.h"

namespace EasyRandR {
    
class Crtc : public QObject
{
    Q_OBJECT
    
public:
    explicit Crtc(EasyRandR::Screen *scr, RRCrtc crtc);
    virtual ~Crtc();
    
    /**
     * @brief Returns true if crtc info is valid, false otherwise
     *
     * @return bool
     **/
    bool isValid(void) const;
    
    /**
     * @brief Force update of crtc info
     **/
    void updateInfo(void);
    
    /**
     * @brief Returns the width of crtc in pixels
     *
     * @return uint
     **/
    uint width(void) const;
    
    /**
     * @brief Returns the height of crtc in pixels
     *
     * @return uint
     **/
    uint height(void) const;
    
    /**
     * @brief Returns the mode id applied to this crtc
     *
     * @return RRMode
     **/
    RRMode mode(void) const;
    
    /**
     * @brief Get what rotation and/or reflection is applied to this crtc
     *
     * @return Rotation
     **/
    Rotation rotation(void) const;
    
    /**
     * @brief Returns the supported rotations and reflections
     *
     * To get which rotation and reflection is supported you have to use bitwise
     * operations using RR_Rotation* and RR_Reflection* from randr.h
     * 
     * @return Rotation
     **/
    Rotation supportedRotations(void) const;
    
    /**
     * @brief Get which outputs are connected to this crtc
     *
     * The returned list contains the ids of the outputs that are
     * connected to this crtc. All these outputs are clones.
     * 
     * @return QList< RROutput >
     **/
    QList<RROutput> connectedTo(void) const;
    
    /**
     * @brief Returns the current timestamp
     *
     * @return Time
     **/
    Time timestamp(void) const;
    
    /**
     * @brief Get the x coordinate of the crtc in screen space
     *
     * @return uint
     **/
    uint x(void) const;

    /**
     * @brief Get the y coordinate of the crtc in screen space
     *
     * @return uint
     **/
    uint y(void) const;
    
    /**
     * @brief Tries to set a new configuration for this Crtc and returns the result of this try
     *
     * @param x holds the x coordinate in screen space
     * @param y holds the y coordinate in screen space
     * @param mode is the id of the new mode for the crtc
     * @param rotation holds the new rotation/reflection
     * @param outputs is the list of outputs using this crtc
     * @return int
     **/
    int setCrtcConfig(int x, int y, RRMode mode, Rotation rotation, QList<RROutput> outputs);
    
    /**
     * @brief Get possible outputs for this crtc
     * 
     * Returns a list of output ids that can be connected to this crtc.
     *
     * @return QList< RROutput >
     **/
    QList<RROutput> possibleOutputs(void) const;
    
private:
    EasyRandR::Screen *m_screen;
    RRCrtc m_id;
    XRRCrtcInfo *m_info;
    bool m_valid;
};

}
#endif // CRTC_H
