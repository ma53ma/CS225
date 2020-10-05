/**
 * @file cartalk_puzzle.cpp
 * Holds the function which solves a CarTalk puzzler.
 *
 * @author Matt Joras
 * @date Winter 2013
 */

#include <fstream>
#include <iostream>

#include "cartalk_puzzle.h"

using namespace std;

/**
 * Solves the CarTalk puzzler described here:
 * http://www.cartalk.com/content/wordplay-anyone.
 * @return A vector of (string, string, string) tuples
 * Returns an empty vector if no solutions are found.
 * @param d The PronounceDict to be used to solve the puzzle.
 * @param word_list_fname The filename of the word list to be used.
 */
vector<std::tuple<std::string, std::string, std::string>> cartalk_puzzle(PronounceDict d,
                                    const string& word_list_fname)
{

    //vector<string> words = file_to_vector(word_list_fname);

    vector<std::tuple<std::string, std::string, std::string>> ret;

    ifstream wordsFile(word_list_fname);
    string word;
    vector<string> vectorWords;
    if (wordsFile.is_open()) {
      while(getline(wordsFile, word)) {
        if (word.length() == 5) {
          vectorWords.push_back(word);
        }
      }
    }

    for (string str : vectorWords) {
      string string2 = str.substr(1);
      string string3 = str.substr(0,1) + str.substr(2);
      if (d.homophones(str, string2) && d.homophones(str, string3)) {
        tuple<string, string, string> tup = {str, string2, string3};
        ret.push_back(tup);
      }
    }


    /* Your code goes here! */
    return ret;
}
