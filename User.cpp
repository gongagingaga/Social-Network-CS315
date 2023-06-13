/*
 * These functions are used for creating the sets of users in a matrix.
 * Included in these is also the functions that make the HTML files. 
*/
#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>
#include "User.hpp"

std::vector<std::vector<std::string>> User::parse(std::vector<Token> &tokens) {
    int size = tokens.size();

    //this is going to hold all the information for our html function to make
    // the required html files.
    std::vector<std::vector<std::string>> userMatrix;

    for (int i = 0; i < size; i++) {
        std::string name;
        std::string idHtml;
        std::string location;
        std::string id_str = "0";
        std::string pic_url = "https://upload.wikimedia.org/wikipedia/commons/a/ac/Default_pfp.jpg";

        std::vector<std::string> individualUser = {id_str, idHtml, name,
                                                   location, pic_url};

        bool userExists = false;

        while (tokens[i].text != "}") {
            i++;
            if (tokens[i].text == "id_str") {
                //if there is an id_str then there must be a user, therefore:
                userExists = true;

                //erases the current token with "id_str" as the text,
                // so that we don't iterate through this exact user twice
                tokens.erase(tokens.begin() + i);

                //everytime i erase a token the list gets smaller,
                // so i need to shrink how many times loop iterates with it.
                size--;

                //since we deleted the element we were on, "id_str", we are now
                // looking at ":" in which case we only have to just a since
                // token to get to the data we are looking for.
                i++;

                //we need to make sure that we have a userId in the list,
                // as the first item for simplicity's sake.
                individualUser[0] = tokens[i].text;

                //we also need to put it in a varible for later use
                id_str = tokens[i].text;

                //this makes the userid into "user[id_str].html" so we can
                // use that later in the linking process
                std::string userId = "user" + tokens[i].text + ".html";

                //we push back the "user[id_str].html
                individualUser[1] = userId;

                //delete the id_str just in case, even though we should never
                // run into it again
                tokens.erase(tokens.begin() + i);

                size--;
            }
            if (tokens[i].text == "name") {
                tokens.erase(tokens.begin() + i);

                size--;

                i++;

                individualUser[2] = tokens[i].text;

                tokens.erase(tokens.begin() + i);

                size--;
            }
            if (tokens[i].text == "location") {
                tokens.erase(tokens.begin() + i);

                size--;

                i++;

                location = " in " + tokens[i].text;

                individualUser[3] = location;

                tokens.erase(tokens.begin() + i);

                size--;
            }

            if (tokens[i].text == "pic_url") {
                tokens.erase(tokens.begin() + i);

                size--;

                i++;

                individualUser[4] = tokens[i].text;

                tokens.erase(tokens.begin() + i);

                size--;
            }

            if (tokens[i].text == "follows") {
                tokens.erase(tokens.begin() + i);

                size--;

                i++;

                // The list of followers is terminated by a square bracket.
                while(tokens[i].text != "]"){
                    // if this STRING(not an operator(i.e., ",")) then we can
		    // add it to the list. 
                    if(tokens[i].type == STRING){
                        individualUser.push_back(tokens[i].text);

                        tokens.erase(tokens.begin() + i);

                        size--;

                        i--;
                    }
                    i++;
                }
            }
        }
        //now that we have all of a single user's information we are
        //able to hand it back to the matrix.
        if(userExists) {
            userMatrix.push_back(individualUser);
        }
        tokens.erase(tokens.begin()+1);

        size--;
    }

    //the list is sorted here using merge sort. O(nlog(n)) worst case scenario.
    matrixSort(userMatrix, 0, userMatrix.size() - 1);
    return userMatrix;
}

//this creates the html profiles and calls two helper functions.
void User::createHtmlProfiles(std::vector<std::vector<std::string>> &userMatrix) {
    // Create a matrix of bools containing the follows
    std::vector<std::vector<bool>> followerMatrix = createFollowerMatrix(userMatrix);

    for(int user = 0; user < userMatrix.size(); user++){
        // Create the html pages for every individual user
        createHtmlUser(userMatrix, userMatrix[user], followerMatrix, user);
    }
}

std::vector<std::vector<bool>> User::createFollowerMatrix(
                   const std::vector<std::vector<std::string>>& userMatrix) {
    // This just initializes a 2d matrix full of 0's, it'll always be a square
    // so I make use of userMatrix.size() twice since userMatrix.size()
    // is also the number of users.
    std::vector<std::vector<bool>> followerMatrix(userMatrix.size(),
                                                  std::vector<bool>
                                                 (userMatrix.size(),
                                                  false));

    for(int i = 0; i < userMatrix.size(); i++){

        // We look at the 5th position since the way our individual users are
        // created they have either, different information taking up the first
        // 4 spots, or blank spaces representing those elements. Hence, the
        // 5 spot and onward is the people the user is following.
        for(int j = 5; j < userMatrix[i].size(); j++){
            if(!userMatrix[i][j].empty()){
                std::string follower = userMatrix[i][j];

                // Since our individualUsers are vectors of strings, we can't
                // just use them as is and have to convert them to ints.
                int followerInt = std::stoi(follower);

                // Make that value,(i.e., user1: follower 3, would make
                // the 3rd value in the first user's 3rd spot not false.
                followerMatrix[i][followerInt - 1] = true;
            }
        }
    }
    return followerMatrix;
}

void User::createHtmlUser(std::vector<std::vector<std::string>> userMatrix,
                          std::vector<std::string> individualUser,
                          std::vector<std::vector<bool>> following,
                          int user) {

    std::filebuf fb;
    fb.open(individualUser[1], std::ios::out);
    std::ostream userOut(&fb);

    // The first part of every userHtml is the same with different user
    // information.
    userOut << "<!DOCTYPE html>\n<html>\n<head>\n<title>" + individualUser[2]
               + " Profile</title>\n</head>\n<body>\n<h1>" + individualUser[2]
               + individualUser[3] + "</h1>\n<img alt=\"Profile pic\" src=\""
               + individualUser[4] + "\"/>\n<h2>Following</h2>\n<ul>\n";

    // This is what allows us to store the "follows" part of the matrix
    // to later be compared with the "followers" to find mutuals.
    std::vector<int> userX;

    // Using the follower matrix youll be able to know who our user is following
    // since we can look at i(user)j(following)
    bool isFollowing = false;

    for(int i = 0; i < following.size(); i++){
        if (following[user][i]){
            isFollowing = true;

            userOut << "<li><a href=\"user"
                        + std::to_string(i+1)
                        + ".html" "\">"
                        + userMatrix[i][2]
                        + "</a></li>\n";;

            userX.push_back(i);
        }
    }

    if(!isFollowing){
        userOut << "<li>Not Following anyone!</li>\n";
    }

    userOut << "</ul>\n<h2>Followers</h2>\n<ul>\n";

    // This is storing the "followers"
    std::vector<int> userY;

    bool hasFollowers = false;

    for(int i = 0; i < following.size(); i++){
        if (following[i][user]){
            hasFollowers = true;

            userOut << "<li><a href=\""
                       + userMatrix[i][1]
                       + "\">" + userMatrix[i][2]
                       + "</a></li>\n";;

            userY.push_back(i);
        }
    }

    if(!hasFollowers){
        userOut << "<li>No Followers!</li>\n";
    }

    // This section compares the userX and userY and if there are any
    // matches we make them a mutual
    userOut << "</ul>\n<h2>Mutuals</h2>\n<ul>\n";

    bool hasMutual = false;

    if(!userX.empty() || !userY.empty()){
        for(int i : userX){
            for(int j : userY){
                if(i == j){
                    hasMutual = true;

                    std::string mutual = "<li><a href=\""+ userMatrix[i][1]
                                        + "\">"
                                        + userMatrix[i][2]
                                        + "</a></li>\n";

                    userOut << mutual;
                }
            }
        }
    }
    if(!hasMutual){
        userOut << "<li>No Mutuals!</li>\n";
    }

    userOut << "</ul>\n</body>\n</html>" << std::endl;

    fb.close();
}

void User::createIndexHtml(std::vector<std::vector<std::string>> userMatrix) {
    std::filebuf fb;
    fb.open("userIndex.html", std::ios::out);
    std::ostream userOut(&fb);

    userOut << "<!DOCTYPE html>\n<html>\n<head>\n<title>My Social Network"
               "</title>\n</head>\n<body>\n"
               "<h1>My Social Network: User List</h1>\n<ol>\n";

    for(auto & individualUser : userMatrix){
        userOut << "<li><a href=\"" + individualUser[1] + "\">" + individualUser[2] + "</a></li>\n";
    }
    userOut << "</ol>\n</body>\n</html>";
}

//debug tool: is used to print the users in the userMatrix as well as the data
void User::printUsers(const std::vector<std::vector<std::string>>& userMatrix) {
    for(const auto& individualUser : userMatrix){
        for(auto & j : individualUser){
            std::cout << j;
        }
        std::cout << std::endl;
    }
}

// These following functions are a merge sort that works with our userMatrix
// and compares the id_str.
void User::matrixMerge(std::vector<std::vector<std::string>> &userMatrix,  int const left, int const mid,
                      int const right) {
    // Merges two subarrays of userMatrix[].
    // First subarray is arr[begin..mid]
    // Second subarray is arr[mid+1..end]

    auto const subArrayOne = mid - left + 1;
    auto const subArrayTwo = right - mid;

    // Create temp arrays
    std::vector<std::vector<std::string>> leftArray;
    std::vector<std::vector<std::string>> rightArray;

    // Copy data to temp arrays leftArray[] and rightArray[]
    for (auto i = 0; i < subArrayOne; i++)
        leftArray.push_back(userMatrix[left + i]);
    for (auto j = 0; j < subArrayTwo; j++)
        rightArray.push_back(userMatrix[mid + 1 + j]);

    auto indexOfSubArrayOne
            = 0, // Initial index of first sub-userMatrix
    indexOfSubArrayTwo
            = 0; // Initial index of second sub-userMatrix
    int indexOfMergedArray
            = left; // Initial index of merged userMatrix

    // Merge the temp arrays back into userMatrix[left..right]
    while (indexOfSubArrayOne < subArrayOne
           && indexOfSubArrayTwo < subArrayTwo) {
        if (std::stoi(leftArray[indexOfSubArrayOne][0])
            <= std::stoi(rightArray[indexOfSubArrayTwo][0])) {
            userMatrix[indexOfMergedArray]
                    = leftArray[indexOfSubArrayOne];
            indexOfSubArrayOne++;
        }
        else {
            userMatrix[indexOfMergedArray]
                    = rightArray[indexOfSubArrayTwo];
            indexOfSubArrayTwo++;
        }
        indexOfMergedArray++;
    }
    // Copy the remaining elements of
    // left[], if there are any
    while (indexOfSubArrayOne < subArrayOne) {
        userMatrix[indexOfMergedArray]
                = leftArray[indexOfSubArrayOne];
        indexOfSubArrayOne++;
        indexOfMergedArray++;
    }
    // Copy the remaining elements of
    // right[], if there are any
    while (indexOfSubArrayTwo < subArrayTwo) {
        userMatrix[indexOfMergedArray]
                = rightArray[indexOfSubArrayTwo];
        indexOfSubArrayTwo++;
        indexOfMergedArray++;
    }
}

// begin is for left index and end is
// right index of the sub-userMatrix
// of arr to be sorted 
void User::matrixSort(std::vector<std::vector<std::string>> &userMatrix, int const begin, int const end)
{
    if (begin >= end)
        return; // Returns recursively

    auto mid = begin + (end - begin) / 2;
    matrixSort (userMatrix, begin, mid);
    matrixSort(userMatrix, mid + 1, end);
    matrixMerge(userMatrix, begin, mid, end);
}
