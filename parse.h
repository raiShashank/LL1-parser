#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <iostream>
#include <set>
#include <map>
#include <vector>

//element refers 2 d mapped integer i.e id may b mapped to 3 and '+' mignt b mapped to -2, these ints r element

class Parsing_table
{
private:
	bool follow_changes;
	std::set <int> pending;
	std::map<int, std::set<std::vector<int> > > grammar;//non-left-recursive nd left-factored grammar, will get it frm shyamal
	std::vector<int> keys;
	std::map <int, std::set<int> > first;	//first[i] is a set of int. it contains all items in first[i] including epsilon
	std::map <int, std::set<int> > follow;	//it contains all items in follow[i], no epsilon, may include $

	void construct_first_of(int item);		//called 2 construct first set of a particular item
	void construct_follow_of(int item);
	bool is_present_in_first_of(int element,int item);	//returns true if element is present in first[item]
	bool is_first_set_constructed(int item);	//return true if item is a key in first
	bool is_present_in_pending(int item);		//returns true if d first set of item is in recursion

	bool derives_element(int element, int item);	//returns true if item derives element in 0 or more productions
	void add_first_to_first(int item1, int item2);	//adds first[item1] to first[item2]
	void add_first_to_follow(int item1, int item2);	//adds first[item1] to follow[item2]
	void add_follow_to_follow(int item1, int item2);//adds follow[item1] to follow[item2]
	void add_element_to_follow(int element, int item);	//adds element to follow[item]
public:
	Parsing_table(std::map<int, std::set<std::vector<int> > >& g, std::vector<int>& k);
	void construct_first_set();		//generic fn called 2 construct first set of all items(terminal as well as non-terminal)
	void construct_follow_set();
	std::set<int> get_first_of(int item) { return first[item]; }	//returns first[item]
	std::set<int> get_follow_of(int item) { return follow[item]; };	//returns follow[item]

};