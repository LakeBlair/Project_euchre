// Project UID 1d9f47bfc76643019cfbf037641defe1

#include "Card.h"
#include "unit_test_framework.h"
#include <iostream>
#include <sstream>

using namespace std;


TEST(test_card_ctor) {
    Card c(Card::RANK_ACE, Card::SUIT_HEARTS);
    ASSERT_EQUAL(Card::RANK_ACE, c.get_rank());
    ASSERT_EQUAL(Card::SUIT_HEARTS, c.get_suit());
}

// Add more test cases here

// general case for the two get suit functions 
TEST(test_get_suit_general) {
    Card c_red(Card::RANK_ACE, Card::SUIT_HEARTS);
    Card c_black(Card::RANK_THREE, Card::SUIT_SPADES);
    ASSERT_EQUAL(Card::SUIT_HEARTS, c_red.get_suit());
    ASSERT_EQUAL(Card::SUIT_HEARTS, c_red.get_suit(Card::SUIT_DIAMONDS));
    ASSERT_EQUAL(Card::SUIT_SPADES, c_black.get_suit());
    ASSERT_EQUAL(Card::SUIT_SPADES, c_black.get_suit(Card::SUIT_CLUBS));
}

// test get_suit correctly deals with the left bower 
TEST(test_get_suit_left_bower) {
    Card c(Card::RANK_JACK, Card::SUIT_SPADES);
    ASSERT_EQUAL(Card::SUIT_CLUBS, c.get_suit(Card::SUIT_CLUBS));
}

// test get_suit don't return trump suit for non-jack cards
// in the adjacent suit 
TEST(test_get_suit_not_bower) {
    Card c(Card::RANK_TEN, Card::SUIT_SPADES);
    ASSERT_EQUAL(Card::SUIT_SPADES, c.get_suit(Card::SUIT_CLUBS));
}

// general test cases for the is_face function
// Try all four suits with different ranks 
TEST(test_is_face_general) {
    Card hearts_ace(Card::RANK_ACE, Card::SUIT_HEARTS);
    Card diamonds_nine(Card::RANK_NINE, Card::SUIT_DIAMONDS);
    Card clubs_jack(Card::RANK_JACK, Card::SUIT_CLUBS);
    Card spades_four(Card::RANK_FOUR, Card::SUIT_SPADES);

    ASSERT_TRUE(hearts_ace.is_face());
    ASSERT_TRUE(clubs_jack.is_face());
    ASSERT_FALSE(diamonds_nine.is_face());
    ASSERT_FALSE(spades_four.is_face());
}

// general test cases for is_right_bower
TEST(test_is_right_bower) {
    Card c_true(Card::RANK_JACK, Card::SUIT_HEARTS);
    Card c_false_suit(Card::RANK_JACK, Card::SUIT_CLUBS);
    Card c_false_rank(Card::RANK_EIGHT, Card::SUIT_DIAMONDS);

    ASSERT_TRUE(c_true.is_right_bower(Card::SUIT_HEARTS));
    ASSERT_FALSE(c_false_suit.is_right_bower(Card::SUIT_SPADES));
    ASSERT_FALSE(c_false_rank.is_right_bower(Card::SUIT_DIAMONDS));
}

// general test cases for is_left_bower
TEST(test_is_left_bower) {
    Card c_true(Card::RANK_JACK, Card::SUIT_CLUBS);
    Card c_right_bower(Card::RANK_JACK, Card::SUIT_SPADES);
    Card c_false_rank(Card::RANK_QUEEN, Card::SUIT_HEARTS);

    ASSERT_TRUE(c_true.is_left_bower(Card::SUIT_SPADES));
    ASSERT_FALSE(c_right_bower.is_left_bower(Card::SUIT_SPADES));
    ASSERT_FALSE(c_false_rank.is_left_bower(Card::SUIT_DIAMONDS));
}

// general test cases for is_trump
TEST(test_is_trump) {
    Card c_left_bower(Card::RANK_JACK, Card::SUIT_DIAMONDS);
    Card c_trump_nine(Card::RANK_NINE, Card::SUIT_CLUBS);
    Card c_not_trump(Card::RANK_KING, Card::SUIT_SPADES);

    ASSERT_FALSE(c_not_trump.is_trump(Card::SUIT_HEARTS));
    ASSERT_TRUE(c_left_bower.is_trump(Card::SUIT_HEARTS));
    ASSERT_TRUE(c_trump_nine.is_trump(Card::SUIT_CLUBS));
}

// exhaustive testing for Suit_next
TEST(test_Suit_next) {
    ASSERT_EQUAL(Suit_next(Card::SUIT_CLUBS), Card::SUIT_SPADES);
    ASSERT_EQUAL(Suit_next(Card::SUIT_DIAMONDS), Card::SUIT_HEARTS);
    ASSERT_EQUAL(Suit_next(Card::SUIT_HEARTS), Card::SUIT_DIAMONDS);
    ASSERT_EQUAL(Suit_next(Card::SUIT_SPADES), Card::SUIT_CLUBS);
}

// next we test the card comparison operators 
// for these we will use the same four test schemes 
// 1, card comapred with itself 
// 2, same suit, different rank
// 3, different suit, different rank
// 4, different suit, same rank

TEST(test_less_than) {
    Card clubs_ten(Card::RANK_TEN, Card::SUIT_CLUBS);
    Card clubs_six(Card::RANK_SIX, Card::SUIT_CLUBS);
    Card hearts_king(Card::RANK_KING, Card::SUIT_HEARTS);
    Card diamonds_king(Card::RANK_KING, Card::SUIT_DIAMONDS);

    ASSERT_FALSE(clubs_ten < clubs_ten);
    ASSERT_TRUE(clubs_six < clubs_ten);
    ASSERT_FALSE(diamonds_king < clubs_ten);
    ASSERT_TRUE(hearts_king < diamonds_king);
} 

TEST(test_less_than_or_equal) {
    Card clubs_ten(Card::RANK_TEN, Card::SUIT_CLUBS);
    Card hearts_five(Card::RANK_FIVE, Card::SUIT_HEARTS);
    Card hearts_king(Card::RANK_KING, Card::SUIT_HEARTS);
    Card diamonds_king(Card::RANK_KING, Card::SUIT_DIAMONDS);

    ASSERT_TRUE(hearts_king <= hearts_king);
    ASSERT_FALSE(hearts_king <= hearts_five);
    ASSERT_TRUE(clubs_ten <= hearts_king);
    ASSERT_FALSE(diamonds_king <= hearts_king);
}

TEST(test_larger_than) {
    Card spades_four(Card::RANK_FOUR, Card::SUIT_SPADES);
    Card spades_six(Card::RANK_SIX, Card::SUIT_SPADES);
    Card hearts_king(Card::RANK_KING, Card::SUIT_HEARTS);
    Card diamonds_king(Card::RANK_KING, Card::SUIT_DIAMONDS);

    ASSERT_FALSE(spades_six > spades_six);
    ASSERT_TRUE(spades_six > spades_four);
    ASSERT_FALSE(spades_six > hearts_king);
    ASSERT_TRUE(diamonds_king > hearts_king);
}

TEST(test_larger_than_or_equal_to) {
    Card spades_four(Card::RANK_FOUR, Card::SUIT_SPADES);
    Card spades_six(Card::RANK_SIX, Card::SUIT_SPADES);
    Card clubs_ace(Card::RANK_ACE, Card::SUIT_CLUBS);
    Card diamonds_ace(Card::RANK_ACE, Card::SUIT_DIAMONDS);

    ASSERT_TRUE(clubs_ace >= clubs_ace);
    ASSERT_FALSE(spades_four >= spades_six);
    ASSERT_TRUE(clubs_ace >= spades_six);
    ASSERT_FALSE(clubs_ace >= diamonds_ace);
}

TEST(test_equal) {
    Card spades_four(Card::RANK_FOUR, Card::SUIT_SPADES);
    Card hearts_six(Card::RANK_SIX, Card::SUIT_HEARTS);
    Card hearts_jack(Card::RANK_JACK, Card::SUIT_HEARTS);
    Card diamonds_jack(Card::RANK_JACK, Card::SUIT_DIAMONDS);

    ASSERT_TRUE(spades_four == spades_four);
    ASSERT_FALSE(hearts_six == hearts_jack);
    ASSERT_FALSE(spades_four == diamonds_jack);
    ASSERT_FALSE(hearts_jack == diamonds_jack);
}

TEST(test_not_equal) {
    Card spades_two(Card::RANK_TWO, Card::SUIT_SPADES);
    Card hearts_five(Card::RANK_FIVE, Card::SUIT_HEARTS);
    Card hearts_ten(Card::RANK_TEN, Card::SUIT_HEARTS);
    Card diamonds_ten(Card::RANK_TEN, Card::SUIT_DIAMONDS);

    ASSERT_FALSE(hearts_ten != hearts_ten);
    ASSERT_TRUE(hearts_five != hearts_ten);
    ASSERT_TRUE(spades_two != diamonds_ten);
    ASSERT_TRUE(hearts_ten != diamonds_ten);
}

TEST(test_print) {
    Card clubs_five(Card::RANK_FIVE, Card::SUIT_CLUBS);
    Card diamonds_ace(Card::RANK_ACE, Card::SUIT_DIAMONDS);
    ostringstream buffer1;
    buffer1 << clubs_five;
    ASSERT_EQUAL(buffer1.str(), "Five of Clubs");
    ostringstream buffer2;
    buffer2 << diamonds_ace;
    ASSERT_EQUAL(buffer2.str(), "Ace of Diamonds");    
}

////////////////////////////////////////////
// general test cases for Card_less(trump)//
////////////////////////////////////////////

// the t in function names stands for trump only

// first we try compare within the same suit 
// and comparing non-trump cards to trump cards
// Spades is trump 
TEST(test_card_less_t_general) {
    Card spades_king(Card::RANK_KING, Card::SUIT_SPADES);
    Card spades_seven(Card::RANK_SEVEN, Card::SUIT_SPADES);
    Card hearts_ace(Card::RANK_ACE, Card::SUIT_HEARTS);
    Card hearts_jack(Card::RANK_JACK, Card::SUIT_HEARTS);

    ASSERT_TRUE(Card_less(spades_seven, spades_king, Card::SUIT_SPADES));
    ASSERT_FALSE(Card_less(hearts_ace, hearts_jack, Card::SUIT_SPADES));
    ASSERT_TRUE(Card_less(hearts_ace, spades_seven, Card::SUIT_SPADES));
}

// test that the right bower correctly trumps all other cards 
// Diamonds is trump
TEST(test_card_less_t_right_bower) {
    Card right_bower(Card::RANK_JACK, Card::SUIT_DIAMONDS);
    Card diamonds_ace(Card::RANK_ACE, Card::SUIT_DIAMONDS);
    Card left_bower(Card::RANK_JACK, Card::SUIT_HEARTS);
    Card clubs_nine(Card::RANK_NINE, Card::SUIT_CLUBS);

    ASSERT_FALSE(Card_less(right_bower, diamonds_ace, Card::SUIT_DIAMONDS));
    ASSERT_TRUE(Card_less(left_bower, right_bower, Card::SUIT_DIAMONDS));
    ASSERT_FALSE(Card_less(right_bower, clubs_nine, Card::SUIT_DIAMONDS));
}

// test that left bower correctly trumps all other cards except for right bower
// Clubs is trump
TEST(test_card_less_t_left_bower) {
    Card left_bower(Card::RANK_JACK, Card::SUIT_SPADES);
    Card right_bower(Card::RANK_JACK, Card::SUIT_CLUBS);
    Card clubs_queen(Card::RANK_QUEEN, Card::SUIT_CLUBS);
    Card spades_king(Card::RANK_KING, Card::SUIT_SPADES);
    Card diamonds_jack(Card::RANK_JACK, Card::SUIT_DIAMONDS);

    ASSERT_TRUE(Card_less(clubs_queen, left_bower, Card::SUIT_CLUBS));
    ASSERT_TRUE(Card_less(spades_king, left_bower, Card::SUIT_CLUBS));
    ASSERT_FALSE(Card_less(left_bower, diamonds_jack, Card::SUIT_CLUBS));
    ASSERT_FALSE(Card_less(right_bower, left_bower, Card::SUIT_CLUBS));
}

// edge cases where cards are compared to themselves
// Hearts is trump
TEST(test_card_less_t_edge) {
    Card right_bower(Card::RANK_JACK, Card::SUIT_HEARTS);
    Card left_bower(Card::RANK_JACK, Card::SUIT_DIAMONDS);
    Card hearts_eight(Card::RANK_EIGHT, Card::SUIT_HEARTS);
    Card clubs_nine(Card::RANK_NINE, Card::SUIT_CLUBS);

    ASSERT_FALSE(Card_less(right_bower, right_bower, Card::SUIT_HEARTS));
    ASSERT_FALSE(Card_less(left_bower, left_bower, Card::SUIT_HEARTS));
    ASSERT_FALSE(Card_less(hearts_eight, hearts_eight, Card::SUIT_HEARTS));
    ASSERT_FALSE(Card_less(clubs_nine, clubs_nine, Card::SUIT_HEARTS));
}

//////////////////////////////////////////////////
// general test cases for Card_less(trump, lead)//
//////////////////////////////////////////////////

// the tl in function names stands for trump and lead

// general test cases that doesn't involve trump suit cards
// Ten of diamonds is lead card
// Clubs is trump suit
TEST(test_card_less_tl_lead_only) {
    Card lead(Card::RANK_TEN, Card::SUIT_DIAMONDS);
    Card diamonds_ten(Card::RANK_TEN, Card::SUIT_DIAMONDS);
    Card diamonds_three(Card::RANK_THREE, Card::SUIT_DIAMONDS);
    Card spades_ace(Card::RANK_ACE, Card::SUIT_SPADES);

    ASSERT_TRUE(Card_less(diamonds_three, diamonds_ten, lead, Card::SUIT_CLUBS));
    ASSERT_FALSE(Card_less(diamonds_ten, spades_ace, lead, Card::SUIT_CLUBS));
}

// general test cases that doesn't involve led suit cards 
// Queen of Spades is lead card
// Hearts is the trump suit 
TEST(test_card_less_tl_trump_only) {
    Card lead(Card::RANK_QUEEN, Card::SUIT_SPADES);
    Card diamonds_ace(Card::RANK_ACE, Card::SUIT_DIAMONDS);
    Card clubs_king(Card::RANK_KING, Card::SUIT_CLUBS);
    Card clubs_nine(Card::RANK_NINE, Card::SUIT_CLUBS);
    Card hearts_five(Card::RANK_FIVE, Card::SUIT_HEARTS);

    ASSERT_TRUE(Card_less(clubs_nine, clubs_king, lead, Card::SUIT_HEARTS));
    ASSERT_FALSE(Card_less(diamonds_ace, clubs_king, lead, Card::SUIT_HEARTS));
    ASSERT_TRUE(Card_less(clubs_king, hearts_five, lead, Card::SUIT_HEARTS));
}

// test right bowers correctly beats all other cards 
// regardless of whether it's lead suit or not
// Ace of Clubs is the lead card
// Diamonds is the trump suit
TEST(test_card_less_tl_right_bower) {
    Card lead(Card::RANK_ACE, Card::SUIT_CLUBS);
    Card right_bower(Card::RANK_JACK, Card::SUIT_DIAMONDS);
    Card left_bower(Card::RANK_JACK, Card::SUIT_HEARTS);
    Card clubs_king(Card::RANK_KING, Card::SUIT_CLUBS);
    Card diamonds_queen(Card::RANK_QUEEN, Card::SUIT_DIAMONDS);
    Card spades_ace(Card::RANK_ACE, Card::SUIT_SPADES);

    ASSERT_FALSE(Card_less(right_bower, clubs_king, lead, Card::SUIT_DIAMONDS));
    ASSERT_TRUE(Card_less(clubs_king, right_bower, lead, Card::SUIT_HEARTS));
    ASSERT_FALSE(Card_less(right_bower, diamonds_queen, lead, Card::SUIT_HEARTS));
    ASSERT_TRUE(Card_less(spades_ace, right_bower, lead, Card::SUIT_HEARTS));
}

// test left bower correctly beats all other cards except right bower
// Nine of Hearts is the lead card
// Spades is the trump suit
TEST(test_card_less_tl_left_bower) {
    Card lead(Card::RANK_NINE, Card::SUIT_HEARTS);
    Card right_bower(Card::RANK_JACK, Card::SUIT_SPADES);
    Card left_bower(Card::RANK_JACK, Card::SUIT_CLUBS);
    Card hearts_ten(Card::RANK_TEN, Card::SUIT_HEARTS);
    Card spades_king(Card::RANK_KING, Card::SUIT_SPADES);
    Card clubs_ace(Card::RANK_ACE, Card::SUIT_CLUBS);

    ASSERT_FALSE(Card_less(left_bower, hearts_ten, lead, Card::SUIT_SPADES));
    ASSERT_TRUE(Card_less(left_bower, right_bower, lead, Card::SUIT_SPADES));
    ASSERT_FALSE(Card_less(left_bower, spades_king, lead, Card::SUIT_SPADES));
    ASSERT_TRUE(Card_less(clubs_ace, left_bower, lead, Card::SUIT_SPADES));
}

// More complicated cases involving both trump and led suit 
// to approximate actual application
// Queen of Diamonds is the lead card
// Hearts is the trump suit 
TEST(test_card_less_tl_trump_and_lead) {
    Card lead(Card::RANK_QUEEN, Card::SUIT_DIAMONDS);
    Card diamonds_four(Card::RANK_FOUR, Card::SUIT_DIAMONDS);
    Card diamonds_ace(Card::RANK_ACE, Card::SUIT_DIAMONDS);
    Card hearts_nine(Card::RANK_NINE, Card::SUIT_HEARTS);
    Card hearts_king(Card::RANK_TEN, Card::SUIT_HEARTS);
    Card spades_seven(Card::RANK_SEVEN, Card::SUIT_SPADES);
    Card spades_jack(Card::RANK_JACK, Card::SUIT_SPADES);
    Card clubs_jack(Card::RANK_JACK, Card::SUIT_CLUBS);

    // lead vs neither
    ASSERT_FALSE(Card_less(diamonds_four, spades_jack, lead, Card::SUIT_HEARTS));
    // lead vs trump
    ASSERT_TRUE(Card_less(diamonds_ace, hearts_nine, lead, Card::SUIT_HEARTS));
    // lead vs lead
    ASSERT_FALSE(Card_less(diamonds_ace, diamonds_four, lead, Card::SUIT_HEARTS));
    // neither vs neither
    ASSERT_TRUE(Card_less(spades_jack, clubs_jack, lead, Card::SUIT_HEARTS));
    // trump vs neither 
    ASSERT_FALSE(Card_less(hearts_nine, spades_jack, lead, Card::SUIT_HEARTS));
    // trump vs trump
    ASSERT_TRUE(Card_less(hearts_nine, hearts_king, lead, Card::SUIT_HEARTS));
}

// edge case where the lead suit is the same as the trump suit 
// Nine of Clubs is the lead card
// Clubs is the trump suit 
TEST(test_card_less_tl_edge_1) {
    Card lead(Card::RANK_NINE, Card::SUIT_CLUBS);
    Card clubs_eight(Card::RANK_EIGHT, Card::SUIT_CLUBS);
    Card left_bower(Card::RANK_JACK, Card::SUIT_SPADES);
    Card hearts_ace(Card::RANK_ACE, Card::SUIT_HEARTS);
    Card clubs_king(Card::RANK_KING, Card::SUIT_CLUBS);

    ASSERT_FALSE(Card_less(left_bower, clubs_king, lead, Card::SUIT_CLUBS));
    ASSERT_TRUE(Card_less(hearts_ace, clubs_eight, lead, Card::SUIT_CLUBS));
    ASSERT_FALSE(Card_less(clubs_king, clubs_eight, lead, Card::SUIT_CLUBS));
}

// edge case where the left bower is the lead card
// jack of clubs is the lead card
// spades is the trump suit
TEST(test_card_less_tl_edge_2) {
    Card lead_card = Card(Card::RANK_JACK, Card::SUIT_CLUBS);
    Card nine_clubs = Card(Card::RANK_NINE, Card::SUIT_CLUBS);
    Card ten_clubs = Card(Card::RANK_TEN, Card::SUIT_CLUBS);
    Card queen_diamonds = Card(Card::RANK_QUEEN, Card::SUIT_DIAMONDS);

    ASSERT_TRUE(Card_less(nine_clubs, queen_diamonds, lead_card, Card::SUIT_SPADES));
    ASSERT_FALSE(Card_less(lead_card, ten_clubs, lead_card, Card::SUIT_SPADES));
}

// edge cases where cards are compared with themselves 
// Nine of Spades is the lead card
// Hearts is the trump suit 
TEST(test_card_less_tl_edge_3) {
    Card lead(Card::RANK_NINE, Card::SUIT_SPADES);
    Card right_bower(Card::RANK_JACK, Card::SUIT_HEARTS);
    Card left_bower(Card::RANK_JACK, Card::SUIT_DIAMONDS);
    Card hearts_six(Card::RANK_SIX, Card::SUIT_HEARTS);
    Card spades_ace(Card::RANK_ACE, Card::SUIT_SPADES);
    Card clubs_ten(Card::RANK_TEN, Card::SUIT_CLUBS);

    ASSERT_FALSE(Card_less(right_bower, right_bower, lead, Card::SUIT_HEARTS));
    ASSERT_FALSE(Card_less(left_bower, left_bower, lead, Card::SUIT_HEARTS));
    ASSERT_FALSE(Card_less(hearts_six, hearts_six, lead, Card::SUIT_HEARTS));
    ASSERT_FALSE(Card_less(spades_ace, spades_ace, lead, Card::SUIT_HEARTS));
    ASSERT_FALSE(Card_less(clubs_ten, clubs_ten, lead, Card::SUIT_HEARTS));
}

TEST_MAIN()
