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
#include <QSize>

namespace EasyRandR {
    
/**
  * @brief Use this class to access the randr screen
  * 
  * This isn't a real screen but a virtual space where all graphics are being
  * rendered. Crtcs display parts of this framebuffer to different outputs. You
  * shouldn't normally use objects of this class.
  **/
class Screen: public QObject
{
    Q_OBJECT
    
public:
    explicit Screen( Window w, int scid);
    virtual ~Screen();
    
    /**
     * @brief Returns the last screen config timestamp
     *
     * @return Time
     **/
    Time configTimestamp(void) const;
    
    /**
     * @brief Returns the last timestamp
     *
     * @return Time
     **/
    Time timestamp(void) const;
    
    /**
     * @brief Get the list of available outputs
     *
     * The list contains the output ids of all available outputs
     * 
     * @return QList< RROutput >
     **/
    QList<RROutput> getOutputs(void) const;
    
    /**
     * @brief Get number of available outputs
     *
     * @return int
     **/
    int getOutputCount(void) const;
    
    /**
     * @brief Get the list of available Crtcs
     *
     * The list contains the crtc ids of all available crtcs
     * 
     * @return QList< RRCrtc >
     **/
    QList<RRCrtc> getCrtcs(void) const;
    
    /**
     * @brief Get a list of available modes
     *
     * The list contains the info of all supported modes by the screen. This
     * list contains all available modes, even some not supported by all
     * outputs.
     * 
     * @return QList< XRRModeInfo >
     **/
    QList< XRRModeInfo > getModes(void) const;
    
    /**
     * @brief Get the screen resources
     * 
     * The screen resources structure cannot be used directly but is needed
     * by many functions for crtcs and outputs.
     *
     * @return XRRScreenResources*
     **/
    XRRScreenResources *getResources(void) const;
    
    /**
     * @brief Returns true if screen info is valid, false otherwise
     *
     * @return bool
     **/
    bool isInfoValid(void) const;
    
    /**
     * @brief Returns true if screen resources are valid, false otherwise
     *
     * @return bool
     **/
    bool isResValid(void) const;
    
    /**
     * @brief Updates the screen info
     *
     * @return void
     **/
    void updateInfo(void);
    
    /**
     * @brief Updates the screen resources
     *
     * @return void
     **/
    void updateResources(void);
    
    /**
     * @brief Sets the size of the screen
     *
     * Returns true on success, false otherwise.
     * 
     * @param s holds the new size of the screen
     * @return bool
     **/
    bool setSize(QSize s);
    
    QSize getSize(void) const;
    uint minWidth(void) const;
    uint maxWidth(void) const;
    uint minHeight(void) const;
    uint maxHeight(void) const;

private:
    int m_id;
    Window m_window;
    bool m_infoValid;
    bool m_resValid;
    
    XRRScreenResources *m_resources;
    XRRScreenConfiguration *m_info;
    
    // These are all set as int and not uint deliberately. XRRGetScreenSizeRange doesn't like uints!
    int m_minWidth, m_minHeight, m_maxWidth, m_maxHeight;
};

}
#endif // SCREEN_H
