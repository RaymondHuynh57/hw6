#ifndef RECCHECK
#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <random>
#include <iomanip>
#include <fstream>
#include <exception>
#endif

#include "boggle.h"

std::vector<std::vector<char> > genBoard(unsigned int n, int seed)
{
	//random number generator
	std::mt19937 r(seed);

	//scrabble letter frequencies
	//A-9, B-2, C-2, D-4, E-12, F-2, G-3, H-2, I-9, J-1, K-1, L-4, M-2, 
	//N-6, O-8, P-2, Q-1, R-6, S-4, T-6, U-4, V-2, W-2, X-1, Y-2, Z-1
	int freq[26] = {9,2,2,4,12,2,3,2,9,1,1,4,2,6,8,2,1,6,4,6,4,2,2,1,2,1};
	std::vector<char> letters;
	for(char c='A'; c<='Z';c++)
	{
		for(int i=0;i<freq[c-'A'];i++)
		{
			letters.push_back(c);
		}
	}
	std::vector<std::vector<char> > board(n);
	for(unsigned int i=0;i<n;i++)
	{
		board[i].resize(n);
		for(unsigned  int j=0;j<n;j++)
		{
			board[i][j] = letters[(r() % letters.size())];
		}
	}
	return board;
}

void printBoard(const std::vector<std::vector<char> >& board)
{
	unsigned int n = board.size();
	for(unsigned int i=0;i<n;i++)
	{
		for(unsigned int j=0;j<n;j++)
		{
			std::cout << std::setw(2) << board[i][j];
		}
		std::cout << std::endl;
	}
}

std::pair<std::set<std::string>, std::set<std::string> > parseDict(std::string fname)
{
	std::ifstream dictfs(fname.c_str());
	if(dictfs.fail())
	{
		throw std::invalid_argument("unable to open dictionary file");
	} 
	std::set<std::string> dict;
	std::set<std::string> prefix;
	std::string word;
	while(dictfs >> word)
	{
		dict.insert(word);
		for(unsigned int i=word.size()-1;i>=1;i--)
		{
			prefix.insert(word.substr(0,i));
		}
	}
	prefix.insert("");
	return make_pair(dict, prefix);
}

std::set<std::string> boggle(const std::set<std::string>& dict, const std::set<std::string>& prefix, const std::vector<std::vector<char> >& board)
{
	std::set<std::string> result;
	for(unsigned int i=0;i<board.size();i++)
	{
		for(unsigned int j=0;j<board.size();j++)
		{
			boggleHelper(dict, prefix, board, "", result, i, j, 0, 1);  //This will go right
			boggleHelper(dict, prefix, board, "", result, i, j, 1, 0);  //This will go down
			boggleHelper(dict, prefix, board, "", result, i, j, 1, 1);  //This will go bottom right diagonal
      //std::cout << "------------------------------------------------------------>" << std::endl;
		}
	}
	
	return result;
}

bool boggleHelper(const std::set<std::string>& dict, const std::set<std::string>& prefix, const std::vector<std::vector<char> >& board, 
								   std::string word, std::set<std::string>& result, unsigned int r, unsigned int c, int dr, int dc)
{
//add your solution here!
    //You 
    //std::cout << "BOGGLEHELPER" << std::endl;
    //Need a way to know which direction to go depending on the dr and dc. dr and dc should remain as 0's and 1's since they are just int
    //One Recursion will go specifically right all the way, One Recursion will go specifically down all the way, and One recursion will go specifcally right diagonal all the way. There are no in between change of directions
    //This first recursion will go specifically right for now
    //1) For the one that goes specifically right, then it won't go to the next row since the loops from the other function does it already
    //2) For the one that goes specifically down, then it won't go do the next column
    //3) For the one that goes specifically diagonal right, then it won't go to the next diagonal right that is on the right of the current diagonal right
    //std::cout << r << " " << c << std::endl;
    //If the word continue being made is part of a prefix, then keep recursing
    //If the word continue being made is not part of a prefix, then return false
//----------------------------------------------------------------> //This goes right when dr = 0 and dr = 1

  if(dr == 0 && dc == 1){
        if(c == board[r].size()){ //If the recursion reaches the end of the column
            //std::cout << "LEFT TO RIGHT word END: " << word << std::endl;
            //if(word.substr(0, word.size() - 1) == "AWE"){
                    //std::cout << "THE BASE CASE WORD IS: " << word << std::endl;
            //}
            if(dict.find(word) != dict.end()){  //If we find the word in the dictionary
                
                result.insert(word);
                //std::cout << "RETURN TRUE: " << word << std::endl;

              //Maybe if dict.find(word.substr(0, word.size() - 1) != dict.end())
                std::string lesser_string = word.substr(0, word.size() - 1);
                if(result.find(lesser_string) != result.end()){ //Detect the one less size with prefix. Might have to do this in the main recursion other than the base case as well
                    //std::cout << "LESSER_STRING: " << lesser_string << std::endl;
                }
                return true;
            }
            return false;
        }else{  //Recurse left to right on the same row
            //std::cout << "LEFT TO RIGHT: " << r << " " << c << std::endl;
            //std::cout << "LEFT TO RIGHT word: " << word << std::endl;
            //if(word == "AWEE"){
                //std::cout << "LEFT TO RIGHT word: " << word << std::endl;
            //}

            
            
             
            if(boggleHelper(dict, prefix, board, word + board[r][c], result, r, c + 1, dr, dc)){ //If dr is 0 and dc is 1
                //Might have to do stuff here
                //std::cout << "THIS IS TRUE YO: " << word << std::endl;
                return true;
            }else{  //If they can't find the "largest word" in the dict set in the base case, then we need to see if the word in this recursion is the "largest word"
                  //if(){
                 //std::cout << "THIS IS FALSE: " << word + board[r][c] << std::endl;
                
                 if(dict.find(word) != dict.end()){ //If we are able to find this word after the "largest word" failing, this would be the next largest word
                    result.insert(word);
                    return true;
                 }
                  //}
            }

            if(prefix.find(word) == prefix.end()){  //If we can't find a prefix, return false
                //std::cout << "NOT PREFIX: " << word << std::endl;
                return false;
            }

            return false;
        }
    }
//----------------------------------------------------------------> //This goes down only
  else if(dr == 1 && dc == 0){
        if(r == board.size()){ //If the recursion reaches the the row after the last row
            //std::cout << "UP TO DOWN word END: " << word << std::endl;
            if(dict.find(word) != dict.end()){  //If we find the word in the dictionary
                  result.insert(word);
                  return true;
              }
            return false;
        }else{  //Recurse up to down on the same column
            //std::cout << "UP TO DOWN: " << r << " " << c << std::endl;
            //std::cout << "UP TO DOWN word: " << word << std::endl;
           
            
            if(boggleHelper(dict, prefix, board, word + board[r][c], result, r + 1, c, dr, dc)){ //If dr is 1 and dc is 0
                return true;
            }else{  //If they can't find the "largest word" in the dict set in the base case, then we need to see if the word in this recursion is the "largest word"
                  //if(){
                 //std::cout << "THIS IS FALSE: " << word + board[r][c] << std::endl;
                 if(dict.find(word) != dict.end()){ //If we are able to find this word after the "largest word" failing, this would be the next largest word
                    result.insert(word);
                    return true;
                 }
                  //}
            }

            if(prefix.find(word) == prefix.end()){  //If we can't find a prefix, return false
                //std::cout << "NOT PREFIX: " << word << std::endl;
                return false;
            }
            return false;
        }
  }
  //--------------------------------------------------------------> //This goes right down diagonal only
  else if(dr == 1 && dc == 1){
        if(r == board.size() || c == board[r].size()){  //If the recursion reaches the diagonal outside the grid
            //std::cout << "DIAGONAL BOTTOM RIGHT TRAVEL word END: " << word << std::endl;
            if(dict.find(word) != dict.end()){  //If we find the word in the dictionary
                result.insert(word);
                return true;
            }
            return false;
        }else{  //Recurse from current index to the bottom right
            //std::cout << "DIAGONAL BOTTOM RIGHT TRAVEL: " << r << " " << c << std::endl;
            //std::cout << "DIAGONAL BOTTOM RIGHT TRAVEL word: " << word << std::endl;
            
            if(boggleHelper(dict, prefix, board, word + board[r][c], result, r + 1, c + 1, dr, dc)){ //If dr is 1 and dc is 1

                return true;
            }else{  //If they can't find the "largest word" in the dict set in the base case, then we need to see if the word in this recursion is the "largest word"
                  //if(){
                 //std::cout << "THIS IS FALSE: " << word + board[r][c] << std::endl;
                 if(dict.find(word) != dict.end()){ //If we are able to find this word after the "largest word" failing, this would be the next largest word
                    result.insert(word);
                    return true;
                 }
                  //}
            }

            if(prefix.find(word) == prefix.end()){  //If we can't find a prefix, return false
                //std::cout << "NOT PREFIX: " << word << std::endl;
                return false;
            }
            return false;
        }
  }
  

  /*
  std::set<std::string>::iterator it;
  for(it = dict.begin(); it != dict.end(); ++it){
      std::cout << "DICT: " << *it << std::endl;
  }
  */
 // for()
}
