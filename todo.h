#ifndef TODO_H
#define TODO_H

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <boost/serialization/vector.hpp>
#include "serial.h"

#define RED     "\x1b[31m"
#define YELLOW  "\x1b[33m"
#define GREEN   "\x1b[32m"
#define CYAN    "\x1b[36m"
#define BLUE    "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define RESET   "\x1b[0m"

using std::string;
using std::vector;
using std::cout;
using std::cin;
using std::endl;
using std::ostream;

struct todo_item {
	template <class Archive>
	void serialize(Archive& ar, const unsigned version) {
		ar & name;
		ar & description;
		ar & priority;
    }

	string name;
	string description;
	unsigned priority;	
};

ostream& operator<<(ostream& out, const todo_item& item) {
	out << "Name: " << item.name << endl;
	out << "Description: " << item.description << endl;
	out << "Priority: " << item.priority << endl;	

	return out;
}

string colorize(const todo_item& ti) {
	if (ti.priority == 0)
		return RED;
	else if (ti.priority == 1)
		return YELLOW;
	else if (ti.priority == 2)
		return GREEN;
	else if (ti.priority == 3)
		return CYAN;
	else if (ti.priority == 4)
		return BLUE;
	else if (ti.priority >= 5)
		return MAGENTA;
}

class todo_list {
private:
	SERIAL_ACCESS;
	template <class Archive>
	void serialize(Archive& ar, const unsigned version) {
		ar & main_list;
    }

	vector<todo_item> main_list;

public:
	void add(string task, string description = "DEFAULT_DESC", unsigned prior = 3) {
		if (description == "DEFAULT_DESC")
			description = task; // make description same as task name if none specified

		main_list.push_back(todo_item {
			task,
			description,
			prior
        });
    }	

	void del(unsigned n) {
		cout << "Are you sure you want to delete task #" << n << "? ";
		char resp;
		cin >> resp;
		if (resp != 'y')
			return;	

		main_list.erase(main_list.begin() + n);
    }

	void prioritize(unsigned n, unsigned prior = 0) {
		// defaults to highest priority
		main_list.at(n).priority = prior;
    }

	void show() {
		unsigned cnt = 0;
		for (auto t: main_list) {
			cout << colorize(t);
			cout << cnt << ". " << t.name << RESET << endl;	
			++cnt;
        }
    }

	void showtask(unsigned n) {
		cout << main_list.at(n);
    }
};

// TODO 
// add color based on priority
// overload output for item, list

#endif // TODO_H
