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


#ifndef OUTPUTPROPERTY_H
#define OUTPUTPROPERTY_H
#include <X11/extensions/Xrandr.h>
#include <QList>

namespace EasyRandR{
    
class OutputProperty
{

public:
    OutputProperty(RROutput output);
    bool isValid(void) const;
    int query(void);
    int configure(void) const;
    int change(Atom type, int format, int mode, QList< quint32 > data) const;
    int remove(void) const;
    int getValue(Atom type, long offset, long length, bool del, bool pending, Atom *actual_type, int *actual_format,
		 unsigned long *nitems, unsigned long *bytes_after, unsigned char **prop);
    void setPending(bool value);
    void setRange(bool value);
    void setValues(QList<long> values);
    bool pending(void) const;
    bool range(void) const;
    QList<long> validValues(void) const;
    
private:
    bool m_valid;
    RROutput m_output;
    Atom m_id;
    bool m_pending;
    bool m_range;
    bool m_immutable;
    QList<long> m_valid_values;
};

}
#endif // OUTPUTPROPERTY_H
