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
#ifndef __server_h__
#define __server_h__

#include <map>
#include <wx/log.h>
#include "attribute.h"

class Label;

struct ServerData {
	typedef std::map<const Label*, AttributeBase* > attributemap_t;
	attributemap_t	attributes;

	int query_ref; 	// How many queries are referencing this..

	ServerData() : query_ref(0) {
		wxLogDebug(_T("ServerData() %p"), this);
	}

	~ServerData() {
		wxLogDebug(_T("~ServerData() %p"), this);
		for(attributemap_t::iterator i = this->attributes.begin(); i!= this->attributes.end(); ++i)
			delete i->second;
	}
};

class Server {
protected:
	ServerData*	data;
public:
	Server() {
		this->data = new ServerData;
		this->data->query_ref++;
		wxLogDebug(_T("Server() ServerData[%p %d]"), this->data, this->data->query_ref);
	}
	Server(const Server& s) {
		this->data = s.data;
		s.data->query_ref++;
		wxLogDebug(_T("Server(Server&) ServerData[%p %d]"), this->data, this->data->query_ref);
	}
	~Server() {
		wxLogDebug(_T("~Server() ServerData[%p %d]"), this->data, this->data->query_ref);
		this->data->query_ref--;
		if(this->data->query_ref==0)
			delete this->data;
	}

	Server& operator=(const Server& s) {
		this->data = s.data;
		s.data->query_ref++;
		wxLogDebug(_T("operator=() ServerData[%p %d]"), this->data, this->data->query_ref);
		return *this;
	}

	template<typename T>
	void update(const Label* l, const Attribute<T>& val) {
		if( this->data->attributes.find(l) != this->data->attributes.end() )
			delete this->data->attributes[l];
		Attribute<T>* newval = new Attribute<T>(val);
		this->data->attributes[l] = newval;
	}

	AttributeBase* get(const Label* l) {
		AttributeBase* res = NULL;
		if( this->data->attributes.find(l) != this->data->attributes.end() )
			res = this->data->attributes[l];

		return res;
	}
};

#endif
