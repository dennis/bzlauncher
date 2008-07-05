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
#ifndef __label_h__
#define __label_h__

#include <wx/string.h>
#include "config.h"

class Label {
protected:
	wxString	tag;
	wxString	name;
	long		pos;
	long		width;
public:
	Label(const wxString& _t, const wxString& _n) : tag(_t), name(_n) {
		appConfig.loadLabelSettings(this);
	}

	~Label() {
		appConfig.saveLabelSettings(this);
	}

	const wxString& getName() const {
		return this->name;
	}
	const wxString& getTag() const {
		return this->tag;
	}

	void setWidth(long w) {
		this->width = w;
	}
	void setPos(long p) {
		this->pos = p;
	}

	long getWidth() const {
		return this->width;
	}

	long getPos() const {
		return this->pos;
	}
};

#endif
