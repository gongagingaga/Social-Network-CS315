/*
 * Benito Sanchez
 * CS315
 * Professor Rivoire 
 *
 * This program takes a specifically formatted file similar to that of 
 * JSON format, and outputs users profiles and an index containing the list of 
 * users. Both types of output are in HTML format. 
*/
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "Tokenizer.hpp"
#include "User.hpp"

int main() {
    std::ifstream input("input.json");
    std::string fullInput;
    while (!input.eof()) {
        std::string tempInput;
        getline(input, tempInput);
        fullInput += tempInput;

    }

    Tokenizer tokenizer;
    std::vector<Token> tokens = tokenizer.parse(fullInput);

//    this part prints the token list
//    for(const Token& currToken : tokens){
//        currToken.debugPrint();
//    }

    //creates parsed users as a 2d array/vector.
    std::vector<std::vector<std::string>> userMatrix = User::parse(tokens);

    //takes the information from the parsed lads and lasses and puts everything in html format.
    User::createHtmlProfiles(userMatrix);

    //create userIndex html
    User::createIndexHtml(userMatrix);
    return 0;
}
