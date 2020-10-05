/**
 * @file common_words.cpp
 * Implementation of the CommonWords class.
 *
 * @author Zach Widder
 * @date Fall 2014
 */

#include "common_words.h"

#include <fstream>
#include <string>
#include <vector>
#include <iostream>
#include <iterator>
#include <algorithm>

using std::string;
using std::vector;
using std::ifstream;
using std::cout;
using std::endl;
using std::feof;

string remove_punct(const string& str)
{
    string ret;
    std::remove_copy_if(str.begin(), str.end(), std::back_inserter(ret),
                        std::ptr_fun<int, int>(&std::ispunct));
    return ret;
}

CommonWords::CommonWords(const vector<string>& filenames)
{
    // initialize all member variables
    init_file_word_maps(filenames);
    init_common();
}

void CommonWords::init_file_word_maps(const vector<string>& filenames)
{
    fileNames_ = filenames;
    // make the length of file_word_maps the same as the length of filenames
    file_word_maps.resize(filenames.size());

    // go through all files
    for (size_t i = 0; i < filenames.size(); i++) {
        // get the corresponding vector of words that represents the current
        // file
        vector<string> words = file_to_vector(filenames[i]);
        for (string str : words) {
          map<string, unsigned int>::iterator lookup = file_word_maps[i].find(str);
          if (lookup != file_word_maps[i].end()) {
            lookup->second = lookup->second + 1;
          } else {
            file_word_maps[i].insert(std::pair<string, unsigned int>(str, 1));
          }
        }
    }
}

void CommonWords::init_common()
{
    /* Your code goes here! */
    /*
    vector<string> totalWords;

    for (size_t i = 0; i < filenames.size(); i++) {
      vector<string> words = file_to_vector(filenames[i]);
      totalWords.insert(totalWords.end(), words.begin(), words.end());
    }
    */
    map<string, bool> repeat;
    for (size_t i = 0; i < fileNames_.size(); i++) {
      vector<string> words = file_to_vector(fileNames_[i]);
      for (string str : words) {
        map<string, bool>::iterator lookup = repeat.find(str);
        map<string, unsigned int>::iterator comLookup = common.find(str);

        if (lookup == repeat.end()) {
          repeat.insert(std::pair<string, bool>(str, false));
          if (comLookup != common.end()) {
            comLookup->second = comLookup->second + 1;
          } else {
            common.insert(std::pair<string, int>(str, 1));
          }
        }
      }
      repeat.clear();
    }
}

/**
 * @param n The number of times to word has to appear.
 * @return A vector of strings. The vector contains all words that appear
 * in each file >= n times.
 */
vector<string> CommonWords::get_common_words(unsigned int n) const
{
    bool nTimes = true;
    vector<string> out;

    for (std::pair<string, unsigned int> pair : common) {
      if (pair.second == fileNames_.size()) {
        for (size_t i = 0; i < file_word_maps.size(); i++) {
          map<string, unsigned int> mappy = file_word_maps[i];
          map<string,unsigned int>::iterator lookup = mappy.find(pair.first);
          if (lookup->second < n) {
            nTimes = false;
            break;
          }
        }
        if (nTimes) {
          out.push_back(pair.first);
        } else {
          nTimes = true;
        }
      }
    }
    /* Your code goes here! */
    return out;
}

/**
 * Takes a filename and transforms it to a vector of all words in that file.
 * @param filename The name of the file that will fill the vector
 */
vector<string> CommonWords::file_to_vector(const string& filename) const
{
    ifstream words(filename);
    vector<string> out;

    if (words.is_open()) {
        std::istream_iterator<string> word_iter(words);
        while (!words.eof()) {
            out.push_back(remove_punct(*word_iter));
            ++word_iter;
        }
    }
    return out;
}
