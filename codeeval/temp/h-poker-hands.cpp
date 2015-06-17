#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#ifdef TEST
void test();
#endif //#ifdef TEST
void process_file(char*);

int main(int argc, char ** argv) {
#ifdef TEST
  test();
#else //#ifdef TEST
  process_file(argv[1]);
#endif //#ifdef TEST
	return 0;
}

using std::string;
// required when TEST is defined
template<typename V> using vector = std::vector<V>;

string ranks { "23456789TJQKA" };
string suites{ "DHCS" };

bool less_by_rank (const string & c1, const string & c2) {
  if(c1[0] == c2[0]) { // same rank
    return c2[1] < c1[1];
  }
  return ranks.find(c2[0]) < ranks.find(c1[0]);
};

enum class hr { 
  highest_card,    //# High Card: Highest value card.
  one_pair,        //# One Pair: Two cards of the same value.
  two_pairs,       //# Two Pairs: Two different pairs.
  three_of_a_kind, //# Three of a Kind: Three cards of the same value.                
  straight,        //# Straight: All cards are consecutive values.
  flush,           //# Flush: All cards of the same suit.
  full_house,      //# Full House: Three of a kind and a pair.
  four_of_a_kind,  //# Four of a Kind: Four cards of the same value.
  straight_flush,  //# Straight Flush: All cards are consecutive values of same suit.
  royal_flush      //# Royal Flush: Ten, Jack, Queen, King, Ace, in same suit.
};

struct hand_t {
  vector<string> initial;
  vector<string> same_1;
  vector<string> same_2;
  bool is_flush;
  bool is_straight;

  void put(string && card) {
    initial.emplace_back(std::move(card));
  }

  void calc() {
    // 1. sort by suite:
    std::sort(initial.begin(), initial.end(), [](const string & c1, const string & c2) { return c1[1] < c2[1]; });
    int cur_suite = 0;
    int suite_cnt = 0;
    int suite_max = 0;
    for(auto & card : initial) {
      if(card[1] == cur_suite) {
        ++suite_cnt;
        suite_max = std::max(suite_max, suite_cnt);
      } else {
        cur_suite = card[1];
        suite_cnt = 1;
      }
    }
    is_flush = 5 == suite_max;
    // 2. sort by rank descending:
    std::sort(initial.begin(), initial.end(), less_by_rank);
    for(auto & card : initial) {
      card += (char)('a' + ranks.find(card[0]));
    }
    // check for is_straight:
    is_straight = true;
    for(int i = 1; (i != initial.size()) && is_straight; ++i) {
      is_straight = 1 == ranks.find(initial[i-1][0]) - ranks.find(initial[i][0]);
    }
    // check for groups:
    int group_rank = -1;
    int group_size = 0;
    for(auto & card : initial) {
      if(card[2] - 'a' == group_rank) {
        ++group_size;
      } else {
        group_rank = card[2] - 'a';
        group_size = 1;
        //TODO: grow group
      }

      }
    }

  }
};

void process(string line) {
#ifdef TEST
  std::cout << "s = '" << line << "'\n";
#endif //#ifdef TEST
  std::istringstream ss { line };
  
  hand_t left;
  for(int i = 0; i < 5; ++i) {
    string card;
    ss >> card;
    left.put(std::move(card));
  }
  left.calc();

  hand_t right;
  for(int i = 0; i < 5; ++i) {
    string card;
    ss >> card;
    right.put(std::move(card));
  }
  right.calc();
}

#ifdef TEST
void test() {
  vector<string> v_test {
    "6D 7H AH 7S QC 6H 2D TD JD AS",
    "JH 5D 7H TC JS JD JC TS 5S 7S",
    "2H 8C AD TH 6H QD KD 9H 6S 6C",
    "JS JH 4H 2C 9H QH KC 9D 4D 3S",
    "TC 7H KH 4H JC 7D 9S 3H QS 7S",
    "AH KH QH JH TH JD TD AD KD QD",
  };
  vector<string> v_expect {
    "left",
    "none",
    "right",
    "left",
    "right",
    "none",
  };
  for(int i = 0, j = std::min(v_test.size(), v_expect.size()); i < j; ++i) {
    process(v_test[i]);
    std::cout << v_expect[i] << "\n";
  }
}
#endif //#ifdef TEST

void process_file(char* path) {
  std::ifstream stream(path);
  for(string line; std::getline(stream, line); ) {
    process(line);
  }
}
