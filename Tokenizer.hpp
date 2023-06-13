/*
 * These functions are used to split up a given string into sections
 * that we can use to make the users. 
*/
#ifndef PROJECT1_TOKENIZER_HPP
#define PROJECT1_TOKENIZER_HPP
#include <string>
#include <vector>

enum TokenType{
    DEFAULT, //no token ever has this type
    OPERATOR,
    STRING

};

static const char *TokenTypeString[] = {
        "DEFAULT",
        "OPERATOR",
        "STRING"

};


class Token {
public:
    enum TokenType type{DEFAULT};
    std::string text;
    void debugPrint() const;

};

class Tokenizer{
public:
    std::vector<Token> parse(const std::string &inProgram);

private:
    void endToken(Token &token, std::vector<Token> &tokens);

};


#endif //PROJECT1_TOKENIZER_HPP
