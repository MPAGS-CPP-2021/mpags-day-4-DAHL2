//! Unit Tests for MPAGSCipher CaesarCipher Class
#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "PlayfairCipher.hpp"

TEST_CASE("Playfair Cipher encryption", "[playfair]")
{
    PlayfairCipher pc{"My key!"};
    // Obnoxious string to make sure key features are tested
    REQUIRE(pc.applyCipher("HELLOSSXXX", CipherMode::Encrypt) ==
            "MNWNNHXEXESVZU");
}

TEST_CASE("Playfair Cipher decryption", "[playfair]")
{
    PlayfairCipher pc{"My key!"};
    REQUIRE(pc.applyCipher("MNWNNHXEXESVZU", CipherMode::Decrypt) ==
            "HELXLOSXSXXQXZ");
}