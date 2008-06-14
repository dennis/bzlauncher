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
#include "server.h"

Server::Server() : fullyParsed(false), favorite(false), recent(false) {
}

int Server::GetPlayerCount() const {
	// This it to avoid discarding qualifiers. As we operate only with a handfull
	// of different teams, the performance hit won't be any issue. But there gotta
	// be a better way.
	Teams::teammap_t	color_const = this->teams.color;	
	return 
		color_const[Server::team_rogue].value.getCount()+
		color_const[Server::team_red].value.getCount()+
		color_const[Server::team_green].value.getCount()+
		color_const[Server::team_blue].value.getCount()+
		color_const[Server::team_purple].value.getCount();
}

bool Server::IsFull() const {
	return this->GetPlayerCount() == this->maxPlayers.value;
}

bool Server::IsEmpty() const {
	return this->GetPlayerCount() == 0;
}

void Server::setIP(const wxIPV4address& val) {
	this->ip = val;
	this->ping = Ping(val);
}

const wxIPV4address& Server::getIP() const {
	return this->ip;
}
