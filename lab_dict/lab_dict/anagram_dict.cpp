/**
 * @file anagram_dict.cpp
 * Implementation of the AnagramDict class.
 *
 * @author Matt Joras
 * @date Winter 2013
 */

#include "anagram_dict.h"

#include <algorithm> /* I wonder why this is included... */
#include <fstream>
#include<iostream>

using std::string;
using std::vector;
using std::ifstream;

using std::cout;
using std::endl;

/**
 * Constructs an AnagramDict from a filename with newline-separated
 * words.
 * @param filename The name of the word list file.
 */
AnagramDict::AnagramDict(const string& filename)
{
    /* Your code goes here! */
    ifstream wordsFile(filename);
    string word;
    vector<string> vectorWords;
    if (wordsFile.is_open()) {
      while(getline(wordsFile, word)) {
        vectorWords.push_back(word);
      }
    }
    for (string str : vectorWords) {
      string word = str;
      std::sort(str.begin(), str.end());
      std::map<std::string, std::vector<std::string>>::iterator it = dict.find(str);
      if (it != dict.end()) {
        (it->second).push_back(word);
      } else {
        dict[str] = {word};
      }
    }
}

/**
 * Constructs an AnagramDict from a vector of words.
 * @param words The vector of strings to be used as source words.
 */
AnagramDict::AnagramDict(const vector<string>& words)
{
    /* Your code goes here! */
    for (string str : words) {
      string word = str;
      std::sort(str.begin(), str.end());
      cout << "sorted word is " << str << endl;
      std::map<std::string, std::vector<std::string>>::iterator it = dict.find(str);
      if (it != dict.end()) {
        (it->second).push_back(word);
        cout << "lists is: ";
        for (string str : it->second) {
          cout << str << " ";
        }
        cout << " " << endl;
      } else {
        dict[str] = {word};
      }
    }
}

/**
 * @param word The word to find anagrams of.
 * @return A vector of strings of anagrams of the given word. Empty
 * vector returned if no anagrams are found or the word is not in the
 * word list.
 */
vector<string> AnagramDict::get_anagrams(const string& word) const
{
    /* Your code goes here! */
    vector<string> empty;
    empty.clear();
    string wordSort = word;
    std::sort(wordSort.begin(), wordSort.end());
    std::map<std::string, std::vector<std::string>>::const_iterator it = dict.find(wordSort);
    if (it != dict.end()) {
      return it->second;
    } else {
      return empty;
    }
}

/**
 * @return A vector of vectors of strings. Each inner vector contains
 * the "anagram siblings", i.e. words that are anagrams of one another.
 * NOTE: It is impossible to have one of these vectors have less than
 * two elements, i.e. words with no anagrams are ommitted.
 */
vector<vector<string>> AnagramDict::get_all_anagrams() const
{
    /* Your code goes here! */
    vector<vector<string>> toVect;
    std::map<std::string, std::vector<std::string>>::const_iterator it;
    for (it = dict.begin(); it != dict.end(); it++) {
      if ((it->second).size() > 1) {
        toVect.push_back(it->second);
      }
    }
    return toVect;

}
