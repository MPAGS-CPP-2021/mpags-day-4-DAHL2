#include "PlayfairCipher.hpp"

#include <algorithm>
#include <iostream>
#include <string>

PlayfairCipher::PlayfairCipher(const std::string& key)
{
    this->setKey(key);
}

void PlayfairCipher::setKey(const std::string& key)
{
    // store the original key
    key_ = key;
    
    // Append the alphabet
    key_ += alphabet_;

    // Make sure the key is upper case
    // For info on why ::toupper rather than std::toupper, see:
    // https://stackoverflow.com/questions/7131858/stdtransform-and-toupper-nomatching-function
    std::transform(key_.begin(), key_.end(), key_.begin(), ::toupper);

    // Remove non-alpha characters
    key_.erase(std::remove_if(key_.begin(), key_.end(),
               [] (char c)
                  { return !( std::isalpha(c) ); }     ),
        key_.end());

    // Change J -> I
    auto jToI = [] (const char c) {
        if (c == 'J')
            return 'I';
        else
            return c;
    };
    std::transform(key_.begin(), key_.end(), key_.begin(), jToI);

    // Remove duplicated letters
    std::string foundCharacters {""};

    // Lambda to test whether character already exists
    auto remDup = [&] (char c) {
        if (foundCharacters.find(c) == std::string::npos) {
            foundCharacters += c;
            return false;
        } else
            return true;
    };

    key_.erase(std::remove_if(key_.begin(), key_.end(), remDup),
        key_.end());

    // Store the coords of each letter
    
    // Store the playfair cihper key map
}

std::string PlayfairCipher::applyCipher(const std::string& inputText,
                                      const CipherMode cipherMode) const
{
    // Change J → I

    // If repeated chars in a digraph add an X or Q if XX

    // if the size of input is odd, add a trailing Z

    // Loop over the input in Digraphs

    // - Find the coords in the grid for each digraph
    // - Apply the rules to these coords to get 'new' coords
    // - Find the letter associated with the new coords
    // return the text
    if (cipherMode == CipherMode::Encrypt)  // TEMPORARY
    {
    return inputText;
    } else {return inputText;}
}
