// Project UID 1d9f47bfc76643019cfbf037641defe1

// add any necessary #include or using directives here
#include <cassert>
#include <iostream>
#include <string>
#include "Card.h"
using namespace std;

//declare helper functions
static int find_index_rank(std::string const rank);
static int find_index_suit(std::string const suit);
static bool check_invariants(std::string suit, std::string rank);
static bool validate_suit_arg(std::string suit);
static bool validate_rank_arg(std::string suit);

// rank and suit names -- do not remove these
constexpr const char* const Card::RANK_TWO;
constexpr const char* const Card::RANK_THREE;
constexpr const char* const Card::RANK_FOUR;
constexpr const char* const Card::RANK_FIVE;
constexpr const char* const Card::RANK_SIX;
constexpr const char* const Card::RANK_SEVEN;
constexpr const char* const Card::RANK_EIGHT;
constexpr const char* const Card::RANK_NINE;
constexpr const char* const Card::RANK_TEN;
constexpr const char* const Card::RANK_JACK;
constexpr const char* const Card::RANK_QUEEN;
constexpr const char* const Card::RANK_KING;
constexpr const char* const Card::RANK_ACE;

constexpr const char* const Card::SUIT_SPADES;
constexpr const char* const Card::SUIT_HEARTS;
constexpr const char* const Card::SUIT_CLUBS;
constexpr const char* const Card::SUIT_DIAMONDS;

// add your code below
Card::Card()
	:rank(Card::RANK_TWO), suit(Card::SUIT_SPADES) {
}

Card::Card(const string& rank, const string& suit) 
	: rank(rank), suit(suit) {
	assert(check_invariants(suit, rank));
}

string Card::get_rank() const {
	return rank;
}

string Card::get_suit() const {
	return suit;
}

string Suit_next(const string& suit) {
	assert(validate_suit_arg(suit));

	if (suit == Card::SUIT_SPADES) {
		return Card::SUIT_CLUBS;
	}
	else if (suit == Card::SUIT_CLUBS) {
		return Card::SUIT_SPADES;
	}
	else if (suit == Card::SUIT_DIAMONDS) {
		return Card::SUIT_HEARTS;
	}
	else if (suit == Card::SUIT_HEARTS) {
		return Card::SUIT_DIAMONDS;
	}
	return "";
}

bool Card::is_right_bower(const string& trump) const {
	assert(validate_suit_arg(trump));

	return get_suit() == trump && get_rank() == Card::RANK_JACK;
}

bool Card::is_left_bower(const string& trump) const {
	assert(validate_suit_arg(trump));

	return get_rank() == Card::RANK_JACK && get_suit() == Suit_next(trump);
}

string Card::get_suit(const string& trump) const {
	assert(validate_suit_arg(trump));

	if (is_left_bower(trump)) {
		return trump;
	}
	else {
		return get_suit();
	}
}

bool Card::is_face() const {
	bool is_face_jack = get_rank() == Card::RANK_JACK;
	bool is_face_queen = get_rank() == Card::RANK_QUEEN;
	bool is_face_king = get_rank() == Card::RANK_KING;
	bool is_face_ace = get_rank() == Card::RANK_ACE;

	return is_face_jack || is_face_queen || is_face_king || is_face_ace;
}

bool Card::is_trump(const string& trump) const {
	assert(validate_suit_arg(trump));

	return get_suit() == trump || is_left_bower(trump);
}

bool Card_less(const Card& a, const Card& b, const string& trump) {
	assert(validate_suit_arg(trump));

	bool lhs_trump = a.is_trump(trump);
	bool rhs_trump = b.is_trump(trump);
	bool lhs_right_bower = a.is_right_bower(trump);
	bool rhs_right_bower = b.is_right_bower(trump);
	bool lhs_left_bower = a.is_left_bower(trump);
	bool rhs_left_bower = b.is_left_bower(trump);

	// first we catch the special case where the two cards are the same
	if (a == b) {
		return false;
	}

	if (!lhs_trump && !rhs_trump) {
		return a < b;
	} 
	else if (lhs_trump && !rhs_trump) {
		return false;
	} 
	else if (!lhs_trump && rhs_trump) {
		return true;
	} 
	else if (lhs_trump && rhs_trump) {

		// if either card is the right bower 
		// then it is definitely bigger
		if (lhs_right_bower || rhs_right_bower) {
			return !lhs_right_bower;

		// now we know neither card is the right bower
		// if the left bower is present it is definitely larger
		} else if (lhs_left_bower || rhs_left_bower) {
			return !lhs_left_bower;

		// now we know both cards are in the trump suit
		// but neither are the bowers
		// we can compare them directly then 
		} else {
			return a < b;
		}
	}
	return false;
}

bool Card_less(const Card& a, const Card& b, const Card& led_card,
	const string& trump) {
	assert(validate_suit_arg(trump));
	
	bool lhs_trump = a.is_trump(trump);
	bool rhs_trump = b.is_trump(trump);
	bool lhs_right_bower = a.is_right_bower(trump);
	bool rhs_right_bower = b.is_right_bower(trump);
	bool lhs_left_bower = a.is_left_bower(trump);
	bool rhs_left_bower = b.is_left_bower(trump);
	bool lhs_led_suit = a.get_suit(trump) == led_card.get_suit(trump);
	bool rhs_led_suit = b.get_suit(trump) == led_card.get_suit(trump);

	// first we catch the special case where the two cards are the same
	if (a == b) {
		return false;
	}

	if (lhs_trump && rhs_trump) {
		if (lhs_right_bower || rhs_right_bower) {
			return !lhs_right_bower;
		} else if (lhs_left_bower || rhs_left_bower) {
			return !lhs_left_bower;
		} else {
			return a < b;
		}
	} 
	else if (lhs_trump && !rhs_trump) {
		return false;
	}
	else if (!lhs_trump && rhs_trump) {
		return true;
	}
	else if (!lhs_trump && !rhs_trump) {
		if (lhs_led_suit && !rhs_led_suit) {
			return false;
		} else if (!lhs_led_suit && rhs_led_suit) {
			return true;
		} else {
			return a < b;
		}
	}
	return false;
}

/////////////////////////////////////////////////////////////////////////
//Operator Section
/////////////////////////////////////////////////////////////////////////

bool operator<(const Card& lhs, const Card& rhs) {
	if (find_index_rank(lhs.get_rank()) != find_index_rank(rhs.get_rank())) {
		return find_index_rank(lhs.get_rank()) < find_index_rank(rhs.get_rank());
	}
	else {
		return find_index_suit(lhs.get_suit()) < find_index_suit(rhs.get_suit());
	}
}

bool operator==(const Card& lhs, const Card& rhs) {
	return lhs.get_rank() == rhs.get_rank() && lhs.get_suit() == rhs.get_suit();
}

bool operator<=(const Card& lhs, const Card& rhs) {
	return lhs < rhs || lhs == rhs;
}

bool operator>(const Card& lhs, const Card& rhs) {
	return !(lhs <= rhs);
}

bool operator>=(const Card& lhs, const Card& rhs) {
	return !(lhs < rhs);
}

bool operator!=(const Card& lhs, const Card& rhs) {
	return !(lhs == rhs);
}

ostream& operator<<(ostream& os, const Card& card) {
	os << card.get_rank() << " of " << card.get_suit();
	return os;
}
// NOTE: We HIGHLY recommend you check out the operator overloading
// tutorial in the project spec (see the appendices) before implementing
// the following operator overload functions:
//   operator<<
//   operator<
//   operator<=
//   operator>
//   operator>=
//   operator==
//   operator!=

/////////////////Helper Functions/////////////////////
static int find_index_suit(string const suit) {
	assert(validate_suit_arg(suit));

	for (int i = 0; i < NUM_SUITS; i++) {
		if (suit == SUIT_NAMES_BY_WEIGHT[i]) {
			return i;
		}
	}
	return -1;
}

static int find_index_rank(string const rank) {
	assert(validate_rank_arg(rank));

	for (int i = 0; i < NUM_RANKS; i++) {
		if (rank == RANK_NAMES_BY_WEIGHT[i]) {
			return i;
		}
	}
	return -1;
}

static bool check_invariants(const string suit, const string rank) {
	return validate_suit_arg(suit) && validate_rank_arg(rank);
}

static bool validate_suit_arg(const string suit) {
	for (int i = 0; i < NUM_SUITS; i++) {
		if (suit == SUIT_NAMES_BY_WEIGHT[i]) {
			return true;
		}
	}
	return false;
}

static bool validate_rank_arg(const string rank) {
	for (int i = 0; i < NUM_RANKS; i++) {
		if (rank == RANK_NAMES_BY_WEIGHT[i]) {
			return true;
		}
	}
	return false;
}
