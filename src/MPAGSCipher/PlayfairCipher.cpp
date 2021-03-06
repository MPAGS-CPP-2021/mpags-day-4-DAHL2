#include "PlayfairCipher.hpp"

#include <algorithm>
#include <iostream>
#include <map>
#include <string>

PlayfairCipher::PlayfairCipher(const std::string& key)
{
    this->setKey(key);
}

std::string PlayfairCipher::convertItoJ(const std::string& input)
{
    auto jToI = [](const char c) {
        if (c == 'J')
            return 'I';
        else
            return c;
    };
    std::string output{input};
    std::transform(input.begin(), input.end(), output.begin(), jToI);
    return output;
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
                              [](char c) { return !(std::isalpha(c)); }),
               key_.end());

    // Change J -> I
    key_ = convertItoJ(key_);

    // Remove duplicated letters
    std::string foundCharacters{""};

    // Lambda to test whether character already exists
    auto remDup = [&](char c) {
        if (foundCharacters.find(c) == std::string::npos) {
            foundCharacters += c;
            return false;
        } else
            return true;
    };

    key_.erase(std::remove_if(key_.begin(), key_.end(), remDup), key_.end());

    // Store the coords of each letter
    // Uses typedefs defined for the class
    // coord is std::pair<int, int> defined for class
    for (
        size_t i{0}; i < 25;
        i++) {    // Length of key_, should be 25, if not we have a problem elsewhere!
        // Using (row, column) format:
        // (1,2) corresponds to
        // O O O O O
        // O O X O O
        // O O O O O
        // O O O O O
        // O O O O O

        // Store the playfair cipher key map
        coordLookup_[key_[i]] = coord(i % 5, i / 5);
        charLookup_[coord(i % 5, i / 5)] = key_[i];
    }
}

std::string PlayfairCipher::applyCipher(const std::string& inputText,
                                        const CipherMode cipherMode) const
{
    if (inputText == "")
        return "";

    // Change J ??? I
    const std::string message{convertItoJ(inputText)};

    // Initialise paired message
    std::string pairedMsg{message};

    // Only change the message if it should be encrypted
    // - Decrypting message ought to have the right form already
    // We have absolutely no protection yet, but given we are covering
    //  exceptions later, I assume there's no point adding anything now
    if (cipherMode == CipherMode::Encrypt) {
        // If repeated chars in a digraph add an X or Q if XX
        pairedMsg = "";

        // With this vector, only have to update 1 variable per loop in most loops
        // Current character is dual[secondInDual], previous is dual[!secondInDual]
        std::vector<char> dual{message[0], message[1]};

        for (size_t i{0}; i < message.size(); i++) {
            if (dual[0] != dual[1]) {
                pairedMsg += dual[0];
                pairedMsg += dual[1];

                // This will not raise an exception, just be empty characters if out of bounds
                dual[0] = message[i + 2];
                dual[1] = message[i + 3];

                i++;    // can skip the next value, since we process it here
            } else if (dual[0] != 'X') {
                pairedMsg += dual[0];
                pairedMsg += 'X';

                // Only need to update the second character, since what is now the second is
                //  shifted to be the first character, but this is equal to the second character
                dual[1] = message[i + 2];

                // Don't add extra increment to i
            } else {
                pairedMsg += dual[0];
                pairedMsg += 'Q';

                // Only need to update the second character, since what is now the second is
                //  shifted to be the first character, but this is equal to the second character
                dual[1] = message[i + 2];

                // Don't add extra increment to i
            }
        }

        // If the size of input is odd, add a trailing Z
        // '\0' is null char
        if (pairedMsg.back() == '\0') {
            // Remove the null character
            pairedMsg.pop_back();
            pairedMsg += 'Z' - 9 * (*pairedMsg.rbegin() == 'Z');
            // Weird iterator thing:
            // If
            //  a * reference to the value of the iterator:
            //  rbegin() which looks at the contents of the string in reverse
            // is a Z, then subtract 9 from char 'Z' to turn it into 'Q'
        }
    }

    std::string output{""};

    // 6 if Encrypt, 4 is Decrypt: sets direction to change values
    // - This is 5 +/- 1  to ensure we don't get negative results after taking % 5 (grid size = 5)
    // Annoying warning means this has to be int (rather than i.e. int8_t)
    const int direction{2 * (cipherMode == CipherMode::Encrypt) + 4};

    // Loop over the input in Digraphs
    for (size_t i{0}; i < pairedMsg.size(); i += 2) {
        // - Find the coords in the grid for each digraph
        coord c1{coordLookup_.at(pairedMsg[i])};
        coord c2{coordLookup_.at(pairedMsg[i + 1])};
        // - Apply the rules to these coords to get 'new' coords
        if (c1.first == c2.first) {
            // - Find the letters associated with the new coords
            // - Next character on same row with wrap around
            output +=
                charLookup_.at(coord(c1.first, (c1.second + direction) % 5));
            output +=
                charLookup_.at(coord(c2.first, (c2.second + direction) % 5));
        } else if (c1.second == c2.second) {
            // - Find the letters associated with the new coords
            // - Next character on same column with wrap around
            output +=
                charLookup_.at(coord((c1.first + direction) % 5, c1.second));
            output +=
                charLookup_.at(coord((c2.first + direction) % 5, c2.second));
        } else {
            // - Find the letters associated with the new coords
            // - Swap the column the letters appear in
            output += charLookup_.at(coord(c1.first, c2.second));
            output += charLookup_.at(coord(c2.first, c1.second));
        }
    }
    // Return the text
    return output;
}
