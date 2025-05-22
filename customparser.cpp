// Complete me
#include "customparser.h"
#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

const char* error_msg_1 = "Cannot read integer n";
const char* error_msg_2 = "Error in content parsing";
const char* error_msg_3 = "Error in user parsing";

// To Do - Complete this function
void CustomFormatParser::parse(std::istream& is, std::vector<Content*>& content, std::vector<User*>& users)
{
    // Erase any old contents
    content.clear();
    users.clear();

    
    // TO DO - Add your code below.
    //Reads the number of a content items and user accountsl, what is parse is doing is essentially 
    //parsing content from the format input stream 
    //
    int num;
    is >> num; 
    if (is.fail()) {
      throw ParserError(error_msg_1);
    }
    for (int i=0; i < num; i++) {
      Content *c;
      int id; 
      int type; 
      std::string name;
      int numReviews_;
      int totalStars_; 
      int ratings; 
      int episodes; 
      std::string temp; 
      is >> id >> type;
      if (is.fail()) {
        throw ParserError(error_msg_2);
      }
      std::getline(is, name);
      std::getline(is, name);
      if (is.fail()) {
        throw ParserError(error_msg_2);
      }
      name = name.substr(1);  // eliminates leading tab char
      is >> numReviews_;
      is >> totalStars_;
      if (is.fail()) {
        throw ParserError(error_msg_2);
      }
      //cout << totalStars_ << endl; 
      is >> ratings; 
      if (is.fail()) {
        throw ParserError(error_msg_2);
      }
      if (type == 1) {
        is >> episodes; 
        c = new Series(id, name, numReviews_, totalStars_, ratings, episodes);
      }
      else {
        c = new Movie(id, name, numReviews_, totalStars_, ratings); 
      }
      //cout << episodes << endl; 
      getline(is, temp);
      getline(is, temp);
      if (is.fail()) {
        throw ParserError(error_msg_2);
      }
      stringstream ss(temp);
      std::string user;
      while (ss >> user) {
        c->addViewer(user);
        if (is.fail()) {
          throw ParserError(error_msg_2);
        }
      } 
  
      content.push_back(c);
    }

    //Add users
      std::string username;
      int ratingL; 
      std::string temp; 

      while(is >> username) {
        is >> ratingL;
        if (is.fail()) {
          throw ParserError(error_msg_3);
        }
        User* u = new User(username, ratingL);
        getline(is, temp); // clears out linefeed
        getline(is, temp);
         if (is.fail()) {
          throw ParserError(error_msg_3);
        }

        stringstream ss(temp);
        int read;
      while (ss >> read) {
        u->addToHistory(read);
      } 
      users.push_back(u);    
      }
}
