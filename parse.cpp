#ifndef PARSE_H
#define PARSE_H
#include "parse.h"
#endif

#define dollar -1000
#define epsilon -1
using namespace std;

Parsing_table::Parsing_table(map<int, set<vector<int> > >& g, vector<int>& k) 
{
	grammar = g;
	keys = k;
	follow_changes = true;	//fr 1st iteration it shd b initialized 2 true
}

void Parsing_table::construct_first_set()
{
	/*map<int, set<vector<int> > >::iterator map_iter;
	set<vector<int> >::iterator set_iter;
	for (map_iter = grammar.begin(); map_iter != grammar.end(); ++map_iter){

		for(set_iter = (*map_iter).begin(); set_iter != (*map_iter).end(); ++set_iter){
			//*set_iter is a vecor of int
			for (int i = 0; i < (*set_iter).size(); ++i){

			}
		}
	}*/
	for (int i = 0; i < keys.size(); ++i){
		if (!is_first_set_constructed(keys[i])){
			construct_first_of(keys[i]);
		}
	}
}
void Parsing_table::construct_first_of(int item)
{
	if (item < 0){
		first[item].insert(item);
		return;
	}
	//grammar[item] is a set of vectors
	pending.insert(item);
	set<vector<int> >::iterator set_iter;
	for(set_iter = grammar[item].begin(); set_iter != grammar[item].end(); ++set_iter){
		//*set_iter is a vecor of int
		if (is_present_in_pending((*set_iter)[0])) {
			cout << "Grammar has epsilon transition.Left Recursion cannot be removed.\n";
			exit(1);
		}
		if (!is_first_set_constructed((*set_iter)[0])){
			construct_first_of((*set_iter)[0]);
		}
		add_first_to_first((*set_iter)[0], item);
		
	}
	pending.erase(item);
}
bool Parsing_table::is_first_set_constructed(int item)
{
	if (first.count(item) > 0) return true;
	else return false;
}
bool Parsing_table::is_present_in_pending(int item)
{
	if (pending.count(item) > 0) return true;
	else return false;
}
void Parsing_table::add_first_to_first(int item1, int item2)
{
	//first[item1] is a set of integers
	set<int >::iterator iter;
	for(iter = first[item1].begin(); iter != first[item1].end(); ++iter){
		//*iter is an int
		first[item2].insert(*iter);
	}
}
void Parsing_table::construct_follow_set()
{
	add_element_to_follow(dollar, keys[0]);	//assumed dat keys[0] is d start symbol

	//if sm non-terminal has an epsilon production, I must hav already calculated first[epsilon]. I will not hav calculated
	//first[epsilon] only if dere is no epsilon production
	set<vector<int> >::iterator set_iter;
	while (follow_changes){
		follow_changes = false;

		for (int i = 0; i < keys.size(); ++i){
			//grammar[i] is a set of vectors of ints
			for(set_iter = grammar[i].begin(); set_iter != grammar[i].end(); ++set_iter){
			//*set_iter is a vecor of int
				vector<int> temp = *set_iter;
				int j = temp.size() - 1;
				if (temp[j] >= 0)	//i.e it is a non-terminal
					add_follow_to_follow(temp[i],temp[j]);
				--j;
				bool flag = true;
				for (; j >= 0; --j){
					if (temp[j] >= 0){
						add_first_to_follow(temp[j + 1], temp[j]);
						if (!derives_element(epsilon, temp[j + 1])) flag = false;
						if (flag){
							add_follow_to_follow(temp[i],temp[j]);
						}
					}
				}
			}
		}
	}
}
void Parsing_table::add_element_to_follow(int element, int item)
{
	if (follow[item].count(element) > 0) return;
	follow[item].insert(element);
	follow_changes = true;
}
void Parsing_table::add_first_to_follow(int item1, int item2)
{
	if (item1 < 0){
		if (follow[item2].count(item1) > 0) return;
		follow[item2].insert(item1);
		follow_changes = true;
	}
	set<int >::iterator iter;
	for(iter = first[item1].begin(); iter != first[item1].end(); ++iter){
		//*iter is an int
		if (follow[item2].count(*iter) > 0) follow_changes = true;
		else follow[item2].insert(*iter);
	}
}
void Parsing_table::add_follow_to_follow(int item1, int item2)
{
	set<int >::iterator iter;
	for(iter = follow[item1].begin(); iter != follow[item1].end(); ++iter){
		//*iter is an int
		if (follow[item2].count(*iter) > 0) follow_changes = true;
		else follow[item2].insert(*iter);
	}
}
bool Parsing_table::derives_element(int element, int item)
{
	if (first[item].count(element) > 0) return true;
	else return false;
}
