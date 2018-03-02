/*
 * Project: bid_estimator
 * File: generics.cpp
 * -------------------------------------------------------------------------------------------------
 * Author/Developer: Evan Sheehan
 *
 */

#include "generics.h"
#include <string>

const int ZERO = 0;
const int SENTINEL = -1;
const int INDEX_TWO = 2;
const int INDEX_THREE = 3;
const int INDEX_FOUR = 4;
const int INDEX_FIVE = 5;
const int INDEX_SIX = 6;
const int INDEX_SEVEN = 7;
const int INDEX_EIGHT = 8;
const int INDEX_NINE = 9;
const int INDEX_TEN = 10;
const int INDEX_ELEVEN = 11;
const int INDEX_TWELVE = 12;
const int INDEX_THIRTEEN = 13;
const int INDEX_FOURTEEN = 14;
const int INDEX_FIFTEEN = 15;
const int INDEX_SIXTEEN = 16;

/*
 */
bool getIntegerAugment(std::string entry) {

    try {
        int i = stoi(entry);
        return true;
    } catch (...) {
        return false;
    }
}

