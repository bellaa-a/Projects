//Bella Lu
//xlu62

#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <iostream>
#include <tuple>
#include <vector>
#include <algorithm>
#include "language_model.h"

using std::tuple; using std::make_tuple;
using std::get;
using std::cout; using std::endl;
using std::string;
using std::vector;
using std::sort; using std::greater;

void print_trigram(vector<trigram> trigram_list) {
    for (vector<trigram>::iterator it = trigram_list.begin(); it != trigram_list.end(); it++) {
        cout << get<3>(*it) << " - [" << get<0>(*it) << " " << get<1>(*it) << " " << get<2>(*it) << "]" << endl;
    }
}

void handle_a_command(vector<trigram> trigram_list) {
    sort(trigram_list.begin(), trigram_list.end());
    print_trigram(trigram_list);
}

bool greater_than(trigram trigram1, trigram trigram2) {
    return trigram1 > trigram2;
}

void handle_d_command(vector<trigram> trigram_list) {
    sort(trigram_list.begin(), trigram_list.end(), greater_than);
    print_trigram(trigram_list);
}

bool larger_count(trigram trigram1, trigram trigram2) {
    return get<3>(trigram1) > get<3>(trigram2);
}

void handle_c_command(vector<trigram> trigram_list) {
    //Sorts trigram_list by decreasing frequency but dones't account for sorting by ASCII value when ties are encountered.
    sort(trigram_list.begin(), trigram_list.end(), larger_count);

    //Sorts trigram_list by ASCII value when their frequencies are tied.
    int start_index = 0; //Where a particular frequency starts.
    int len = 1; //The amount of trigram that has that particular frequency. 
    for (int i = 0; i < (int)trigram_list.size() - 1; i++) {
        if (i == (int)trigram_list.size() - 2) { //If the end of trigram_list has been reached. 
            sort(trigram_list.begin() + start_index, trigram_list.begin() + start_index + len + 1); //Sorts the last chunk.
        } else if (get<3>(trigram_list[i]) == get<3>(trigram_list[i+1])) { //If frequency tied with next trigram.
            len++; //Number of consecutive ties increments by one.
        } else { //Frequency differs from next trigram. 
            sort(trigram_list.begin() + start_index, trigram_list.begin() + start_index + len); //Sorts that chunk. 
            start_index += len; //Starting index of next frequency. 
            len = 1; //Resets amount back to one.
        }
    }
    print_trigram(trigram_list);
}

void handle_f_command(vector<trigram> trigram_list, string word1, string word2) {
    int highest = 0;
    int index = 0;
    bool found = false;
    sort(trigram_list.begin(), trigram_list.end());
    for (int i = 0; i < (int)trigram_list.size(); i++) {
        if (get<0>(trigram_list[i]) == word1 
        && get<1>(trigram_list[i]) == word2) {
            if (get<3>(trigram_list[i]) > highest) {
                highest = get<3>(trigram_list[i]);
                index = i;
            }
            found = true;
        }
    }
    if (found) {
        cout << get<3>(trigram_list[index]) << " - [" << get<0>(trigram_list[index]) << " " << get<1>(trigram_list[index]) << " " << get<2>(trigram_list[index]) << "]" << endl;
    } else {
        cout << "No trigrams of the form [" << word1 << " " << word2 << " ?]" << endl;
    }
}
