#include "RandomLetter.h"

//RandomLetter Constructor responsible for initializing an instance of the RandomLetter class by reading data from a file and setting up a random number generator.
RandomLetter::RandomLetter(std::string& filename) {
    readfile(filename);
    gen = std::mt19937(static_cast<unsigned int>(std::time(0))); // Initialize the random number generator 'gen' here.
}

RandomLetter::~RandomLetter(){};//RandomLetter destructer responsible for cleaning up any resources allocated by the object before its memory is freed.

std::vector<std::string> RandomLetter::getdict(){return dictionary;}//RandomLetter fuction responsible for getting the vecter of the file.

//RandomLetter function responsible for getting 16 random letter with at least 2 vowel.
void RandomLetter::randomlt(std::vector<std::string>& vector){
    std::uniform_int_distribution<> dis('a', 'z');
    std::vector<char> vowel{'a','e','i','o','u'};

    //Keep track of the number of vowel in the vecter.
    int vowel_counter = 0;
    for (auto& letter:vector){
        if(letter == "a" ||letter == "e"||letter == "i"||letter == "o"||letter == "u"){
            vowel_counter++;
        }
    }

    //Push in random letters and vowel into the vecter.
    for (auto& letter : vector) {
        if (letter.empty()) {
            char randomChar = static_cast<char>(dis(gen));
            if ((vowel_counter<2)){letter.push_back(vowel[random_vowel_order%5]);
                random_vowel_order++;
                vowel_counter++;}
            else{letter.push_back(randomChar);}
        }
    }
};

//RandomLetter Function responsible for opening the file.
void RandomLetter::readfile(std::string& filename){
std::vector<std::string> dict;
  std::fstream textfile(filename);
    if (textfile.is_open()){
      std::string line;
      while(getline(textfile,line)){
        if (line.size() <= MAX_LETTER && line.size() >= MIN_LETTER){
        dict.push_back(line);
        }
      }
    }
  dictionary = dict;
}
