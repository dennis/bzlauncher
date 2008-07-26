/*
The MIT License

Copyright (c) 2008 Dennis Møllegaard Pedersen <dennis@moellegaard.dk>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#ifndef __attribute_h__
#define __attribute_h__

#include <wx/string.h>

class AttributeBase {
public:
	virtual operator wxString() const = 0;
	virtual wxString operator()() const = 0;
	virtual wxString aswxString() const = 0;
	virtual AttributeBase* dupe() const = 0;
};

template< typename T >
class Attribute : public AttributeBase {
public:
	T	value;

	Attribute() {};
	Attribute(T val) {
		this->value = val;
	}

	AttributeBase* dupe() const {
		return new Attribute<T>(value);
	}

	// TODO Remove these next two functions
	operator wxString() const {
		return convertTowxString(this->value);
	}
	wxString operator()() const {
		return convertTowxString(this->value);
	}
	wxString aswxString() const {
		return convertTowxString(this->value);
	}
};

inline wxString convertTowxString(const bool& v) {
	if(v)
		return wxString(_T("Yes"));
	else
		return wxString(_T("No"));
}

inline wxString convertTowxString(const wxString& v) {
	return wxString(v);
}

inline wxString convertTowxString(uint16_t v) {
	return wxString::Format(_T("%d"), v);
}

inline wxString convertTowxString(const unsigned char& v) {
	return wxString::Format(_T("%d"), v);
}

#endif
