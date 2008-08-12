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
#ifndef __queue_h__
#define __queue_h__

#include <stack>
#include <wx/thread.h>

template<typename T>
class Queue {
private:
	std::stack<T>	stack;
	wxMutex			mutex;
	wxMutex			cond_mutex;
	wxCondition		cond;

public:
	Queue() : cond(cond_mutex) {
	}

	void push(T& t) {
		wxMutexLocker m(this->mutex);
		this->stack.push(t);
		this->cond.Signal();
	}

	T pop() {
		this->cond.Wait();
		wxMutexLocker m(this->mutex);
		T t = this->stack.top();
		this->stack.pop();
		return t;
	}

	// No blocking
	bool pop_nb(T* t) {
		if( this->cond.Wait() != wxCOND_TIMEOUT ) {
			wxMutexLocker m(this->mutex);
			*t = this->stack.top();
			this->stack.pop();
			return true;
		}

		return false;
	}
};

#endif
