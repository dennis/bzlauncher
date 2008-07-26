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
protected:
	typedef std::map<const Label*, AttributeBase* > attributemap_t;
	attributemap_t	attributes;
	int query_ref; 	// How many queries are referencing this..
	friend class Server;
public:

	ServerData() : query_ref(0) {
	}

	~ServerData() {
		for(attributemap_t::iterator i = this->attributes.begin(); i!= this->attributes.end(); ++i)
			delete i->second;
	}

	template<typename T>
	void update(const Label* l, const Attribute<T>& val) {
		if( this->attributes.find(l) != this->attributes.end() )
			delete this->attributes[l];
		Attribute<T>* newval = new Attribute<T>(val);
		this->attributes[l] = newval;
	}

	AttributeBase* get(const Label* l) {
		AttributeBase* res = NULL;
		if( this->attributes.find(l) != this->attributes.end() )
			res = this->attributes[l];

		return res;
	}
};

class Server {
protected:
	ServerData*	data;
public:
	Server() {
		this->data = new ServerData();
		this->data->query_ref++;
	}
	Server(const Server& s) {
		this->data = s.data;
		s.data->query_ref++;
	}
	~Server() {
		this->data->query_ref--;
		if(this->data->query_ref==0)
			delete this->data;
	}

	Server& operator=(const Server& s) {
		this->data = s.data;
		s.data->query_ref++;
		return *this;
	}

	Server dupe() const {
		Server	s;

		wxLogDebug(_T("Server::dupe() org-data: %p"), this->data);

		for(ServerData::attributemap_t::const_iterator i = this->data->attributes.begin(); i != this->data->attributes.end(); ++i )
			s.data->attributes[i->first] = i->second->dupe();

		s.data->query_ref++;
		wxLogDebug(_T("Server::dupe() copy-data: %p"), &s.data);

		return s;
	}

	template<typename T>
	void update(const Label* l, const Attribute<T>& val) {
		this->data->update(l,val);
	}

	AttributeBase* get(const Label* l) {
		return this->data->get(l);
	}
};

#endif
