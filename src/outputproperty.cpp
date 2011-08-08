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


#include "outputproperty.h"

EasyRandR::OutputProperty::OutputProperty(RROutput output, Atom id)
{
    m_output = output;
    m_id = id;
}

int EasyRandR::OutputProperty::remove(void) const
{
    Display* dpy = XOpenDisplay(NULL);
    
    if (dpy)
	XRRDeleteOutputProperty(dpy, m_output, m_id);
    else
	return -1;
}

int EasyRandR::OutputProperty::query(void)
{
    Display* dpy = XOpenDisplay(NULL);
    XRRPropertyInfo *info;
    
    if (dpy)
	info = XRRQueryOutputProperty(dpy, m_output, m_id);
    else
	return -1;
    
    if (info) {
	m_immutable = info->immutable;
	m_pending = info->pending;
	m_range = info->range;
	m_valid_values.clear();
	for (int i=0; i<info->num_values; i++)
	    m_valid_values.append(info->values[i]);
    }
    else
	return -2;
}

bool EasyRandR::OutputProperty::pending(void) const
{
    return m_pending;
}

bool EasyRandR::OutputProperty::range(void) const
{
    return m_range;
}

QList< long int > EasyRandR::OutputProperty::validValues(void) const
{
    return m_valid_values;
}

void EasyRandR::OutputProperty::setPending(bool value)
{
    m_pending = value;
}

bool EasyRandR::OutputProperty::isValid(void) const
{
    return m_valid;
}

void EasyRandR::OutputProperty::setRange(bool value)
{
    m_range = value;
}

void EasyRandR::OutputProperty::setValues(QList< long int > values)
{
    m_valid_values = values;
}

int EasyRandR::OutputProperty::configure(void) const
{
    Display* dpy = XOpenDisplay(NULL);
    long* values = NULL;
    int num_values;

    if (dpy) {
	num_values = m_valid_values.count();
	if (num_values>0) {
	    values = new long[num_values];
	    for (int i=0; i<num_values; i++)
		values[i] = m_valid_values[i];
	}
	XRRConfigureOutputProperty(dpy, m_output, m_id, m_pending, m_range, num_values, values);
	XCloseDisplay(dpy);
    }
    else
	return -1;
}

int EasyRandR::OutputProperty::change(Atom type, int format, int mode, QList< quint32 > data) const
{
    Display* dpy = XOpenDisplay(NULL);
    unsigned char* values = NULL;
    int num_values;
    const unsigned char mask = 0xff;
    
    if (dpy) {
	num_values = data.count();
	if (num_values>0) {
	    values = new unsigned char[num_values*format/8];
	    for (int i=0; i<num_values; i++) {
		for (int j=0; j<(format/8); j++) {
		    values[i*(format/8)+j] = (data[i] >> j) & mask;
		}
	    }
	}
	XRRChangeOutputProperty(dpy, m_output, m_id, type, format, mode, values, num_values*format/8);
	XCloseDisplay(dpy);
    }
    else
	return -1;
}

int EasyRandR::OutputProperty::getValue(Atom type, long int offset, long int length, bool del, bool pending,
					Atom* actual_type, int* actual_format, long unsigned int* nitems,
					long unsigned int* bytes_after, unsigned char** prop)
{
    int return_value;
    Display* dpy = XOpenDisplay(NULL);
    
    if (dpy) {
	return_value = XRRGetOutputProperty(dpy, m_output, m_id, offset, length, del, pending, type, actual_type,
			     actual_format, nitems, bytes_after, prop);
	XCloseDisplay(dpy);
	return return_value;
    }
    else
	return -1;
}
