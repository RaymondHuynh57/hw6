#ifndef HASH_H
#define HASH_H


#include <iostream>
#include <cmath>
#include <random>
#include <chrono>


#include <ctime>
#include <cstdlib>


typedef std::size_t HASH_INDEX_T;


struct MyStringHash {
    HASH_INDEX_T rValues[5] { 983132572, 1468777056, 552714139, 984953261, 261934300 };
    MyStringHash(bool debug = true)
    {
        if(false == debug){
            generateRValues();
        }
    }




 


    // hash function entry point (i.e. this is h(k))
    HASH_INDEX_T operator()(const std::string& k) const
    {
        // Add your code here
        //std::cout << "HASH INDEX OPERATOR" << std::endl;
        //std::cout << "INPUT: " << k << std::endl;
        //Only do 6 letters for each w[index]
      std::string w_index_string = "";
      size_t beginning_index = 0;
      size_t end_index = k.size() - 1;
      unsigned long long w_array[5]; //This is to hold w[0], w[1], w[2], w[3], w[4]
      std::string w_array_string[5];
      unsigned long long result = 0;
      for(size_t i = 0; i < 5; ++i){  //Make the array hold all zeroes for now
          w_array[i] = 0;
          //w_array_string[i] = "aaaaaa"; //Make it six letters
          w_array_string[i].resize(6);
      }
      //size_t w_index_end = 4; //Will be used for the index of w_array
      std::string temp_k = k; //This variable will hold the same exact value of k and will be used in the loop
      //std::string temp_k = "YOMAMAISSOCOOLILIKEYOUDUDE"; //This variable will hold the same exact value of k and will be used in the loop
      for(int i = 4; i >= 0; --i){
          //Make a helper function that will add 6 to each w[index] backwards
          //Need to pass a difference parameter to the helper function
          Add_Six_To_W_Index(w_array_string[i], temp_k);
          w_array[i] = Base_To_Decimal_Converter(w_array_string[i]);
      }


      for(size_t i = 0; i < 5; ++i){  //Now we add them all up with the random values being multiplied as well
          result += (rValues[i] * w_array[i]);
      }


      for(size_t i = 0; i < 5; ++i){
          std::cout << "w[" << i << "]: " << w_array[i] << std::endl;
      }
      return result;
      //Now we need to make a base to decimal 36 conversion
 
       
     
  /*
        std::cout << "SHOW ARRAY STRING" << std::endl;
        for(size_t i = 0; i < 5; ++i){
            std::cout << w_array_string[i] << std::endl;
        }


        std::cout << "LETTER TEST" << std::endl;
        size_t number_result = letterDigitToNumber('9');
        std::cout << "NUMBER RESULT: " << number_result << std::endl;
  */
    }




   void Add_Six_To_W_Index(std::string& w_string, std::string& temp_k) const{  //This will add 6 characters to each w_array_string


      if(temp_k.size() <= 6){ //If the size of temp_k is less than or equal to 6, than just add what is in temp_k to w_string[4]
         
          for(size_t i = 0; i < 6; ++i){
              if(i < temp_k.size()){  //If i is less than temp_k size, that means we are still adding
                  w_string[i] = temp_k[i];
              }else{
                 
                  w_string[i] = char(0);  //Send in empty char
              }
             
          }
          temp_k = "";
      }else{  //If the size of temp_k is greater than 6
          //std::cout << "GREATER THAN 6" << std::endl;
          size_t w_string_end_index = 5;
          for(size_t i = temp_k.size() - 1; i >= temp_k.size() - 6; --i){  //Loop 6 times so that the loop will not loop for the entire temp_k. Also, start at the end since we are going backwards
              w_string[w_string_end_index] = temp_k[i];
              --w_string_end_index;
          }
          temp_k = temp_k.substr(0, temp_k.size() - 6);
      }


   }
   unsigned long long Base_To_Decimal_Converter(const std::string& w_index_string/*, unsigned long long w_array[]*/) const{ //Converts Base to Decimal 36
        //std::cout << "BASE TO DECIMAL CONVERTER" << std::endl;
        unsigned long long result = 0;
        unsigned long long b = 1;
        for(int i = w_index_string.size() - 1; i >= 0; --i){ //Loop 6 times because the word has a length of 6
            if(w_index_string[i] == char(0)){ //This detects empty spaces
                //std::cout << "THESE ARE EMPTY" << std::endl;
                //++i;
                continue;
            }
            //w_array[i] = 1;
            //std::cout << "b: " << b << std::endl;
            //std::cout << "w index string: " << letterDigitToNumber(w_index_string[i]) << std::endl;
            //std::cout << "VALUES: " << (letterDigitToNumber(w_index_string[i])*b) << std::endl;
            result += (letterDigitToNumber(w_index_string[i]) * b);
            b *= 36;
        }
        return result;
    }
    // A likely helper function is to convert a-z,0-9 to an integral value 0-35
    HASH_INDEX_T letterDigitToNumber(char letter) const
    {
        // Add code here or delete this helper function if you do not want it
        //std::cout << "LETTERDIGITTONUMBER" << std::endl;
       
        size_t number_result = 0;
        if(letter >= 97 && letter <= 122){  //This is used to detect lowercase alphabets
            //std::cout << "LETTER: " << (size_t)(letter) << std::endl;
            return (size_t)(letter) - 97;
        }else if(letter >= 65 && letter <= 90){ //This is used to detect uppercase alphabets
            return (size_t)(letter) - 65;
        }
        else if(letter >= 48 && letter <= 57){ //Detects 0 to 9
            //std::cout << "NUMBER: " << (size_t)(letter) << std::endl;
            return (size_t)(letter) - 22;
        }
    }


   


    // Code to generate the random R values
    void generateRValues()
    {
        // obtain a seed from the system clock:
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::mt19937 generator (seed);  // mt19937 is a standard random number generator


        // Simply call generator() [it has an operator()] to get another random number
        for(int i{ 0 }; i < 5; ++i)
        {
            rValues[i] = generator();
        }
    }
};


#endif





