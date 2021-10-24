// Project UID 1d9f47bfc76643019cfbf037641defe1

#include <iostream>
#include <string>
#include <vector>
#include <cassert>
#include <fstream>
#include "Card.h"
#include "Pack.h"
#include "Player.h"
using namespace std;


// helper function declaration //
static bool validate_commandline_args(const int argc, char* const argv[]);
static void print_commandline_args(char* const argv[]);
static int find_winner_index(const vector<Card> cards, const string trump, 
                            const int leader);
static bool validate_suit_arg(const string suit);


class Game {
public:
    
    // class constructor function 
    Game(Pack pack, bool shuffle, int point_to_win, vector<Player*> players)
    :pack(pack), shuffle(shuffle), point_to_win(point_to_win), players(players),
    teamA_point(0), teamB_point(0), hand_num(0), dealer(0), eldest_hand(1) {
        assert(0 < point_to_win && point_to_win <= 100);
    }

    // MODIFIES upcard
    // EFFECTS shuffle the pack if requested, populate player hands
    void shuffle_and_deal(Card &upcard) {
        if (shuffle) {
            pack.shuffle();
            pack.reset();
        } else {
            pack.reset();
        }

        // first round of dealing (3-2-3-2)
        for (int i = 0; i < 4; i++) {
            int dealing_to = (eldest_hand+i) % 4;

            if (i % 2 == 0) {
                for (int j = 0; j < 3; j++) {
                    players[dealing_to]->add_card(pack.deal_one());
                }
            } else {
                for (int j = 0; j < 2; j++) {
                    players[dealing_to]->add_card(pack.deal_one());
                }
            }
        }

        // second round of dealing (2-3-2-3)
        for (int i = 0; i < 4; i++) {
            int dealing_to = (eldest_hand+i) % 4;

            if (i % 2 == 0) {
                for (int j = 0; j < 2; j++) {
                    players[dealing_to]->add_card(pack.deal_one());
                }
            } else {
                for (int j = 0; j < 3; j++) {
                    players[dealing_to]->add_card(pack.deal_one());
                }
            }
        }

        upcard = pack.deal_one();

        cout << "Hand " << hand_num << endl;
        cout << *players[dealer] << " deals" << endl;
        cout << upcard << " turned up" << endl;
    }

    // make trump
    // calls make trump round 1 first 
    // if returns false, call round 2
    string make_trump(const Card upcard, int &order_up) {
        string trump = "";

        if (!make_trump_round1(upcard, trump, order_up)) {
            make_trump_round2(upcard, trump, order_up);
        }

        cout << endl;

        return trump;
    }

    // make trump round 1 
    bool make_trump_round1(const Card upcard, string &trump, int &order_up) {
        for (int i = 0; i < 4; i++) {
            int ordering = (eldest_hand + i) % 4;
            
            if (ordering == dealer) {
                if (players[ordering]->make_trump(upcard, true, 1, trump)) {
                    order_up = ordering;
                    cout << *players[order_up] << " orders up " << trump << endl;
                    players[dealer]->add_and_discard(upcard);
                    return true;
                } else {
                    cout << *players[ordering] << " passes" << endl;
                }
            } else {
                if (players[ordering]->make_trump(upcard, false, 1, trump)) {
                    order_up = ordering;
                    cout << *players[order_up] << " orders up " << trump << endl;
                    players[dealer]->add_and_discard(upcard);                    
                    return true;
                } else {
                    cout << *players[ordering] << " passes" << endl;
                }
            }
        }
        return false;
    }

    // make trump round 2 
    bool make_trump_round2(const Card upcard, string &trump, int &order_up) {
        for (int i = 0; i < 4; i++) {
            int ordering = (eldest_hand + i) % 4;
            if (ordering == dealer) {
                if (players[ordering]->make_trump(upcard, true, 2, trump)) {
                    order_up = ordering;
                    cout << *players[order_up] << " orders up " << trump << endl;
                    return true;
                } 
            } else {
                if (players[ordering]->make_trump(upcard, false, 2, trump)) {
                    order_up = ordering;
                    cout << *players[order_up] << " orders up " << trump << endl;
                    return true;
                } else {
                    cout << *players[ordering] << " passes" << endl;
                }
            }
        }
        return false;
    }

    // play trick 
    vector<Card> play_trick(const string trump, const int leader) {
        assert(validate_suit_arg(trump));
        assert(0 <= leader && leader < 4);

        vector<Card> cards_played;
        
        // determine lead card
        Card lead_card = players[leader]->lead_card(trump);
        cout << lead_card << " led by " << *players[leader] << endl;
        cards_played.push_back(lead_card);
        
        // iterate through the rest of the players
        for (int i = 1; i < 4; i++) {
            int playing = (leader + i) % 4;
            Card card_played = players[playing]->play_card(lead_card, trump);
            cards_played.push_back(card_played);
            cout << card_played << " played by " << *players[playing] << endl;
        }

        return cards_played;
    }

    // find the player that won the trick
    // use in conjunction with play trick
    int find_trick_winner(const vector<Card> cards, const string trump,
        const int leader) {
        assert(validate_suit_arg(trump));
        assert(0 <= leader && leader < 4);

        return find_winner_index(cards, trump, leader);
    }


    // award the team of the trick winner 
    void award_trick_winner_team(const int winner, int &teamA_trick, int &teamB_trick) {
        assert(0 <= winner && winner < 4);

        if (winner % 2 == 0) {
            teamA_trick += 1;
        } else {
            teamB_trick += 1;
        }
    }

    // announce the team that has won the hand
    void announce_hand_winner(const int teamA_trick, const int teamB_trick) {
        assert((teamA_trick + teamB_trick) == 5);

        if (teamA_trick > teamB_trick) {
            cout << *players[0] << " and " << *players[2] << " win the hand" << endl;
        } else {
            cout << *players[1] << " and " << *players[3] << " win the hand" << endl;
        }
    }

    // calculate points, do appropriate output after finishing playing a hand
    // use in conjunction with play hand
    void award_winner(const int teamA_trick, const int teamB_trick, const int order_up) {
        assert(0 <= order_up && order_up < 4);
        assert(0 <= teamA_trick && teamA_trick <= 5);
        assert(0 <= teamB_trick && teamB_trick <= 5);
        assert((teamA_trick + teamB_trick) == 5);

        bool teamA_order_up = (order_up % 2) == 0;
        bool teamA_won = teamA_trick > teamB_trick;

        if (teamA_won) {
            if (teamA_order_up) {
                if (teamA_trick< 5) {
                    teamA_point += 1;
                } else {
                    teamA_point += 2;
                    cout << "march!" << endl;
                }
            } else {
                teamA_point += 2;
                cout << "euchred!" << endl;
            }
        } else {
            if (teamA_order_up) {
                teamB_point += 2;
                cout << "euchred!" << endl;
            } else {
                if (teamB_trick < 5) {
                    teamB_point += 1;
                } else {
                    teamB_point += 2;
                    cout << "march!" << endl;
                }
            }
        }
    }

    // print updated scores of both teams
    void print_score() {
        cout << *players[0] << " and " << *players[2] << " have " 
                           << teamA_point << " points" << endl;
        cout << *players[1] << " and " << *players[3] << " have " 
                           << teamB_point << " points" << endl;
        cout << endl; 
    }

    // play hand 
    void play_hand() {
        Card upcard;
        string trump;
        int teamA_trick = 0;
        int teamB_trick = 0;
        int order_up = 0;
        int trick_winner = 0;
        int leader = eldest_hand;

        shuffle_and_deal(upcard);

        trump = make_trump(upcard, order_up);

        for (int i = 0; i < 5; i++) {
            vector<Card> cards_played = play_trick(trump, leader);

            trick_winner = find_trick_winner(cards_played, trump, leader);
            award_trick_winner_team(trick_winner, teamA_trick, teamB_trick);
            leader = trick_winner;

            cout << *players[trick_winner] << " takes the trick" << endl;
            cout << endl;
        }
        
        announce_hand_winner(teamA_trick, teamB_trick);
        award_winner(teamA_trick, teamB_trick, order_up);
        print_score();
        hand_num++;
        dealer++;
        dealer = dealer % 4;
        eldest_hand = (dealer + 1) % 4;
    }

    // check game is not over
    bool check_game_end() {
        return teamA_point >= point_to_win || teamB_point >= point_to_win;
    }

    // check which team wins and announce it
    void announce_game_winner() {
        if (teamA_point >= point_to_win) {
            cout << *players[0] << " and " << *players[2] << " win!" << endl;
        } else if (teamB_point >= point_to_win) {
            cout << *players[1] << " and " << *players[3] << " win!" << endl;;
        } 
    }

    // terminate game
    // go into players vector and delete each player
    void terminate_game() {
        for (int i = 0; i < 4; i++) {
            delete players[i];
        }
    }

private:
    // teamA contains player 0 and 2
    // teamB contains player 1 and 3
    Pack pack;
    bool shuffle;
    int point_to_win;
    vector<Player*> players;
    int teamA_point;
    int teamB_point;
    int hand_num;
    int dealer;
    int eldest_hand;
};


static Game setup_game(char* const argv[], istream& is);


int main(int argc, char* argv[]) {
    if (!validate_commandline_args(argc, argv)) {
        cout << "Usage: euchre.exe PACK_FILENAME [shuffle|noshuffle] "
             << "POINTS_TO_WIN NAME1 TYPE1 NAME2 TYPE2 NAME3 TYPE3 "
             << "NAME4 TYPE4" << endl;
        return 1;
    }

    string pack_filename = argv[1];
    ifstream fin;
    fin.open(pack_filename);

    if (!fin.is_open()) {
        cout << "Error opening " << pack_filename << endl;
        return 2;
    } else {
        print_commandline_args(argv);
    }
    
    // finished validating command line arguments
    // actual game implementation is below 

    //initiate game object
    Game new_game = setup_game(argv, fin);

    // use a while true loop to play
    while (true) {
        new_game.play_hand();

        if (new_game.check_game_end()) {
            break;
        }
    }
    new_game.announce_game_winner();
    new_game.terminate_game();

    return 0;
}   


// helper function implementations
static bool validate_commandline_args(const int argc, char* const argv[]) {
    if (argc != 12) {
        return false;
    }

    int point_to_win = atoi(argv[3]);
    if (!(0 < point_to_win && point_to_win <= 100)) {
        return false;
    }

    string shuffle = argv[2];
    if (shuffle != "shuffle" && shuffle != "noshuffle") {
        return false;
    }

    for (int i = 5; i < argc; i+=2) {
        string player_type = argv[i];
        if (player_type != "Simple" && player_type != "Human") {
            return false;
        } 
    }
    return true;
}

static void print_commandline_args(char* const argv[]) {
    for (int i = 0; i < 12; i++) {
        string arg = argv[i];
        cout << arg << " ";
    }
    cout << endl;
}

// process command line arguments and create appropriate Game object
static Game setup_game(char* const argv[], istream& is) {

    // we are sure that argv[2] is either shuffle or no shuffle
    string shuffle_arg = argv[2];
    bool shuffle = shuffle_arg == "shuffle";

    int point_to_win = atoi(argv[3]);

    Pack pack(is);

    vector<Player*> players;

    for (int i = 4; i < 11; i+=2) {
        string player_name = argv[i];
        string player_type = argv[i+1];

        Player * new_player = Player_factory(player_name, player_type);
        players.push_back(new_player);
    }

    // init and return game
    Game new_game = Game(pack, shuffle, point_to_win, players);
    return new_game;
}

// REQUIRES trump is a valid suit, cards contain unique entries only
//  leader is between 0 and 3 (inclusive)
// EFFECTS find the vector index of the highest value card
//   considers both lead suit and trump suit
static int find_winner_index(const vector<Card> cards, const string trump, 
                             const int leader) {
    assert(0 <= leader && leader < 4);
    assert(validate_suit_arg(trump));
    assert(cards.size() == 4);

    Card lead_card = cards[0];
    int max_index = 0;
    Card max_card = cards[0];

    for (int i = 1; i < 4; i++) {
        if (Card_less(max_card, cards[i], lead_card, trump)) {
            max_index = i;
            max_card = cards[i];
        }
    }

    // for example, player 1 is leader 
    // the third player to play a card wins the trick 
    // his card is at index 2 in cards
    // the index of the winner is 3
    return (max_index + leader) % 4;
}

static bool validate_suit_arg(const string suit) {
    bool check0 = suit == Card::SUIT_CLUBS;
    bool check1 = suit == Card::SUIT_DIAMONDS;
    bool check2 = suit == Card::SUIT_HEARTS;
    bool check3 = suit == Card::SUIT_SPADES;

    return check0 || check1 || check2 || check3;
}
