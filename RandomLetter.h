#ifndef RANDOMLETTER_H
#define RANDOMLETTER_H

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <random>
#include <ctime>
#include <algorithm>

#define MAX_LETTER 16
#define MIN_LETTER 3

//RandomLetter Class
class RandomLetter{
    public:
        RandomLetter(std::string& filename); //filename
        ~RandomLetter();
        void randomlt(std::vector<std::string>& vector); //check if vec is completed(16 member) ,if not then add it up
        void readfile(std::string& filename);

        //check
        std::vector<std::string> getdict();
        
    private:
        std::vector<std::string> dictionary;

        // random variable
        int random_vowel_order = 0;
        std::mt19937 gen;
};

#endif