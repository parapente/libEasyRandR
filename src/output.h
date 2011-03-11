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
#include <QMap>
#include <X11/extensions/Xrandr.h>
#include <X11/extensions/randr.h>
#include "screen.h"
#include "crtc.h"

namespace EasyRandR {
    
/**
 * @brief Graphics card output
 * 
 * It represents one output from the set outputs from all graphics cards
 * attached to the system.
 **/
class Output : public QObject
{
    Q_OBJECT
    
public:
    explicit Output(Display* dpy, Window w, int oid, EasyRandR::Screen* scrres);
    virtual ~Output();
    
    /**
     * @brief Returns true if the information available is valid, false otherwise.
     *
     * @return bool
     **/
    bool isValid(void);
    
    /**
     * @brief Refresh the output info
     *
     * @return void
     **/
    void updateInfo(void);
    
    /**
     * @brief Returns the list of output ids that are clones of this output
     *
     * @return QList< RROutput >
     **/
    QList<RROutput> clones(void);
    
    /**
     * @brief Returns the list of valid crtc connections
     *
     * @return QList< RRCrtc >
     **/
    QList<RRCrtc> validCrtcs(void);
    
    /**
     * @brief Returns the crtc id this output is connected to.
     *
     * @return RRCrtc
     **/
    RRCrtc crtc(void);
    
    /**
     * @brief Gives the time of the last change of the output
     *
     * @return Time
     **/
    Time timestamp(void);
    
    /**
     * @brief Returns the name of the output
     *
     * @return QString
     **/
    QString name(void);
    
    /**
     * @brief Returns a map of all valid modes.
     * 
     * The map uses the mode id as key and the mode name as value.
     *
     * @return QMap< RRMode, QString >
     **/
    QMap<RRMode,QString> validModes(void);
    
    /**
     * @brief Returns the status of the output connection.
     * 
     * It can be either connected, disconnected or unknown, so the
     * returned value will be RR_Connected, RR_Disconnected or
     * RR_UnknownConnection. Their values are declared in randr.h.
     *
     * @return Connection
     **/
    Connection connectionStatus(void);
    
    /**
     * @brief The height of the output in mm
     *
     * @return ulong
     **/
    ulong heightmm(void);
    
    /**
     * @brief The width of the output in mm
     *
     * @return ulong
     **/
    ulong widthmm(void);
    
    /**
     * @brief Gives the subpixel order of the output
     *
     * @return SubpixelOrder
     **/
    SubpixelOrder subpixelOrder(void);
    
    /**
     * @brief Gives the ouput's prefered modes.
     * 
     * Some modes are prefered over others as they will give the
     * best possible result. Using this method you can get a list
     * of all the prefered modes' ids.
     *
     * @return QList< RRMode >
     **/
    QList<RRMode> preferedModes(void);
    
    // Functions using Crtc object
    /**
     * @brief Returns all valid rotation settings for this output.
     * 
     * The value returned contains a mask of all the available settings.
     * You can use bitwise operations to extract which setting can be used,
     * using the RR_Rotate* and RR_Reflect* values from randr.h
     *
     * @return Rotation
     **/
    Rotation validRotations(void);
    
    /**
     * @brief Returns the current rotation & reflect setting
     *
     * @return Rotation
     **/
    Rotation currentRotation(void);
    
    /**
     * @brief The width of the output in pixels
     *
     * @return uint
     **/
    uint width(void);
    
    /**
     * @brief The height of the output in pixels
     *
     * @return uint
     **/
    uint height(void);
    
    /**
     * @brief The x position of the output in the virtual framebuffer called screen
     *
     * @param  ...
     * @return int
     **/
    int x(void);
    
    /**
     * @brief The y position of the output in the virtual framebuffer called screen
     *
     * @return int
     **/
    int y(void);
    
    /**
     * @brief Get the id of the current mode used by the output
     *
     * @return RRMode
     **/
    RRMode currentMode(void);
    
private:
    Display* display;
    Window window;
    int id;
    Screen *screen;
    bool valid;
    XRROutputInfo *info;
    Crtc *pcrtc;
};

}
#endif // OUTPUT_H
