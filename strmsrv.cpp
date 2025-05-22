#include "strmsrv.h"
#include <iostream>
#include <fstream>
using namespace std;

// To do - Complete this function
StreamService::StreamService()

{
    // Update as needed
    cUser_ = NULL; 
}

// To do - Complete this function
StreamService::~StreamService()
{
  for (size_t i =0; i < users_.size(); i++) {
    delete users_[i];
  } 

  for (size_t i =0; i < content_.size(); i++) {
    delete content_[i];
  } 
}

// Complete - Do not alter
void StreamService::readAndParseData(std::istream& is, Parser& p)
{
    p.parse(is, content_, users_);
    cout << "Read " << content_.size() << " content items." << endl;
    cout << "Read " << users_.size() << " users." << endl;
}

// To do - Complete this function
/**
     * @brief Sets the current user to the given username
     * 
     * @param uname username that will be the current user
     * @throw runtime_error if another user is already logged in (i.e. they
     *                      should logout first)
     * @throw invalid_argument if the username is not valid
     */
void StreamService::userLogin(const std::string& uname)
{
  if (cUser_ != NULL) {
    throw std::runtime_error("Another User is already logged in");
  }
  for (size_t i=0; i < users_.size(); i++) {
    if (users_[i]->uname == uname) {
      cUser_ = users_[i];
      return; 
    }
  }
  throw std::invalid_argument("Username is not valid"); 
}

// To do - Complete this function
void StreamService::userLogout()
{
  cUser_ = NULL; 
}

// To do - Complete this function
std::vector<CID_T> StreamService::searchContent(const std::string& partial) const
{
    std::vector<CID_T> results;
    for(size_t i = 0; i < content_.size(); i++){
        // TO DO - modify this to also push back when the string 'partial'
        //  is contained in the name of the current content. Lookup the string
        //  class documentation to find an appropriate function that can be used
        //  to do this simply.
        if(partial == "*" || content_[i]->name().find(partial) != std::string::npos) {

            results.push_back(i);
        }        
    }
    return results;
}

// Complete - Do not alter
std::vector<CID_T> StreamService::getUserHistory() const
{
    throwIfNoCurrentUser();
    return cUser_->history;
}

// To do - Complete this function
/**
     * @brief Updates data structures to indicate the current User has watched
     *        this content
     * 
     * @param contentID ID of the content to watch
     * @throw UserNotLoggedInError if no User is currently logged in.
     * @throw range_error      if the contentID is not valid
     * @throw RatingLimitError if the content's rating is above the User's 
     *                         rating limit
     */
void StreamService::watch(CID_T contentID)
{
  throwIfNoCurrentUser();
  if (!isValidContentID(contentID)) {
    throw std::range_error("ContentID is not valid");
  }
  
  if (content_[contentID]->rating() > cUser_->ratingLimit) {
    throw RatingLimitError("Rating is above the USer's rating limit");
  }
  cUser_->addToHistory(contentID); 
  content_[contentID]->addViewer(cUser_->uname); 
}

  /**
     * @brief Add a review with the given number of stars to the specified
     *        content. We will NOT worry about a user reviewing content
     *        more than once.
     * 
     * @param contentID ContentID to review
     * @param numStars Number of stars for the review (must be 0-5)
     * @throw UserNotLoggedInError if no User is currently logged in.
     * @throw ReviewRangeError if the contentID is not valid or the 
     *                    number of stars is out of the range 0-5
     */
// To do - Complete this function
void StreamService::reviewShow(CID_T contentID, int numStars)
{
    throwIfNoCurrentUser();
   if (numStars < 0 || numStars > 5) {
    throw ReviewRangeError("ContentID is not valid");
  }
  if (!isValidContentID(contentID)) {
    throw ReviewRangeError("ContentID is not valid");
  }
  content_[contentID]->review(numStars); 
}

// To do - Complete this function
    /**
     * @brief Returns the ID of the chosen content suggestion
     *  
     * For the given content, Cp, and current user, Uq, consider the set of 
     * Users, Uj, (Uj /= Uq) who also watched content Cp and find the single, 
     * content item, Ck, (Ck /= Cp) that was viewed the most times by users
     * in the set, Uj.
     * 
     * @param contentID ID of the content for which we want suggested similar
     *                  content
     * @return CID_T of the suggestion.
     * @throw UserNotLoggedInError if no User is currently logged in.
     * @throw range_error      if the contentID is not valid
     */
CID_T StreamService::suggestBestSimilarContent(CID_T contentID) const
{
    // Change this when you are ready, but for now it is a 
    // dummy implementation
    throwIfNoCurrentUser(); //user login 

    //make sure contentID is valid 
    if(!isValidContentID(contentID)) {
      throw std::range_error("ContentID is not valid");
    }
    cout << contentID << endl; 
    //list of all usernames who watched the content 
    std::vector<std::string> totViewers = content_[contentID]->getViewers();

    //vector to count how many times whos watched the content with contentID
    std::vector<int> viwCnt(content_.size());

    // Go through each name in totViewers
    for (std::string viewerName : totViewers) { 
    
      // Find the corresponding user object
      for (User *u : users_) {
        if (u->uname == viewerName) {
          cout << viewerName << endl; 
          // Find all of the media items that the user watched
          for (CID_T watched : u->history) {
            cout << watched << endl; 
            // Increment the corresponding indices in viwCnt vector

            viwCnt[watched]++;
            // Might need an additional layer to link CID_T to index in viwCnt XXXXX
          }
          break;

        }

      }

    }
    cout << "[ ";
    for (int n : viwCnt) {
      cout << n << " ";
    }
    cout << "]\n";
    
    // At this point, viwCnt should contain view counts where the index corresponds to CID_T ?
    /*
    //loop through every user in the system 
    for (int i=0; i < users_.size(); i++) { //looping through each user in the system 
    //checks to see if user is in totViewers 
    if (users_[i]->uname == totViewers[j])
    std::string temp_uname = users_[i]->uname;
      auto it = std::find(totViewers.begin(), totViewers.end(), temp_uname); //string vector find 
     
     //looping though totViewers to match usernames 
      for (int j= 0; i < totViewers.size(); i++) { 
        //if user is in totViewers 
        if (totViewers[i] == users_[i]->uname) {
          //make sure the find function works 
          if (it != totViewers.end()) {
        std::vector<CID_T> temp = users_[i]->history; 
        //make a temp to increment the view count for the content using history 
        for (int j=0; j < temp.size(); j++) {
          viwCnt[temp[j]]++; 
        }
      }
    }
      }
    }
    */
    int max =0; 
    int maxViews = -1; 
    for (unsigned int i=0; i < viwCnt.size(); i++) {
      if (i == contentID || cUser_->haveWatched(i)) {
        continue; 
      }
      if (viwCnt[i] > max) {
        max = viwCnt[i];
        maxViews = i; 
      }
    }
    return maxViews; 

    //return -1;
}

// To do - Complete this function
void StreamService::displayContentInfo(CID_T contentID) const
{
    // Do not alter this
    if(! isValidContentID(contentID)){
        throw std::invalid_argument("Watch: invalid contentID");
    }
    // Call the display abitlity of the appropriate content object
    content_[contentID]->display(std::cout); 
}

// Complete - Do not alter
bool StreamService::isValidContentID(CID_T contentID) const
{
    return (contentID >= 0) && (contentID < (int) content_.size());
}

// Complete - Do not alter
void StreamService::throwIfNoCurrentUser() const
{
    if(cUser_ == NULL){
        throw UserNotLoggedInError("No user is logged in");
    }
}

// Complete - Do not alter
int StreamService::getUserIndexByName(const std::string& uname) const
{
    for(size_t i = 0; i < users_.size(); i++){
        if(uname == users_[i]->uname) {
            return (int)i;
        }
    }
    return -1;
}
