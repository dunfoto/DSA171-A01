/*
 * =========================================================================================
 * Name        : listLib.h
 * Author      : Duc Dung Nguyen
 * Email       : nddung@hcmut.edu.vn
 * Copyright   : Faculty of Computer Science and Engineering - Bach Khoa University
 * Description : library for Assignment 1 - Data structures and Algorithms - Spring 2018
 * =========================================================================================
 */

#ifndef A01_LISTLIB_H
#define A01_LISTLIB_H

#include <string>
using namespace std;

class DSAException {
    int     _error;
    string  _text;
public:

    DSAException() : _error(0), _text("Success") {}
    DSAException(int err) : _error(err), _text("Unknown Error") {}
    DSAException(int err, const char* text) : _error(err), _text(text) {}

    int getError() { return _error; }
    string& getErrorText() { return _text; }
};

template <class T>
struct L1Item {
    T data;
    L1Item<T> *pNext;
    L1Item() : pNext(NULL) {}
    L1Item(L1Item<T>* p) : pNext(p) {}
    L1Item(T &a) : data(a), pNext(NULL) {}
    L1Item(T &&a) : data(std::move(a)), pNext(NULL) {}
};

template <class T>
struct SearchElement {
	int goalIdx;
	int currIdx;
    bool checked = false;
	T*	data;
	SearchElement<T>(T* d) : goalIdx(-1), currIdx(0), data(d) {}
	SearchElement<T>(int i) : goalIdx(i), currIdx(0), data(NULL) {}
	bool isGoal() {
		return goalIdx == currIdx;
	}
	void increaseIdx()	{
		currIdx++;
	}
    bool equalsData(T& d) {
        return d == *data;
    }
};

template <class T>
class L1List {
	T AttrNull;
    L1Item<T>   *_pHead;// The head pointer of linked list
    size_t      _size;// number of elements in this list
public:
    L1List() : _pHead(NULL), _size(0) {}
    ~L1List() {
    	clean();
    }

    void    clean();
    bool    isEmpty() {
        return _pHead == NULL;
    }
    size_t  getSize() {
        return _size;
    }

    bool exist(T &a){
        if (!_pHead) return false;
        L1Item<T> *_pRun = _pHead;
        while (_pRun){
            if (_pRun->data == a) return true;
            _pRun = _pRun->pNext;
        }
        return false;
    }

    T&      at(int i);
    T&      operator[](int i) {
    	return at(i);
    };

    bool    find(T* a, int& idx);
    int     insert(int i, T& a);
    int     remove(int i);
    void    reverse();

    int     push_back(T& a);
    int     insertHead(T& a);
    int     insertHead() {
        _pHead = new L1Item<T>(_pHead);
        _size++;
        return 0;
    }

    int     removeHead();
    int     removeLast();

    void    traverse(void (*op)(T&)) {
        L1Item<T>   *p = _pHead;
        while (p) {
            op(p->data);
            p = p->pNext;
        }
    }
    void    traverse(void (*op)(T&, void*), void* pParam) {
        L1Item<T>   *p = _pHead;
        while (p) {
            op(p->data, pParam);
            p = p->pNext;
        }
    }
};

// ele is a SearchElement object
template <class T>
void _at(T& data, void* ele) {
	SearchElement<T> *e = (SearchElement<T>*) ele;
	if (e->isGoal()) {
		e->data = &data;
	} else {
		e->increaseIdx();
	}
}

template <class T>
T& L1List<T>::at(int i) {
	SearchElement<T> *sElement = new SearchElement<T>(i);
	traverse(_at, sElement);
    if (sElement->data) {
        T* d = sElement->data;
        delete sElement;
        return *d;
    }
    delete sElement;
	return AttrNull;
}

template <class T>
int L1List<T>::insert(int i, T& a) {
	if (getSize() < i || i < 0) return -1;
	if (i == 0) {
		return insertHead(a);
	} else if (i == getSize()) {
		return push_back(a);
	} else {
		L1Item<T> *p = _pHead;
		i--;
		while (i != 0) {
			p = p->pNext;
			i--;
		}
		L1Item<T> *curr = new L1Item<T>(a);
		curr->pNext = p->pNext;
		p->pNext = curr;
		_size++;
		return 0;
	}
}

template <class T>
int L1List<T>::remove(int i) {
	if (getSize() <= i || i < 0) return -1;
	if (i == 0) {
		return removeHead();
	} else if (i == getSize() - 1) {
		return removeLast();
	} else {
		L1Item<T> *p = _pHead;
		i--;
		while (i != 0) {
			p = p->pNext;
			i--;
		}
		L1Item<T> *curr = p->pNext;
		p->pNext = curr->pNext;
		delete curr;
		_size--;
		return 0;
	}
}

template <class T>
void _find(T& data, void *ele) {
    SearchElement<T> *e = (SearchElement<T>*) ele;
    if (e->equalsData(data)) {
        if (!e->checked) {
            e->goalIdx = e->currIdx;
            e->checked = true;
        }
    }
    e->increaseIdx();
}

template <class T>
bool L1List<T>::find(T* a, int& idx) {
    if (a == NULL) return false;
    SearchElement<T> *s = new SearchElement<T>(a);
    traverse(_find, s);
    if (s->checked) {
        idx = s->goalIdx;
        return true;
    }
    return false;
}

template <class T>
void L1List<T>::clean() {
	while (!isEmpty()) {
		removeHead();
	}
}

template <class T>
void L1List<T>::reverse() {
	L1Item<T> *p = _pHead;
	_pHead = NULL;
	_size = 0;
	while (p) {
		insertHead(p->data);
		L1Item<T> *curr = p;
		p = p->pNext;
		delete curr;
	}
}

/// Insert item to the end of the list
/// Return 0 if success
template <class T>
int L1List<T>::push_back(T &a) {
    if (_pHead == NULL) {
        _pHead = new L1Item<T>(a);
    }
    else {
        L1Item<T>   *p = _pHead;
        while (p->pNext) p = p->pNext;
        p->pNext = new L1Item<T>(a);
    }

    _size++;
    return 0;
}

/// Insert item to the front of the list
/// Return 0 if success
template <class T>
int L1List<T>::insertHead(T &a) {
    L1Item<T>   *p = new L1Item<T>(a);
    p->pNext = _pHead;
    _pHead = p;
    _size++;
    return 0;
}

/// Remove the first item of the list
/// Return 0 if success
template <class T>
int L1List<T>::removeHead() {
    if(_pHead) {
        L1Item<T>* p = _pHead;
        _pHead = p->pNext;
        delete p;
        _size--;
        return 0;
    }
    return -1;
}

/// Remove the last item of the list
/// Return 0 if success
template <class T>
int L1List<T>::removeLast() {
    if(_pHead) {
        if(_pHead->pNext) {
            L1Item<T>* prev = _pHead;
            L1Item<T>* pcur = prev->pNext;
            while(pcur->pNext) {
                prev = pcur;
                pcur = pcur->pNext;
            }
            delete pcur;
            prev->pNext = NULL;
        }
        else {
            delete _pHead;
            _pHead = NULL;
        }
        _size--;
        return 0;
    }
    return -1;
}



#endif //A01_LISTLIB_H
