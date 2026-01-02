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
using std::ifstream;
using std::cerr;

int main (int argc, char *argv[]) {
    //Error handling: command line arguments have to include ./hw5, file name, and command.
    if (argc < 3) { 
        cerr << "Invalid command: valid options are a, d, c, and f" << endl;
        return 1;
    } 
    bool found = false;
    int count = 0;

    vector<trigram> trigram_list;
    ifstream infile(argv[1]);

    //Error handling: file from command line cannot be opened.
    if (!infile.is_open()) {
        cerr << "Invalid file list: " << argv[1] << endl;
        return 1;
    }

    //Temporary tring to hold the text file names 
    string file;

    //A vector a strings to hold the text file names stored in the file provided by the command line argument.
    vector<string> filenames;

    while (infile >> file) { //Read from file until it ends.
    
        //Adds file into the vector of strings called filenames. 
        filenames.push_back(file); 

        //Opens file for reading.
        ifstream text(file);

        //Error handling: continues to print whichever file that cannot be opened. 
        if (!text.is_open()) {
            cerr << "Invalid file: " << file << endl;
            count++; //Records number of files that cannot be opened. 
        } else {
            //A vector storing all the words from each text file, with start 1 and 2 as the begining two 
            //arguments and end 1 and 2 as the last two. 
            vector<string> words;
            words.push_back("<START_1>");
            words.push_back("<START_2>");
            string word;
            while (text >> word) {
                words.push_back(word);
            }
            words.push_back("<END_1>");
            words.push_back("<END_2>");

            for (int i = 0; i < (int)words.size() - 2; i++) {
                //Found resets to false everytime another three words can be made. 
                found = false;

                //Temporary trigram stores next three words, then keeps or disgards depending on if trigram already exists or not.
                trigram temp_trigram = make_tuple(words[i], words[i+1], words[i+2], 1);

                for (int j = 0; j < (int)trigram_list.size(); j++) {
                    //Compare if the three words are the same, increments the frequency if they are. 
                    if (get<0>(trigram_list[j]) == get<0>(temp_trigram) 
                    && get<1>(trigram_list[j]) == get<1>(temp_trigram) 
                    && get<2>(trigram_list[j]) == get<2>(temp_trigram)) {
                        get<3>(trigram_list[j])++;
                        found = true; //Occurance found. 
                    }
                }
                //If nothing mathces, then create new trigram. 
                if (!found) {
                    trigram_list.push_back(temp_trigram);
                }
            }

        }
    }
    //Error handling: if all of the files cannot be opened, exit program. 
    if (count == (int)filenames.size()) {
        return 1;
    }
    if (*argv[2] == 'a') {
        handle_a_command(trigram_list); 
    } else if (*argv[2] == 'd') {
        handle_d_command(trigram_list);
    } else if (*argv[2] == 'c') {
        handle_c_command(trigram_list);
    } else if (*argv[2] == 'f') {
        if (argc != 5) {
            //Error handling: there must be exactly two command line arguments after f.
            cerr << "Invalid argument list: f requires two additional command-line arguments" << endl;
            return 1;
        }
        handle_f_command(trigram_list, argv[3], argv[4]);
    } else { 
        //Error handling: user did not enter a valid command.
        cerr << "Invalid command: valid options are a, d, c, and f" << endl;
        return 1;
    }
    return 0;
}