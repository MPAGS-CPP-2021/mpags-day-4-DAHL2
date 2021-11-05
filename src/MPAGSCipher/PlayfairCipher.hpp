#ifndef MPAGSCIPHER_PLAYFAIRCIPHER_HPP
#define MPAGSCIPHER_PLAYFAIRCIPHER_HPP

#include "CipherMode.hpp"

#include <cstddef>
#include <map>
#include <string>
#include <vector>

/**
 * \file PlayfairCipher.hpp
 * \brief Contains the declaration of the PlayfairCipher class
 */

/**
 * \class PlayfairCipher
 * \brief Encrypt or decrypt text using the Playfair cipher with the given key
 */
class PlayfairCipher {
  public:
    /**
     * \brief Create a new CaesarCipher, converting the given string into the key
     *
     * \param key the string to convert into the key to be used in the cipher
     */
    explicit PlayfairCipher(const std::string& key);

    /**
     * \brief Convert all instances of J to I in given string
     *
     * \param input the string in which to perform the J to I conversion
     * \return a copy if the input string with J converted to I
     */
    static std::string convertItoJ(const std::string& input);

    /**
     * \brief Set the key to a new value
     *
     * \param key new value of the key
     */
    void setKey(const std::string& key);

    /**
     * \brief Apply the cipher to the provided text
     *
     * \param inputText the text to encrypt or decrypt
     * \param cipherMode whether to encrypt or decrypt the input text
     * \return the result of applying the cipher to the input text
     */
    std::string applyCipher(const std::string& inputText,
                            const CipherMode cipherMode) const;


  private:
    using coord = std::pair<int, int>;
    using letterCoordMap = std::map<char, coord>;
    using coordLetterMap = std::map<coord, char>;

    /// The alphabet - used to determine the cipher character given the plain character and the key
    const std::string alphabet_{"ABCDEFGHIJKLMNOPQRSTUVWXYZ"};

    /// The cipher key, essentially a constant shift to be applied
    std::string key_{""};

    letterCoordMap lToCMap_;
    coordLetterMap cToLMap_;
};

#endif
