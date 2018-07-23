#include <iostream>

#include "listLib.h"

using namespace std;

template <class T>
void print(T& data) {
	cout << data << " -> ";
}

int main(int arg, char** argv) {
	L1List<int> *list = new L1List<int>();
	int a = 1;
	int b = 2;
	int c = 3;

	list->insert(0, a);
	list->traverse(print);
	cout << endl;

	list->insert(0, b);
	list->traverse(print);
	cout << endl;

	list->insert(0, c);
	list->traverse(print);
	cout << endl;

	list->reverse();
	list->traverse(print);
	cout << endl;

	int d = -1;
	list->find(a, d);
	cout << d;
}
