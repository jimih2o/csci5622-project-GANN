#ifndef NDECKSHOE_H
#define NDECKSHOE_H

#include "blackjackutils.h"

#include <algorithm>
#include <array>
#include <string>

template<const int N>
class NDeckShoe {
public:
    
    NDeckShoe() {
        int pos = 0;
        for (int i = 0; i < N; ++i) {
            for (int s = 0; s < 4; ++s) {
                for (int c = 0; c < 13; ++c) {
                    mDeck[pos] = static_cast<BjUtils::CardValueEnum>(c+2);
                    ++pos;
                }
            }
        }
        mPos = 0;
    }
    
    BjUtils::CardValueEnum getCard() {
        if (mPos >= mDeck.size()) {
            throw std::string("Deck out of bounds.\n");
        }
        return mDeck.at(mPos++);
    }
    
    void shuffle() {
        std::random_shuffle(mDeck.begin(), mDeck.end());
        mPos = 0;
    }
    
    void signalNewHand() {
        if ((float)mPos >= ((float)mDeck.size())*2.0/3.0) {
            // Shuffle after having used the first 2/3 of the shoe
            shuffle();
        }
    }
    
private:
    
    std::array<BjUtils::CardValueEnum, 52*N> mDeck;
    int mPos;
    
};

#endif // NDECKSHOE_H
