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
    explicit Output(Window w, RROutput oid, EasyRandR::Screen* scr);
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
     * @brief The new rotation & reflect setting
     *
     * This setting has not been applied to the output yet. If there was no change,
     * the current rotation is returned.
     * 
     * @return Rotation
     **/
    Rotation newRotation(void);
    
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
     * @brief The maximum value the x position can take depending on the resolution selected
     *
     * If you have set a new mode, it will use its resolution even though the new mode isn't
     * applied yet.
     * 
     * @return uint
     **/
    uint maxX(void);

    /**
     * @brief The maximum value the y position can take depending on the resolution selected
     * 
     * If you have set a new mode, it will use its resolution even though the new mode isn't
     * applied yet.
     *
     * @return uint
     **/
    uint maxY(void);
    
    /**
     * @brief The x position of the output in the virtual framebuffer called screen
     *
     * @return int
     **/
    int x(void);
    
    /**
     * @brief The new x position of the output in the virtual framebuffer called screen
     *
     * This setting has not been applied to the output yet. If there was no change,
     * the current x is returned.
     * 
     * @return int
     **/
    int newx(void);
    
    /**
     * @brief The y position of the output in the virtual framebuffer called screen
     *
     * @return int
     **/
    int y(void);
    
    /**
     * @brief The new y position of the output in the virtual framebuffer called screen
     *
     * This setting has not been applied to the output yet. If there was no change,
     * the current y is returned.
     * 
     * @return int
     **/
    int newy(void);
    
    /**
     * @brief Get the id of the current mode used by the output
     *
     * @return RRMode
     **/
    RRMode currentMode(void);
    
    /**
     * @brief Get the id of the new mode that will be used by the output.
     * 
     * This setting has not been applied to the output yet. If no mode change happened,
     * the current mode is returned.
     *
     * @return RRMode
     **/
    RRMode newMode(void);
    
    /**
     * @brief Set the new position of the output in screen space.
     * 
     * Notice that this setting is not applied immedeately. It keeps the
     * new position in memory until applySettings is called.
     *
     * @param x coordinate
     * @param y coordinate
     **/
    void setPos( uint x, uint y );
    
    /**
     * @brief Sets the new mode used for this output and all it's clones.
     *
     * Notice that this setting is not applied immedeately. It keeps the
     * new mode in memory until applySettings is called. Returns true if
     * the mode can be used, false otherwise.
     *
     * @param mode is the new mode id
     * @return bool
     **/
    bool setMode(RRMode mode);
    
    /**
     * @brief Sets the rotation and reflection used by this output and
     * all it's clones.
     * 
     * Notice that this setting is not applied immediately. It keeps the
     * new rotation in memory until applySettings is called. Returns true
     * if the rotation is valid, false otherwise.
     *
     * @param rotation is the new rotation & reflection
     * @return bool
     **/
    bool setRotation(Rotation rotation);
    
    /**
     * @brief Sets the list of outputs that will use the new settings.
     * 
     * Notice that this setting is not applied immediately. It keeps the
     * new list in memory until applySettings is called. Returns true if
     * the list is valid, false otherwise.
     *
     * @param outputs is a list of output ids
     * @return bool
     **/
    bool setOutputs(QList<RROutput> outputs);
    int applySettings(void);
    
    /**
     * @brief Returns the randr id of the output.
     * 
     * @return RROutput
     **/
    RROutput id(void);
    
    bool positionChanged(void);
    bool modeChanged(void);
    bool rotationChanged(void);
    bool outputsChanged(void);
    void off(void);
    void on(void);
    
private:
    Window m_window;
    RROutput m_outputId;
    Screen *m_screen;
    bool m_valid;
    XRROutputInfo *m_info;
    Crtc *m_pcrtc;
    
    bool m_positionChanged;
    bool m_modeChanged;
    bool m_rotationChanged;
    bool m_outputsChanged;
    uint m_newx, m_newy;
    RRMode m_newmode;
    Rotation m_newrotation;
    QList<RROutput> m_newoutputs;
};

}
#endif // OUTPUT_H
