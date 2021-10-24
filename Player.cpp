// Project UID 1d9f47bfc76643019cfbf037641defe1

#include <iostream>
#include <cassert>
#include <string>
#include "Player.h"
#include <vector>
#include <algorithm>
using namespace std;

static bool validate_suit_arg(const string suit);
static void sort_card_with_lead(vector<Card>& cards, const Card& led_card,
	const std::string& trump);
static void sort_card(vector<Card>& cards, const std::string& trump);
static void print_hand(vector<Card> &cards, const string name);
static int find_card(const vector<Card> cards, const Card card);

class Simple : public Player {
private:
	string simple_name;
	int total_card = 0;
	vector<Card> hand;
public:
	Simple(string name): simple_name(name) {}

	const string & get_name() const override {
		return simple_name;
	}

	void add_card(const Card& c) override {
		assert(total_card < MAX_HAND_SIZE);
		hand.push_back(c);
		total_card += 1;
	}

	bool make_trump(const Card& upcard, bool is_dealer,
		int round, string& order_up_suit) const override {
		assert(round == 1 || round == 2);
		int trump_face_1st_round = 0;
		int trump_face_2nd_round = 0;
		int desired_1st_round_trump = 2;
		int desired_2nd_round_trump = 1;
		string trump = upcard.get_suit();
		string next_suit = Suit_next(trump);

		for (size_t i = 0; i < MAX_HAND_SIZE; i++) {

			if (hand[i].get_suit(trump) == upcard.get_suit()) {
				if (hand[i].is_face()) {
					trump_face_1st_round += 1;
				}
			}

			if (hand[i].get_suit(next_suit) == next_suit) {
				if (hand[i].is_face()) {
					trump_face_2nd_round += 1;
				}
			}
		}

		if(round == 1){
			if (trump_face_1st_round >= desired_1st_round_trump) {
				order_up_suit = upcard.get_suit();
				return true;
			}
			else {
				return false;
			}
		}
		else if (round == 2) {
			if (trump_face_2nd_round >= desired_2nd_round_trump) {
				order_up_suit = next_suit;
				return true;
			}
			else {
				if (is_dealer) {
					order_up_suit = next_suit;
					return true;
				}
				else {
					return false;
				}
			}
		}
		return false;
	}

	void add_and_discard(const Card& upcard) override {
		assert(hand.size() >= 1);

		sort_card(hand, upcard.get_suit());

		if (!Card_less((upcard), hand[0], upcard.get_suit())) {
			hand.erase(hand.begin());
			hand.push_back(upcard);
		}
	}

	Card lead_card(const std::string& trump) override {
		assert(hand.size() >= 1);
		assert(validate_suit_arg(trump));
		total_card -= 1;

		sort_card(hand, trump);

		if (hand[0].is_trump(trump)) {
			Card play = hand[hand.size() - 1];
			hand.erase(hand.end());
			return play;
		}

		if (!hand[hand.size() - 1].is_trump(trump)) {
			Card play = hand[hand.size() - 1];
			hand.erase(hand.end());
			return play;
		}

		for (size_t i = 0; i < hand.size(); i++) {
			if (hand[i].get_suit(trump) == trump) {
				Card play = hand[i - 1];
				hand.erase(hand.begin() + i - 1);
				return play;
			}

		}
		return Card();
	}

	Card play_card(const Card& led_card, const std::string& trump) override {
		assert(hand.size() >= 1);
		assert(validate_suit_arg(trump));
		bool is_suit_in_hand = false;
		int highest_suit_index;
		total_card -= 1;

		sort_card_with_lead(hand, led_card, trump);

		for (size_t i = 0; i < hand.size(); i++) {
			if (hand[i].get_suit(trump) == led_card.get_suit(trump)) {
				is_suit_in_hand = true;
				highest_suit_index = i;
			}
		}
		if (is_suit_in_hand) {
			Card play = hand[highest_suit_index];
			hand.erase(hand.begin() + highest_suit_index);
			return play;
		}
		else {
			Card play = hand[0];
			hand.erase(hand.begin());
			return play;
		}
	}
};

class Human : public Player {
private:
	string human_name;
	int total_card = 0;
	vector<Card> hand;
	
public:
	Human(string name): human_name(name){}

	const string& get_name() const override {
		return human_name;
	}

	void add_card(const Card& c) override {
		assert(total_card < MAX_HAND_SIZE);
		hand.push_back(c);
		total_card += 1;
	}

	bool make_trump(const Card& upcard, bool is_dealer,
		int round, std::string& order_up_suit) const override {
		string decision;
		vector<Card> temp_hand = hand;

		if (round == 2 && is_dealer) {
			order_up_suit = Suit_next(upcard.get_suit());
			cout << endl;
			return true;
		}

		print_hand(temp_hand, human_name);
		cout << "Human player " << human_name 
			 << ", please enter a suit, or \"pass\":" << endl;
		cin >> decision;

		if (decision == "pass") {
			return false;
		} else {
			order_up_suit = decision;
			cout << endl;
			return true;
		}
	}

	void add_and_discard(const Card& upcard) override {
		int card_index;
		string trump = upcard.get_suit();

		vector<Card> io_cards = hand;
		print_hand(io_cards, human_name);

		cout << "Discard upcard: [-1]" << endl;
		cout << "Human player " << human_name 
			 << ", please select a card to discard:" << endl;
		cin >> card_index;

		Card to_discard = io_cards[card_index];
		int discard_index = find_card(hand, to_discard);

		if (card_index != -1) {
			hand.erase(hand.begin() + discard_index);
			hand.push_back(upcard);
		}
	}

	Card lead_card(const std::string& trump) override {
		assert(hand.size() >= 1);
		assert(validate_suit_arg(trump));
		int card_index;
		total_card -= 1;

		vector<Card> io_cards = hand;
		print_hand(io_cards, human_name);

		cout << "Human player " << human_name << ", please select a card:" << endl;
		cin >> card_index;
		Card to_play = io_cards[card_index];
		int discard_index = find_card(hand, to_play);
		
		hand.erase(hand.begin() + discard_index);
		return to_play;
	}
	
	Card play_card(const Card& led_card, const std::string& trump) override {
		assert(hand.size() >= 1);
		assert(validate_suit_arg(trump));
		int card_index;
		total_card -= 1;

		vector<Card> io_cards = hand;
		print_hand(io_cards, human_name);

		cout << "Human player " << human_name << ", please select a card:" << endl;
		cin >> card_index;

		Card to_play = io_cards[card_index];
		int discard_index = find_card(hand, to_play);
		
		hand.erase(hand.begin() + discard_index);
		return to_play;
		return to_play;
	}
};


Player * Player_factory(const string &name, const string &strategy) {
	if (strategy == "Simple") {
		return new Simple(name);
	}
	else if (strategy == "Human") {
		return new Human(name);
	}
	else {
		assert(false);
	}
	return nullptr;
}

ostream & operator<<(ostream &os, const Player &p) {
	os << p.get_name();
	return os;
}

static bool validate_suit_arg(const string suit) {
    bool check0 = suit == Card::SUIT_CLUBS;
    bool check1 = suit == Card::SUIT_DIAMONDS;
    bool check2 = suit == Card::SUIT_HEARTS;
    bool check3 = suit == Card::SUIT_SPADES;

    return check0 || check1 || check2 || check3;
}

static void sort_card_with_lead(vector<Card>& cards, const Card& led_card,
	const std::string& trump) {
	for (size_t i = 0; i < cards.size(); i++) {
		for (size_t j = 0; j < cards.size() - i - 1; j++) {
			if (!Card_less(cards[j], cards[j + 1], led_card, trump)) {
				Card temp = cards[j];
				cards[j] = cards[j + 1];
				cards[j + 1] = temp;
			}
		}
	}
}
static void sort_card(vector<Card> &cards, const std::string& trump) {
	for (size_t i = 0; i < cards.size(); i++) {
		for (size_t j = 0; j < cards.size() - i - 1; j++) {
			if (!Card_less(cards[j], cards[j + 1], trump)) {
				Card temp = cards[j];
				cards[j] = cards[j + 1];
				cards[j + 1] = temp;
			}
		}
	}
}

static void print_hand(vector<Card> &cards, const string name) {
	sort(cards.begin(), cards.end());
	for (size_t i = 0; i < cards.size(); i++) {
		cout << "Human player " << name << "'s hand: "
			 << "[" << i << "] " << cards[i] << endl;
	}
}

static int find_card(const vector<Card> cards, const Card card) {
	for (size_t i = 0; i < cards.size(); i++) {
		if (cards[i] == card) {
			return i;
		}
	}
	return -1;
}
