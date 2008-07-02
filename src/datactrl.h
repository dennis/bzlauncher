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
#ifndef __datactrl_h__
#define __datactrl_h__

#include <wx/string.h>
#include <wx/log.h>
#include <vector>
#include <map>

#include "label.h"
#include "attribute.h"
#include "datasrc.h"

class DataEntity {
protected:
	typedef std::map<const Label*, AttributeBase* > attributemap_t;
	attributemap_t	attributes;
public:
	~DataEntity() {
		for(attributemap_t::iterator i = this->attributes.begin(); i != this->attributes.end(); ++i )
			if( i->second )
				delete i->second;
	}

	template<typename T>
	void update(const Label* l, const Attribute<T>& val) {
		Attribute<T>* valdupe = new Attribute<T>(val);
		this->attributes[l] = valdupe;
	}
};

// This class controls/owns the datalist (serverlist) 
class DataController {
protected:
	typedef std::vector<DataSource*>		sourcelist_t;
	typedef std::map<wxString,DataEntity*>	entitymap_t;

	typedef std::map<wxString,Label*>		labelmap_t;

	sourcelist_t	sourceList;
	entitymap_t		serverList;
	labelmap_t		labelMap;

public:
	~DataController();

	template<typename T>
	void updateAttribute(const wxString& name, const Label* l, const Attribute<T>& val) {
		// Remember to use locks
		wxLogDebug(_T("Update %s attribute for label %lx [%s] = '%s'"), name.c_str(), l, l->getName().c_str(), static_cast<wxString>(val).c_str());

		entitymap_t::iterator i = this->serverList.find(name);

		if( i == this->serverList.end() )
			this->serverList[name] = new DataEntity();
		this->serverList[name]->update(l,val);
	}

	void addLabel(const wxString& name, Label* l) {
		wxLogDebug(_T("Adding label: [%lx] %s"), l, name.c_str());
		this->labelMap[name] = l;
	}

	void add(DataSource*);
	void run();
	void stop();
};

#endif 
