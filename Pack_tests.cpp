// Project UID 1d9f47bfc76643019cfbf037641defe1

#include "Pack.h"
#include "unit_test_framework.h"
#include <cassert>
#include <iostream>
#include <fstream>

using namespace std;
static const int PACK_SIZE = 24;

TEST(test_pack_default_ctor) {
    Pack pack;
    ASSERT_TRUE(!pack.empty());
    Card first = pack.deal_one();
    ASSERT_EQUAL(Card::RANK_NINE, first.get_rank());
    ASSERT_EQUAL(Card::SUIT_SPADES, first.get_suit());

    Card second = pack.deal_one();
    Card third = pack.deal_one();
    ASSERT_EQUAL(Card::RANK_TEN, second.get_rank());
    ASSERT_EQUAL(Card::SUIT_SPADES, second.get_suit());
    ASSERT_EQUAL(Card::RANK_JACK, third.get_rank());
    ASSERT_EQUAL(Card::SUIT_SPADES, third.get_suit());

    pack.deal_one();
    pack.deal_one();
    pack.deal_one();

    Card seventh = pack.deal_one();
    ASSERT_EQUAL(Card::RANK_NINE, seventh.get_rank());
    ASSERT_EQUAL(Card::SUIT_HEARTS, seventh.get_suit());
}

TEST(test_pack_istream_ctor) {
    const string filename = "pack.in";
    ifstream ifs(filename);
    assert(ifs.is_open());
    Pack pack(ifs);

    ASSERT_TRUE(!pack.empty());
    pack.deal_one();
    pack.deal_one();
    Card third = pack.deal_one();

    ASSERT_EQUAL(Card::RANK_JACK, third.get_rank());
    ASSERT_EQUAL(Card::SUIT_SPADES, third.get_suit());

}

TEST(test_pack_reset) {
    Pack pack;
    while (!pack.empty()) {
        pack.deal_one();
    }
    pack.reset();

    pack.deal_one();
    Card second = pack.deal_one();
    ASSERT_EQUAL(second, Card(Card::RANK_TEN, Card::SUIT_SPADES));
}

TEST(test_pack_empty) {
    Pack pack;
    for (int i = 0; i < PACK_SIZE - 1; ++i) {
        pack.deal_one();
        ASSERT_FALSE(pack.empty());
    }
    pack.deal_one();
    ASSERT_TRUE(pack.empty());
}

TEST(test_pack_shuffle) {
    Pack pack;
    pack.shuffle();
    pack.deal_one();
    pack.deal_one();
    Card third = pack.deal_one();
    ASSERT_EQUAL(third, Card(Card::RANK_NINE, Card::SUIT_SPADES));
    Card fourth = pack.deal_one();
    ASSERT_EQUAL(fourth, Card(Card::RANK_ACE, Card::SUIT_CLUBS));
    pack.deal_one();
    Card sixth = pack.deal_one();
    ASSERT_EQUAL(sixth, Card(Card::RANK_TEN, Card::SUIT_SPADES));
}

TEST_MAIN()
