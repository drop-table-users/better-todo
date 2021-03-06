#ifndef LISTS_H
#define LISTS_H

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
using std::cerr;
using std::endl;
using std::ostream;
using std::transform;

const string HOME_DIR = getenv("HOME");
const string TASK_PATH = HOME_DIR + "/.tasks.lance";

struct todo_item {
	template <class Archive>
	void serialize(Archive& ar, const unsigned version) {
		ar & name;
		ar & desc;
		ar & priority;
    }

	string name;
	string desc;
	unsigned priority;	
};

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
	else 
		return MAGENTA;
}

ostream& operator<<(ostream& out, const todo_item& item) {
	out << "Name:\t\t" << item.name << endl;
	out << "Description:\t" << item.desc << endl;
	out << "Priority:\t" << item.priority << endl;	

	return out;
}

class todo_list {
private:
	SERIAL_ACCESS;
	template <class Archive>
	void serialize(Archive& ar, const unsigned version) {
		ar & main_list;
		ar & name;
		ar & desc;
    }

	vector<todo_item> main_list;

public:
	string name;
	string desc;

	void add(string task, string description = "DEFAULT_DESC", unsigned prior = 3) {
		if (description == "DEFAULT_DESC")
			description = task; // make description same as task name if none specified

		main_list.push_back(todo_item {
			task,
			description,
			prior
        });
    }	

	template <class T> // this function is the shittiest hack of all time
	void del(unsigned n, T& tlist) {
		cout << "Are you sure you want to delete task #" << n << "? ";
		char resp;
		cin >> resp;
		if (resp != 'y')
			return;	

		tlist.hist_add(main_list.at(n));
		main_list.erase(main_list.begin() + n);
    }

	void prioritize(unsigned n, unsigned prior = 0) {
		// defaults to highest priority
		main_list.at(n).priority = prior;
    }

	todo_item& at(unsigned n) {
		return main_list.at(n);
    }

	void show() {
		unsigned cnt = 0;
		for (auto t: main_list) {
			cout << colorize(t);
			cout << cnt << ". " << t.name << RESET << endl;	
			++cnt;
        }
    }

	void swap(unsigned n0, unsigned n1) {
		iter_swap(main_list.begin() + n0, main_list.begin() + n1);
    }

	void showtask(unsigned n) {
		cout << main_list.at(n);
    }
};

class list_collection {
private:
	SERIAL_ACCESS;
	template <class Archive>
	void serialize(Archive& ar, const unsigned version) {
		ar & collection;
		ar & history;
    }

	vector<todo_list> collection;
	vector<todo_item> history;

public:
	todo_list& at(unsigned num) {
		return collection.at(num);
    }

	void add(string name, string desc) {
		todo_list tmp;
		tmp.name = name;
		tmp.desc = desc;
		collection.push_back(tmp);	
    }

	void del(unsigned num) {
		cout << "Are you sure you want to delete category #" << num << "? ";
		char resp;
		cin >> resp;
		if (resp != 'y')
			return;	

		collection.erase(collection.begin() + num);
    }

	void hist_add(todo_item i) {
		history.push_back(i);
    }

	void clear_hist() {
		vector<todo_item> blank;
		history = blank;
    }

	void show(string which = "main") {
		unsigned cnt = 0;
		if (which == "main") {
			for (auto list: collection) {
				cout << cnt << ". " << list.name << endl;
				++cnt;
			}
		}

		else if (which == "history") {
			for (auto task: history) {
				cout << cnt << ". " << task.name << endl;
				++cnt;
			}
        }
    }

	void swap(unsigned n0, unsigned n1) {
		iter_swap(collection.begin() + n0, collection.begin() + n1);
    }
};

#endif // LISTS_H
