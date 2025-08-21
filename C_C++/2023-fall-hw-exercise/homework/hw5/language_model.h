//Bella Lu
//xlu62

#ifndef LANGUAGE_MODEL_H
#define LANGUAGE_MODEL_H

#include <tuple>
#include <string>

using std::tuple;
using std::string;
using std::vector;

//A trigram here is a tuple consisting of the three words and its frequency as the fourth argument. 
typedef tuple<string, string, string, int> trigram;

void print_trigram(vector<trigram> trigram_list);
void handle_a_command(vector<trigram> trigram_list);
bool greater_than(trigram trigram1, trigram trigram2);
void handle_d_command(vector<trigram> trigram_list);
bool larger_count(trigram trigram1, trigram trigram2);
void handle_c_command(vector<trigram> trigram_list);
void handle_f_command(vector<trigram> trigram_list, string word1, string word2);

#endif