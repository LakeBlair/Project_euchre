// Project UID 1d9f47bfc76643019cfbf037641defe1

#include "Pack.h"
#include <cassert>
#include <string>
#include <array>
using namespace std;

Pack::Pack() {
    for (int suit = 0; suit < NUM_SUITS; suit++) {
        for (int rank = 7; rank < NUM_RANKS; rank++) {
            cards[suit*6 + rank - 7] = Card(RANK_NAMES_BY_WEIGHT[rank],
                                         SUIT_NAMES_BY_WEIGHT[suit]);
        }
    }
    next = 0;
}

Pack::Pack(std::istream& pack_input) {
    string rank, buffer, suit;
    int index = 0;

    while (pack_input >> rank >> buffer >> suit) {
        cards[index] = Card(rank, suit);
        index++;
    }
    next = 0;
}

Card Pack::deal_one() {
    Card next_card = cards[next];
    next++;
    return next_card;
}

void Pack::reset() {
    next = 0;
}

void Pack::shuffle() {
    array<Card, PACK_SIZE> shuffled_deck;

    for (int i = 0; i < 7; i++) {
        for (int index = 0; index < PACK_SIZE/2; index++) {
            shuffled_deck[2*index + 1] = cards[index];
            shuffled_deck[2*index] = cards[index+12];
        }
        cards = shuffled_deck;
    }
}

bool Pack::empty() const {
    return next == PACK_SIZE;
}