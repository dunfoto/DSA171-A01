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
class L1List {
	T AttrNull;
    L1Item<T>   *_pHead;// The head pointer of linked list
    size_t      _size;// number of elements in this list
public:
    L1List() : _pHead(NULL), _size(0) {}
    ~L1List() {
    	clean();
    }

    L1Item<T> *getHead(){
        return _pHead;
    }

    void    clean();
    bool    isEmpty() {
        return _pHead == NULL;
    }
    size_t  getSize() {
        return _size;
    }

    bool exist(T &a){
        //if (isEmpty()) return false;
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

    bool    find(T& a, int& idx);
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

    bool removeAll(T& d) {
        if (isEmpty()) return false;
        bool check = false;
        while (_pHead -> data == d) {
            removeHead();
            check = true;
        }
        if (isEmpty()) return check;
        L1Item<T> *pre = _pHead;
        L1Item<T> *p = _pHead->pNext;
        while (p) {
            if (p -> data == d) {
                check = true;
                pre -> pNext = p -> pNext;
                delete p;
                p = pre->pNext;
                _size--;
            } else {
                pre = p;
                p = p -> pNext;
            }
        }
        return check;
    }
};

template <class T>
T& L1List<T>::at(int i) {
    L1Item<T> *p = _pHead;
    while (i > 0) {
        if (p) p = p->pNext;
        i--;
    }
	return p->data;
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
bool L1List<T>::find(T& a, int& idx) {
    L1Item<T> *p = _pHead;
    int i = 0;
    while (p) {
        if (p->data == a) {
            idx = i;
            return true;
        }
        p = p->pNext;
        i++;
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
