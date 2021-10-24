// Project UID 1d9f47bfc76643019cfbf037641defe1

#include "Player.h"
#include "unit_test_framework.h"

#include <iostream>
#include <string>

using namespace std;

bool check_card_removed(Card to_remove, Player * player, int num_remaining) {
    // default lead card to nine of spades
    // default trump suit to clubs 

    Card lead_card = Card(Card::RANK_NINE, Card::SUIT_SPADES);

    for (int i = 0; i < num_remaining; i++) {
        Card played = player->play_card(lead_card, Card::SUIT_CLUBS);
        if (played == to_remove) {
            return false;
        }
    }

    return true;
}

TEST(test_player_get_name) {
    Player * alice = Player_factory("Alice", "Simple");
    ASSERT_EQUAL("Alice", alice->get_name());

    delete alice;
}

// Add more tests here

// Test add_card correcty adds a card to the player's hand
// nine of diamonds is the lead card
// clubs is the trump suit
TEST(test_player_add_card) {
    Player * tester = Player_factory("Tester", "Simple");
    Card spades_ace = Card(Card::RANK_ACE, Card::SUIT_SPADES);
    Card lead_card = Card(Card::RANK_NINE, Card::SUIT_DIAMONDS);

    tester->add_card(spades_ace);
    Card played = tester->play_card(lead_card, Card::SUIT_CLUBS);
    ASSERT_TRUE(spades_ace == played);

    delete tester;
}

// make_trump round 1 general case 
// the player has three trump suit cards 
// the player should order up 
// test both when player is dealer and when player is not
// spades is the proposed trump suit
TEST(test_make_trump_round1_general1) {
    Player * tester = Player_factory("Tester", "Simple");

    Card queen_spades = Card(Card::RANK_QUEEN, Card::SUIT_SPADES);
    Card king_spades = Card(Card::RANK_KING, Card::SUIT_SPADES);
    Card ace_spades = Card(Card::RANK_ACE, Card::SUIT_SPADES);
    Card ten_diamonds = Card(Card::RANK_TEN, Card::SUIT_DIAMONDS);
    Card jack_hearts = Card(Card::RANK_JACK, Card::SUIT_HEARTS);
    Card upcard = Card(Card::RANK_NINE, Card::SUIT_SPADES);

    tester->add_card(queen_spades);
    tester->add_card(king_spades);
    tester->add_card(ace_spades);
    tester->add_card(ten_diamonds);
    tester->add_card(jack_hearts);

    string order_up_dealer;
    string order_up_not_dealer;

    ASSERT_TRUE(tester->make_trump(upcard, true, 1, order_up_dealer));
    ASSERT_EQUAL(order_up_dealer, Card::SUIT_SPADES);
    ASSERT_TRUE(tester->make_trump(upcard, false, 1, order_up_not_dealer));
    ASSERT_EQUAL(order_up_not_dealer, Card::SUIT_SPADES);

    delete tester;
}

// make trump round 1 general case 
// the player only have one trump suit cards
// the player should pass 
// diamonds is the proposed trump suit 
TEST(test_make_trump_round1_general2) {
    Player * tester = Player_factory("Tester", "Simple");

    Card ten_spades = Card(Card::RANK_TEN, Card::SUIT_SPADES);
    Card king_hearts = Card(Card::RANK_KING, Card::SUIT_HEARTS);
    Card ace_spades = Card(Card::RANK_ACE, Card::SUIT_SPADES);
    Card ten_diamonds = Card(Card::RANK_TEN, Card::SUIT_DIAMONDS);
    Card jack_hearts = Card(Card::RANK_JACK, Card::SUIT_HEARTS);
    Card upcard = Card(Card::RANK_QUEEN, Card::SUIT_DIAMONDS);
    
    tester->add_card(ten_spades);
    tester->add_card(king_hearts);
    tester->add_card(ace_spades);
    tester->add_card(ten_diamonds);
    tester->add_card(jack_hearts);

    string order_up_dealer = "";
    string order_up_not_dealer = "";
    ASSERT_FALSE(tester->make_trump(upcard, true, 1, order_up_dealer));
    ASSERT_EQUAL(order_up_dealer, "");
    ASSERT_FALSE(tester->make_trump(upcard, false, 1, order_up_not_dealer));
    ASSERT_EQUAL(order_up_not_dealer, "");

    delete tester;
}

// make trump round 1 edge case
// the player has three trump suit cards but only one of which is a face card
// the player should pass
// hearts is the proposed trump suit
TEST(test_make_trump_round1_edge1) {
    Player * tester = Player_factory("Tester", "Simple");

    Card ten_hearts = Card(Card::RANK_TEN, Card::SUIT_HEARTS);
    Card king_hearts = Card(Card::RANK_KING, Card::SUIT_HEARTS);
    Card ace_spades = Card(Card::RANK_ACE, Card::SUIT_SPADES);
    Card ten_diamonds = Card(Card::RANK_TEN, Card::SUIT_DIAMONDS);
    Card nine_hearts = Card(Card::RANK_NINE, Card::SUIT_HEARTS);
    Card upcard = Card(Card::RANK_ACE, Card::SUIT_HEARTS);

    tester->add_card(ten_hearts);
    tester->add_card(king_hearts);
    tester->add_card(ace_spades);
    tester->add_card(ten_diamonds);
    tester->add_card(nine_hearts);

    string order_up_dealer = "";
    string order_up_not_dealer = "";
    
    ASSERT_FALSE(tester->make_trump(upcard, true, 1, order_up_dealer));
    ASSERT_EQUAL(order_up_dealer, "");
    ASSERT_FALSE(tester->make_trump(upcard, false, 1, order_up_not_dealer));
    ASSERT_EQUAL(order_up_not_dealer, "");

    delete tester;
}

// make trump round 1 edge case 
// the player has one trump suit face card as well as the left bower 
// the player should order up 
// clubs is the proposed trump suit
TEST(test_make_trump_round1_edge2) {
    Player * tester = Player_factory("Tester", "Simple");
    
    Card queen_clubs = Card(Card::RANK_QUEEN, Card::SUIT_CLUBS);
    Card left_bower = Card(Card::RANK_JACK, Card::SUIT_SPADES);
    Card ace_spades = Card(Card::RANK_ACE, Card::SUIT_SPADES);
    Card ten_diamonds = Card(Card::RANK_TEN, Card::SUIT_DIAMONDS);
    Card nine_hearts = Card(Card::RANK_NINE, Card::SUIT_HEARTS);
    Card upcard = Card(Card::RANK_JACK, Card::SUIT_CLUBS);

    tester->add_card(queen_clubs);
    tester->add_card(left_bower);
    tester->add_card(ace_spades);
    tester->add_card(ten_diamonds);
    tester->add_card(nine_hearts);

    string order_up_dealer;
    string order_up_not_dealer;

    ASSERT_TRUE(tester->make_trump(upcard, true, 1, order_up_dealer));
    ASSERT_EQUAL(order_up_dealer, Card::SUIT_CLUBS);
    ASSERT_TRUE(tester->make_trump(upcard, false, 1, order_up_not_dealer));
    ASSERT_EQUAL(order_up_not_dealer, Card::SUIT_CLUBS);

    delete tester;
}

// make trump round 1 edge case 
// the player has two trump suit face cards
// the player also has three face cards in another suit as interference 
// the player should order up  
// spades is the proposed trump suit
TEST(test_make_trump_round1_edge3) {
    Player * tester = Player_factory("Tester", "Simple");

    Card queen_spades = Card(Card::RANK_QUEEN, Card::SUIT_SPADES);
    Card king_spades = Card(Card::RANK_KING, Card::SUIT_SPADES);
    Card ace_hearts = Card(Card::RANK_ACE, Card::SUIT_HEARTS);
    Card king_hearts = Card(Card::RANK_KING, Card::SUIT_HEARTS);
    Card jack_hearts = Card(Card::RANK_JACK, Card::SUIT_HEARTS);
    Card upcard = Card(Card::RANK_NINE, Card::SUIT_SPADES);

    tester->add_card(queen_spades);
    tester->add_card(king_spades);
    tester->add_card(king_hearts);
    tester->add_card(ace_hearts);
    tester->add_card(jack_hearts);

    string order_up_dealer;
    string order_up_not_dealer;

    ASSERT_TRUE(tester->make_trump(upcard, true, 1, order_up_dealer));
    ASSERT_EQUAL(order_up_dealer, Card::SUIT_SPADES);
    ASSERT_TRUE(tester->make_trump(upcard, false, 1, order_up_not_dealer));
    ASSERT_EQUAL(order_up_not_dealer, Card::SUIT_SPADES);

    delete tester;
}

// make trump round 1 edge case 
// all cards in the player's hand is in the trump suit 
// the player should order up 
TEST(test_make_trump_round1_edge4) {
    Player * tester = Player_factory("Tester", "Simple");

    Card nine_spades = Card(Card::RANK_NINE, Card::SUIT_SPADES);
    Card ten_spades = Card(Card::RANK_TEN, Card::SUIT_SPADES);
    Card jack_spades = Card(Card::RANK_JACK, Card::SUIT_SPADES);
    Card queen_spades = Card(Card::RANK_QUEEN, Card::SUIT_SPADES);
    Card king_spades = Card(Card::RANK_KING, Card::SUIT_SPADES);
    Card upcard = Card(Card::RANK_ACE, Card::SUIT_SPADES);

    tester->add_card(nine_spades);
    tester->add_card(ten_spades);
    tester->add_card(jack_spades);
    tester->add_card(queen_spades);
    tester->add_card(king_spades);  

    string order_up_dealer;
    string order_up_not_dealer;

    ASSERT_TRUE(tester->make_trump(upcard, true, 1, order_up_dealer));
    ASSERT_EQUAL(order_up_dealer, Card::SUIT_SPADES);
    ASSERT_TRUE(tester->make_trump(upcard, true, 1, order_up_not_dealer));
    ASSERT_EQUAL(order_up_not_dealer, Card::SUIT_SPADES);

    delete tester;
}

// make trump round 2 general case 
// the player has one trump suit face card in hand 
// the player should order up 
// diamonds is the up card suit 
TEST(test_make_trump_round2_general1) {
    Player * tester = Player_factory("Tester", "Simple");

    Card king_hearts = Card(Card::RANK_KING, Card::SUIT_HEARTS);
    Card queen_clubs = Card(Card::RANK_QUEEN, Card::SUIT_CLUBS);
    Card jack_spades = Card(Card::RANK_JACK, Card::SUIT_SPADES);
    Card ace_spades = Card(Card::RANK_ACE, Card::SUIT_SPADES);
    Card ten_diamonds = Card(Card::RANK_TEN, Card::SUIT_DIAMONDS);
    Card upcard = Card(Card::RANK_ACE, Card::SUIT_DIAMONDS);

    string order_up_dealer;
    string order_up_not_dealer;

    tester->add_card(king_hearts);
    tester->add_card(queen_clubs);
    tester->add_card(jack_spades);
    tester->add_card(ace_spades);
    tester->add_card(ten_diamonds);

    ASSERT_TRUE(tester->make_trump(upcard, true, 2, order_up_dealer));
    ASSERT_EQUAL(order_up_dealer, Card::SUIT_HEARTS);
    ASSERT_TRUE(tester->make_trump(upcard, false, 2, order_up_not_dealer));
    ASSERT_EQUAL(order_up_not_dealer, Card::SUIT_HEARTS);

    delete tester;
}

// make trump round 2 general case 
// the player has one face card in the upcard suit
// and no face card in the trump suit 
// the player should pass unless the player is the dealer 
// hearts is the upcard suit 
TEST(test_make_trump_round2_general2) {
    Player * tester = Player_factory("Tester", "Simple");

    Card king_hearts = Card(Card::RANK_KING, Card::SUIT_HEARTS);
    Card nine_diamonds = Card(Card::RANK_NINE, Card::SUIT_DIAMONDS);
    Card ten_hearts = Card(Card::RANK_TEN, Card::SUIT_HEARTS);
    Card ace_spades = Card(Card::RANK_ACE, Card::SUIT_SPADES);
    Card nine_clubs = Card(Card::RANK_NINE, Card::SUIT_CLUBS);
    Card upcard = Card(Card::RANK_QUEEN, Card::SUIT_HEARTS);

    tester->add_card(king_hearts);
    tester->add_card(nine_diamonds);
    tester->add_card(ten_hearts);
    tester->add_card(ace_spades);
    tester->add_card(nine_clubs);

    string order_up_dealer;
    string order_up_not_dealer = "";

    ASSERT_TRUE(tester->make_trump(upcard, true, 2, order_up_dealer));
    ASSERT_EQUAL(order_up_dealer, Card::SUIT_DIAMONDS);
    ASSERT_FALSE(tester->make_trump(upcard, false, 2, order_up_not_dealer));
    ASSERT_EQUAL(order_up_not_dealer, "");

    delete tester;
}

// make trump round 2 edge case 
// the player has the left bower but no other trump suit face card 
// the player should order up 
// diamonds is the upcard suit
TEST(test_make_trump_round2_edge1) {
    Player * tester = Player_factory("Tester", "Simple");

    Card left_bower = Card(Card::RANK_JACK, Card::SUIT_DIAMONDS);
    Card nine_diamonds = Card(Card::RANK_NINE, Card::SUIT_DIAMONDS);
    Card ten_hearts = Card(Card::RANK_TEN, Card::SUIT_HEARTS);
    Card ace_spades = Card(Card::RANK_ACE, Card::SUIT_SPADES);
    Card nine_clubs = Card(Card::RANK_NINE, Card::SUIT_CLUBS);
    Card upcard = Card(Card::RANK_TEN, Card::SUIT_DIAMONDS);

    tester->add_card(left_bower);
    tester->add_card(nine_diamonds);
    tester->add_card(ten_hearts);
    tester->add_card(ace_spades);
    tester->add_card(nine_clubs);

    string order_up_dealer;
    string order_up_not_dealer;

    ASSERT_TRUE(tester->make_trump(upcard, true, 2, order_up_dealer));
    ASSERT_EQUAL(order_up_dealer, Card::SUIT_HEARTS);
    ASSERT_TRUE(tester->make_trump(upcard, false, 2, order_up_not_dealer));
    ASSERT_EQUAL(order_up_not_dealer, Card::SUIT_HEARTS);

    delete tester;
}

// make trump round 2 edge case
// the player has no card in the up card color
// the player will pass if it is not dealer, and order up if it is the dealer
// clubs is the up card suit 
TEST(test_make_trump_round2_edge2) {
    Player * tester = Player_factory("Tester", "Simple");

    Card jack_hearts = Card(Card::RANK_JACK, Card::SUIT_HEARTS);
    Card nine_diamonds = Card(Card::RANK_NINE, Card::SUIT_DIAMONDS);
    Card ten_hearts = Card(Card::RANK_TEN, Card::SUIT_HEARTS);
    Card ace_hearts = Card(Card::RANK_ACE, Card::SUIT_HEARTS);
    Card jack_diamonds = Card(Card::RANK_NINE, Card::SUIT_DIAMONDS);
    Card upcard = Card(Card::RANK_QUEEN, Card::SUIT_CLUBS);

    tester->add_card(jack_hearts);
    tester->add_card(nine_diamonds);
    tester->add_card(ten_hearts);
    tester->add_card(ace_hearts);
    tester->add_card(jack_diamonds);

    string order_up_dealer;
    string order_up_not_dealer = "";

    ASSERT_TRUE(tester->make_trump(upcard, true, 2, order_up_dealer));
    ASSERT_EQUAL(order_up_dealer, Card::SUIT_SPADES);
    ASSERT_FALSE(tester->make_trump(upcard, false, 2, order_up_not_dealer));
    ASSERT_EQUAL(order_up_not_dealer, "");

    delete tester;
} 

// add and discard general case 
// the upcard is the queen of spades
TEST(test_add_and_discard_general1) {
    Player * tester = Player_factory("Tester", "Simple");

    Card ten_hearts = Card(Card::RANK_TEN, Card::SUIT_HEARTS);
    Card king_hearts = Card(Card::RANK_KING, Card::SUIT_HEARTS);
    Card ace_spades = Card(Card::RANK_ACE, Card::SUIT_SPADES);
    Card ten_diamonds = Card(Card::RANK_TEN, Card::SUIT_DIAMONDS);
    Card nine_hearts = Card(Card::RANK_NINE, Card::SUIT_HEARTS);
    Card upcard = Card(Card::RANK_QUEEN, Card::SUIT_SPADES);

    tester->add_card(ten_hearts);
    tester->add_card(king_hearts);
    tester->add_card(ace_spades);
    tester->add_card(ten_diamonds);
    tester->add_card(nine_hearts);

    // the player should discard nine of hearts
    tester->add_and_discard(upcard);

    ASSERT_TRUE(check_card_removed(nine_hearts, tester, 5));

    delete tester;
}

// add and discard general case 
// the player is required to discard a face card 
// jack of diamonds is the up card
TEST(test_add_and_discard_general2) {
    Player * tester = Player_factory("Tester", "Simple");

    Card king_hearts = Card(Card::RANK_KING, Card::SUIT_HEARTS);
    Card queen_clubs = Card(Card::RANK_QUEEN, Card::SUIT_CLUBS);
    Card jack_spades = Card(Card::RANK_JACK, Card::SUIT_SPADES);
    Card ace_spades = Card(Card::RANK_ACE, Card::SUIT_SPADES);
    Card ten_diamonds = Card(Card::RANK_TEN, Card::SUIT_DIAMONDS);
    Card upcard = Card(Card::RANK_JACK, Card::SUIT_DIAMONDS);

    tester->add_card(king_hearts);
    tester->add_card(queen_clubs);
    tester->add_card(jack_spades);
    tester->add_card(ace_spades);
    tester->add_card(ten_diamonds);

    // the player should discard jack of spades 
    tester->add_and_discard(upcard);
    
    ASSERT_TRUE(check_card_removed(jack_spades, tester, 5));

    delete tester;
}

// add and discard edge case 
// the player's hand contains the left bower 
// the player should of course keep the left bower 
// ace of spades is the upcard
TEST(test_add_and_discard_edge1) {
    Player * tester = Player_factory("Tester", "Simple");

    Card left_bower = Card(Card::RANK_JACK, Card::SUIT_CLUBS);
    Card ten_spades = Card(Card::RANK_TEN, Card::SUIT_SPADES);
    Card jack_spades = Card(Card::RANK_JACK, Card::SUIT_SPADES);
    Card queen_spades = Card(Card::RANK_QUEEN, Card::SUIT_SPADES);
    Card king_spades = Card(Card::RANK_KING, Card::SUIT_SPADES);
    Card upcard = Card(Card::RANK_ACE, Card::SUIT_SPADES);

    tester->add_card(left_bower);
    tester->add_card(ten_spades);
    tester->add_card(jack_spades);
    tester->add_card(queen_spades);
    tester->add_card(king_spades);  

    tester->add_and_discard(upcard);

    // the player should discard the ten of spades
    ASSERT_TRUE(check_card_removed(ten_spades, tester, 5));

    delete tester;
}

// add and discard edge case
// the player is required to discard the upcard
// nine of clubs is the upcard
TEST(test_add_and_discard_edge2) {
    Player * tester = Player_factory("Tester", "Simple");

    Card ten_clubs = Card(Card::RANK_TEN, Card::SUIT_CLUBS);
    Card jack_clubs = Card(Card::RANK_JACK, Card::SUIT_CLUBS);
    Card queen_clubs = Card(Card::RANK_QUEEN, Card::SUIT_CLUBS);
    Card king_clubs = Card(Card::RANK_KING, Card::SUIT_CLUBS);
    Card left_bower = Card(Card::RANK_JACK, Card::SUIT_SPADES);
    Card upcard = Card(Card::RANK_NINE, Card::SUIT_CLUBS);

    tester->add_card(ten_clubs);
    tester->add_card(jack_clubs);
    tester->add_card(queen_clubs);
    tester->add_card(king_clubs);
    tester->add_card(left_bower);

    tester->add_and_discard(upcard);

    // the player should discard the upcard
    ASSERT_TRUE(check_card_removed(upcard, tester, 5));

    delete tester;
}

// lead card general case 
// the player has five cards 
// hearts is the trump suit 
TEST(test_lead_card_general1) {
    Player * tester = Player_factory("Tester", "Simple");
    
    Card king_hearts = Card(Card::RANK_KING, Card::SUIT_HEARTS);
    Card queen_clubs = Card(Card::RANK_QUEEN, Card::SUIT_CLUBS);
    Card jack_spades = Card(Card::RANK_JACK, Card::SUIT_SPADES);
    Card ace_spades = Card(Card::RANK_ACE, Card::SUIT_SPADES);
    Card nine_diamonds = Card(Card::RANK_NINE, Card::SUIT_DIAMONDS);

    tester->add_card(king_hearts);
    tester->add_card(queen_clubs);
    tester->add_card(jack_spades);
    tester->add_card(ace_spades);
    tester->add_card(nine_diamonds);

    // by asking the player to lead five times
    // we also test that the discarding of the lead card works correctly
    ASSERT_EQUAL(tester->lead_card(Card::SUIT_HEARTS), ace_spades);
    ASSERT_EQUAL(tester->lead_card(Card::SUIT_HEARTS), queen_clubs);
    ASSERT_EQUAL(tester->lead_card(Card::SUIT_HEARTS), jack_spades);
    ASSERT_EQUAL(tester->lead_card(Card::SUIT_HEARTS), nine_diamonds);
    ASSERT_EQUAL(tester->lead_card(Card::SUIT_HEARTS), king_hearts);

    delete tester;
}

// lead card general case
// the player has three cards in hand 
// spades is the trump suit 
TEST(test_lead_card_general2) {
    Player * tester = Player_factory("Tester", "Simple");

    Card king_spades = Card(Card::RANK_KING, Card::SUIT_SPADES);
    Card jack_spades = Card(Card::RANK_JACK, Card::SUIT_SPADES);
    Card ace_clubs = Card(Card::RANK_ACE, Card::SUIT_CLUBS);

    tester->add_card(king_spades);
    tester->add_card(jack_spades);
    tester->add_card(ace_clubs);

    ASSERT_EQUAL(tester->lead_card(Card::SUIT_SPADES), ace_clubs);
    ASSERT_EQUAL(tester->lead_card(Card::SUIT_SPADES), jack_spades);
    ASSERT_EQUAL(tester->lead_card(Card::SUIT_SPADES), king_spades);

    delete tester;
}

// lead card edge case 
// the player has three trump suit cards including both bower
// diamonds is the trump suit 
TEST(test_lead_card_edge1) {
    Player * tester = Player_factory("Tester", "Simple");

    Card right_bower = Card(Card::RANK_JACK, Card::SUIT_DIAMONDS);
    Card left_bower = Card(Card::RANK_JACK, Card::SUIT_HEARTS);
    Card ace_diamonds = Card(Card::RANK_ACE, Card::SUIT_DIAMONDS);

    tester->add_card(right_bower);
    tester->add_card(left_bower);
    tester->add_card(ace_diamonds);

    ASSERT_EQUAL(tester->lead_card(Card::SUIT_DIAMONDS), right_bower);
    ASSERT_EQUAL(tester->lead_card(Card::SUIT_DIAMONDS), left_bower);
    ASSERT_EQUAL(tester->lead_card(Card::SUIT_DIAMONDS), ace_diamonds);
    
    delete tester;
}

// lead card edge case
// the player only has one card
// clubs is the trump suit
TEST(test_lead_card_edge2) {
    Player * tester = Player_factory("Tester", "Simple");

    Card ten_hearts = Card(Card::RANK_TEN, Card::SUIT_HEARTS);
    tester->add_card(ten_hearts);

    ASSERT_EQUAL(tester->lead_card(Card::SUIT_CLUBS), ten_hearts);

    // add an assertion to check that the card has been correctly discarded

    delete tester;
}

// play card general case 
// the player is able to follow suit 
// ten of diamonds is the lead card 
// hearts is the trump suit 
TEST(test_play_card_general1) {
    Player * tester = Player_factory("Tester", "Simple");

    Card nine_hearts = Card(Card::RANK_NINE, Card::SUIT_HEARTS);
    Card jack_spades = Card(Card::RANK_JACK, Card::SUIT_SPADES);
    Card queen_diamonds = Card(Card::RANK_QUEEN, Card::SUIT_DIAMONDS);
    Card ten_diamonds = Card(Card::RANK_TEN, Card::SUIT_DIAMONDS);
    Card jack_clubs = Card(Card::RANK_JACK, Card::SUIT_CLUBS);
    Card lead_card = Card(Card::RANK_TEN, Card::SUIT_DIAMONDS);

    tester->add_card(nine_hearts);
    tester->add_card(jack_spades);
    tester->add_card(ten_diamonds);
    tester->add_card(queen_diamonds);
    tester->add_card(jack_clubs);

    ASSERT_EQUAL(tester->play_card(lead_card, Card::SUIT_HEARTS), queen_diamonds);
    ASSERT_EQUAL(tester->play_card(lead_card, Card::SUIT_HEARTS), ten_diamonds);
    ASSERT_EQUAL(tester->play_card(lead_card, Card::SUIT_HEARTS), jack_spades);
    ASSERT_EQUAL(tester->play_card(lead_card, Card::SUIT_HEARTS), jack_clubs);
    ASSERT_EQUAL(tester->play_card(lead_card, Card::SUIT_HEARTS), nine_hearts);

    delete tester;
}

// play card general case
// the player is unable to follow suit 
// the player has both bowers
// king of diamonds is the lead card
// spades is the trump suit
TEST(test_play_card_general2) {
    Player * tester = Player_factory("Tester", "Simple");

    Card right_bower = Card(Card::RANK_JACK, Card::SUIT_SPADES);
    Card left_bower = Card(Card::RANK_JACK, Card::SUIT_CLUBS);
    Card jack_hearts = Card(Card::RANK_JACK, Card::SUIT_HEARTS);
    Card lead_card = Card(Card::RANK_KING, Card::SUIT_DIAMONDS);
    
    tester->add_card(right_bower);
    tester->add_card(left_bower);
    tester->add_card(jack_hearts);

    ASSERT_EQUAL(tester->play_card(lead_card, Card::SUIT_SPADES), jack_hearts);
    ASSERT_EQUAL(tester->play_card(lead_card, Card::SUIT_SPADES), left_bower);
    ASSERT_EQUAL(tester->play_card(lead_card, Card::SUIT_SPADES), right_bower);

    delete tester;
}

// play card edge case
// player only has one card
TEST(test_play_card_edge1) {
    Player * tester = Player_factory("Tester", "Simple");

    Card queen_spades = Card(Card::RANK_QUEEN, Card::SUIT_SPADES);
    Card lead_card = Card(Card::RANK_TEN, Card::SUIT_HEARTS);

    tester->add_card(queen_spades);

    ASSERT_EQUAL(tester->play_card(lead_card, Card::SUIT_DIAMONDS), queen_spades);

    delete tester;
}

// play card edge case
// trump suit and lead card suit is the same
// ten of clubs is the lead card
// clubs is the trump suit
TEST(test_play_card_edge2) {
    Player * tester = Player_factory("Tester", "Simple");

    Card jack_hearts = Card(Card::RANK_JACK, Card::SUIT_HEARTS);
    Card queen_clubs = Card(Card::RANK_QUEEN, Card::SUIT_CLUBS);
    Card jack_spades = Card(Card::RANK_JACK, Card::SUIT_SPADES);
    Card ace_clubs = Card(Card::RANK_ACE, Card::SUIT_CLUBS);
    Card jack_diamonds = Card(Card::RANK_JACK, Card::SUIT_DIAMONDS);
    Card lead_card = Card(Card::RANK_TEN, Card::SUIT_CLUBS);

    tester->add_card(jack_hearts);
    tester->add_card(queen_clubs);
    tester->add_card(jack_spades);
    tester->add_card(ace_clubs);
    tester->add_card(jack_diamonds);

    ASSERT_EQUAL(tester->play_card(lead_card, Card::SUIT_CLUBS), jack_spades);
    ASSERT_EQUAL(tester->play_card(lead_card, Card::SUIT_CLUBS), ace_clubs);
    ASSERT_EQUAL(tester->play_card(lead_card, Card::SUIT_CLUBS), queen_clubs);
    ASSERT_EQUAL(tester->play_card(lead_card, Card::SUIT_CLUBS), jack_hearts);
    ASSERT_EQUAL(tester->play_card(lead_card, Card::SUIT_CLUBS), jack_diamonds);

    delete tester;
}

// play card edge case
// the trick is led by the left bower 
// jack of clubs is the lead card
// spades is the trump suit 
TEST(test_play_card_edge3) {
    Player * tester = Player_factory("Tester", "Simple");

    Card nine_clubs = Card(Card::RANK_NINE,Card::SUIT_CLUBS);
    Card ten_clubs = Card(Card::RANK_TEN, Card::SUIT_CLUBS);
    Card queen_diamonds = Card(Card::RANK_QUEEN, Card::SUIT_DIAMONDS);
    Card lead_card = Card(Card::RANK_JACK, Card::SUIT_CLUBS);

    tester->add_card(nine_clubs);
    tester->add_card(ten_clubs);
    tester->add_card(queen_diamonds);

    ASSERT_EQUAL(tester->play_card(lead_card, Card::SUIT_SPADES), nine_clubs);
    ASSERT_EQUAL(tester->play_card(lead_card, Card::SUIT_SPADES), ten_clubs);
    ASSERT_EQUAL(tester->play_card(lead_card, Card::SUIT_SPADES), queen_diamonds);

    delete tester;
}

TEST_MAIN()
