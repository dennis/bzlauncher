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
#ifndef __datasrc_h__
#define __datasrc_h__

#include <wx/thread.h>
#include <wx/log.h>
#include <wx/string.h>

#include "attribute.h"
#include "label.h"
#include "queue.h"

class DataController;

class FullAttributeInfo {
public:
	AttributeBase*	attr;
	const Label*			label;
	const wxString			server;

	FullAttributeInfo(AttributeBase* a, Label* l, wxString s) 
		: attr(a), label(l), server(s) {
	}
};

// This class controls/owns the datalist (serverlist) 
class DataSource : public wxThread {
public:
	Queue< FullAttributeInfo > out_queue;		// queue that is read by data-ctrl

	template<typename T>
	void updateAttribute(const wxString& name, Label* l, const Attribute<T>& attr) {
		out_queue.push(FullAttributeInfo(attr.dupe(), l, name));
	}

	DataSource();

	virtual void initializeLabels(DataController*) = 0;
};

#endif 
