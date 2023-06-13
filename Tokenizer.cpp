/*
 * These functions 
*/
#include "Tokenizer.hpp"
#include <string>
#include <iostream>

// This function takes in the string, that has all the contents of the input
// file and gives each element or set of elements a type and groups them if
// they're a string.
std::vector<Token> Tokenizer::parse(const std::string &inProgram) {
    std::vector<Token> tokens;
    Token currentToken;
    for(char currCh : inProgram){
        switch(currCh){
            case '{':
            case '}':
            case '[':
            case ']':
            case ':':
            case ',':
                if(currentToken.type != STRING){
                    endToken(currentToken, tokens);
                    currentToken.type = OPERATOR;
                    currentToken.text.append(1, currCh);
                    endToken(currentToken, tokens);
                }
                else{
                    currentToken.text.append(1, currCh);
                }
                break;

            case ' ':
                if(currentToken.type == STRING){
                    currentToken.text.append(1, currCh);
                }
                else{
                    endToken(currentToken, tokens);
                }
                break;

            case '\t':
            case '\n':
                endToken(currentToken, tokens);
                break;

            case '"':
                if(currentToken.type != STRING){
                    endToken(currentToken, tokens);
                    currentToken.type = STRING;
                }
                else if(currentToken.type == STRING){
                    endToken(currentToken, tokens);
                }
                break;

            default:
                currentToken.text.append(1, currCh);
                break;

        }
    }
    endToken(currentToken,tokens);

    return tokens;

}

// This makes it easy to get rid of tokens that arent useful to me, such
// as whitespace
void Tokenizer::endToken(Token &token, std::vector<Token> &tokens) {
    if(token.type != DEFAULT){
        tokens.push_back(token);
    }
    token.type = DEFAULT;
    token.text.erase();

}

// debug tool: Prints the token with it's token type to make sure it's the
// right type.
void Token::debugPrint() const{
    std::cout << "Token(" << TokenTypeString[type] << ", \"" << text << "\")"  << std::endl;

}
