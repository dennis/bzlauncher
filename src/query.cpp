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
#include <wx/tokenzr.h>
#include "server.h"
#include "query.h"

bool Query::operator==(const Server& server) const {
	bool result = true;
	wxString token;

	for(unsigned int i = 0; i < this->tokens.GetCount() && result; i++) {
		token = this->tokens[i];

		if( token.CmpNoCase(_T("ALL")) == 0 ) 
			result &= true;
		else
			result &= server.matches(token);
		/*
		else if( token.CmpNoCase(_T("CTF")) == 0 )
			result &= server->gameType.value.isCTF();
		else if( token.CmpNoCase(_T("FFA")) == 0 )
			result &= server->gameType.value.isFFA();
		else if( token.CmpNoCase(_T("RC")) == 0 )
			result &= server->gameType.value.isRC();
		else if( token.CmpNoCase(_T("FAVORITE")) == 0 )
			result &= server->favorite;
		else if( token.CmpNoCase(_T("RECENT")) == 0 )
			result &= server->recent;
		else 
			result &= 
				(server->name().Find(token) != wxNOT_FOUND) ||
				(server->longName().Find(token) != wxNOT_FOUND) ||
				(server->protocolVersion().Find(token) != wxNOT_FOUND);
		*/
	}

	return result;
}

void Query::set(const wxString &q) {
	this->query = q;

	this->tokens.Clear();

	wxStringTokenizer	tok(q, _T(" "));

	while(tok.HasMoreTokens()) {
		wxString str = tok.GetNextToken();
		if(str.Len() > 2 && str[0] == '\'' && str[str.Len()-1] == '\'')
			this->name = str.Mid(1,str.Len()-2);
		else
			this->tokens.Add(str.MakeLower());
	}
}
