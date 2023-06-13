/*
 * These functions are the manipulation of the data, that allows us to create
 * the HTML files and make sure the data is in order.
*/
#ifndef PROJECT1_USER_HPP
#define PROJECT1_USER_HPP
#include <vector>
#include "Tokenizer.hpp"

class User {
public:
    //parse puts the tokens in order in a 2d array/vector that will allow for
    // easy access for the other functions.
    static std::vector<std::vector<std::string>> parse(
                                                std::vector<Token> &tokens);

    //these are functions used for making the html pages,
    // createFollowerMatrix() is a helper function as well as createHtmlUser()
    static void createHtmlProfiles(
                            std::vector<std::vector<std::string>>&userMatrix);

    static void createIndexHtml(
                            std::vector<std::vector<std::string>> userMatrix);

    //debug tools
    static void printUsers(
                    const std::vector<std::vector<std::string>>& userMatrix);

private:
//    std::vector<std::string> individualUser;
//    int userId;

    static void matrixMerge(std::vector<std::vector<std::string>> &userMatrix,
                            int begin, int end, int mid);
    static void matrixSort(std::vector<std::vector<std::string>> &userMatrix,
                           int begin, int end);

    static std::vector<std::vector<bool>> createFollowerMatrix(
            const std::vector<std::vector<std::string>>& userMatrix);

    static void createHtmlUser(std::vector<std::vector<std::string>> userMatrix,
                               std::vector<std::string> individualUser,
                               std::vector<std::vector<bool>> following,
                               int user);

};

#endif //PROJECT1_USER_HPP
