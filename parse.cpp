#ifndef PARSE_H
#define PARSE_H
#include "parse.h"
#endif

#define dollar -1
#define epsilon -7 	//in dis particular example
#define num_terminals 7 	//smhow I need 2 know dis, don't count epsilon in num_terminals
using namespace std;

Parsing_table::Parsing_table(map<int, set<vector<int> > >& g, vector<int>& k, map<int, std::string>& m) 
{
	grammar = g;

	keys = k;
		
	follow_changes = true;	//fr 1st iteration it shd b initialized 2 true
	
	int_to_string = m;
	table = new vector<int> *[keys.size()];
	for (int i = 0; i < keys.size(); ++i){
		table[i] = new vector<int>[num_terminals];
	}
	
}
Parsing_table::~Parsing_table()
{
	for (int i = 0; i < keys.size(); ++i){
		delete[] table[i];
	}
	delete[] table;
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
	for (int i = 0; i < keys.size(); ++i){			//since in grammar d keys r only non-terminals, same in keys
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
	int asd;
	add_element_to_follow(dollar, keys[0]);	//assumed dat keys[0] is d start symbol

	//if sm non-terminal has an epsilon production, I must hav already calculated first[epsilon]. I will not hav calculated
	//first[epsilon] only if dere is no epsilon production
	set<vector<int> >::iterator set_iter;
	while (follow_changes){
		follow_changes = false;

		for (int i = 0; i < keys.size(); ++i){
			//grammar[i] is a set of vectors of ints
			for(set_iter = grammar[keys[i]].begin(); set_iter != grammar[keys[i]].end(); ++set_iter){
			//*set_iter is a vecor of int
				vector<int> temp = *set_iter;
				int j = temp.size() - 1;
				if (temp[j] >= 0)	//i.e it is a non-terminal
					add_follow_to_follow(keys[i],temp[j]);
				--j;
				bool flag = true;
				for (; j >= 0; --j){
					if (temp[j] >= 0){
						add_first_to_follow(temp[j + 1], temp[j]);
						if (!derives_element(epsilon, temp[j + 1])) flag = false;
						if (flag){
							add_follow_to_follow(keys[i],temp[j]);
						}
					}
				}
			}
		}
	}
}
void Parsing_table::print_follow()
{
	set<int> s;
	set<int >::iterator iter;
	for (int i = 0; i < keys.size(); ++i){
		s = get_follow_of(keys[i]);
		cout << "Follow of '" << int_to_string[i] << "' : ";
		for(iter = s.begin(); iter != s.end(); ++iter){
			cout << int_to_string[*iter] << " ";
		}
		cout << "\n";
	}
}
void Parsing_table::print_first()
{
	set<int> s;
	set<int >::iterator iter;
	for (int i = 0; i < keys.size(); ++i){
		s = get_first_of(keys[i]);
		cout << "First of '" << int_to_string[i] << "' : ";
		for(iter = s.begin(); iter != s.end(); ++iter){
			cout << int_to_string[*iter] << " ";
		}
		cout << "\n";
	}
}
void Parsing_table::add_element_to_follow(int element, int item)
{
	if (element == epsilon) return;
	if (follow[item].count(element) > 0) return;
	follow[item].insert(element);
	follow_changes = true;
}
void Parsing_table::add_first_to_follow(int item1, int item2)
{
	if (item1 < 0){
		if (item1 == epsilon) return;
		if (follow[item2].count(item1) > 0) return;
		follow[item2].insert(item1);
		follow_changes = true;
	}
	set<int >::iterator iter;
	for(iter = first[item1].begin(); iter != first[item1].end(); ++iter){
		//*iter is an int
		if (*iter == epsilon) continue;
		if (follow[item2].count(*iter) == 0) { 
			follow_changes = true;
			follow[item2].insert(*iter);
		}
	}
}
void Parsing_table::add_follow_to_follow(int item1, int item2)
{
	set<int >::iterator iter;
	for(iter = follow[item1].begin(); iter != follow[item1].end(); ++iter){
		//*iter is an int
		if (follow[item2].count(*iter) == 0) { 
			follow_changes = true;
			follow[item2].insert(*iter);
		}
	}
}
bool Parsing_table::derives_element(int element, int item)
{
	if (first[item].count(element) > 0) return true;
	else return false;
}
void Parsing_table::construct_table()
{
	//dis memory allocation will b done in constructor
	//each cell of parsing table contains a production (in r case a vector<int>)
	/*table = new vector<int> *[keys.size()];
	for (int i = 0; i < keys.size(); ++i){
		table[i] = new vector<int>[num_terminals];
	}*/
	set<vector<int> >::iterator set_iter;
	for (int i = 0; i < keys.size(); ++i){
		//grammar[keys[i]] is a set of vectors, actually a set of productions
		int item = keys[i];		//item refers 2 d non-terminal whose productions v r currently examining
		for(set_iter = grammar[item].begin(); set_iter != grammar[item].end(); ++set_iter){
			//*set_iter is a vecor of int, the current production
			vector<int> temp = *set_iter;
			//temp[0] is an int. first[temp[0]] is a set of integers, actually a set of terminals
			set<int >::iterator iter;
			for(iter = first[temp[0]].begin(); iter != first[temp[0]].end(); ++iter){
				//*iter is an int
				if (table[item][*iter + num_terminals].size() > 0){
					if (table[item][*iter + num_terminals] != temp) {
						cerr << "First\n";
						cerr << "There is a conflict in 2 productions corresponding 2 '" << int_to_string[item] << "'.hence\
						 grammar is not LL1\n";
						 exit(1);
					}
				}
				table[item][*iter + num_terminals] = temp;
			}
			//r table will also hav table[item][epsilon] = epsilon though v'll never use it, it can b removed
			if (first[temp[0]].count(epsilon) > 0){
				for(iter = follow[item].begin(); iter != follow[item].end(); ++iter){
				//*iter is an int
					if (table[item][*iter + num_terminals].size() > 0){
						if (table[item][*iter + num_terminals] != temp) {
							cerr << "Follow" << int_to_string[item] << " " << int_to_string[*iter] << "\n";
							print_production(item, *iter + num_terminals);
							cout << "\n";
							cerr << "There is a conflict in 2 productions corresponding 2 '" << int_to_string[item] << "'.hence\
							 grammar is not LL1\n";
							 exit(1);
						}
					}
					table[item][*iter + num_terminals] = temp;
				}
			}
		}
		cout << int_to_string[keys[i]] << "\n";
	}
}
void Parsing_table::print_production(int i, int j)
{
	vector<int> temp = table[i][j];
	for (int k = 0; k < temp.size(); ++k){
		cout << int_to_string[temp[k]];
	}
}
void Parsing_table::print_table()
{
	cout << "\t";
	for (int i = num_terminals; i > 0; --i){
		cout << int_to_string[-i] << "\t";
	}
	cout << "\n";
	for (int i = 0; i < keys.size(); ++i){
		cout << int_to_string[keys[i]] << "\t";
		for (int j = 0; j < num_terminals; ++j){
			//table[i][j] is a vecor of int
			print_production(i, j);
			cout << "\t";
		}
		cout << "\n";
	}
}
int main()
{
	map<int, string> m;
	m[0] = "E";
	m[1] = "Ep";
	m[2] = "T";
	m[3] = "Tp";
	m[4] = "F";
	m[-1] = "$";
	m[-2] = ")";
	m[-3] = "(";
	m[-4] = "*";
	m[-5] = "+";
	m[-6] = "id";
	m[-7] = "eps";
	int arr[] = {0,1,2,3,4};
	vector<int> keys(arr, arr + 5);
	map<int, set<vector<int> > > g;
	/*char c = 'y',d = 'n';
	int hash, production;
	while(d!= '!'){
		cin >> hash;
		std::vector<char> v;<int> k;
		//c = getchar();
		cin >> production;
		while (c != '\n'){
			k.push_back(c - '0');
			c = getchar();
		}
		g[hash].insert(k);
		d = getchar();
	}*/
	vector<int> k;

	k.push_back(2);
	k.push_back(1);
	g[0].insert(k);
	k.clear();

	k.push_back(-5);
	k.push_back(2);
	k.push_back(1);
	g[1].insert(k);
	k.clear();

	k.push_back(-7);
	g[1].insert(k);
	k.clear();

	k.push_back(4);
	k.push_back(3);
	g[2].insert(k);
	k.clear();

	k.push_back(-4);
	k.push_back(4);
	k.push_back(3);
	g[3].insert(k);
	k.clear();

	k.push_back(-7);
	g[3].insert(k);
	k.clear();

	k.push_back(-3);
	k.push_back(0);
	k.push_back(-2);
	g[4].insert(k);
	k.clear();

	k.push_back(-6);
	g[4].insert(k);
	k.clear();

	Parsing_table p(g, keys, m);
	p.construct_first_set();
	p.print_first();
	
	cout << "----------------------------------------\n";
	p.construct_follow_set();
	p.print_follow();
	cout << "___________________________________________\n";
	p.construct_table();
	cout << "----------------------------------------\n";
	p.print_table();
	return 0;
}