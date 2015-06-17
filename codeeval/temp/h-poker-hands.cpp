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

bool  sort_by (const string & c1, const string & c2) {
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
  int m[13][4];
  int per_suite_counts[4];
  int per_rank_counts[13];
  int chain_prefix[13];
  int num_chains[5];
  hr  hand_rank;
  int straight_rank;
  int pair1_rank;
  int pair2_rank;
  int triple_rank;
  int four_rank;

  void calc_hand_rank() {
    hand_rank = hr::highest_card;
    for(int suite = 0; suite < 4; ++suite) {
      if(per_suite_counts[suite] == 5) {
        hand_rank = hr::flush;
      }
    }
    if(num_chains[4]) {
      for(int rank = 4; rank < 13; ++rank) {
        if(5 == chain_prefix[rank]) {
          straight_rank = rank;
          break;
        }
      }
      if(hand_rank == hr::flush) {
        hand_rank = hr::straight_flush;
        if(per_rank_counts[12]) {
          hand_rank = hr::royal_flush;
        }
      } else {
        hand_rank = hr::straight;
      }
    }
    if(hr::highest_card == hand_rank) {
      int pairs = 0;
      int triples = 0;
      int fours = 0;
      for(int rank = 0; rank < 13; ++rank) {
        switch(per_rank_counts[rank]) {
          case 2: 
            if(pair1_rank) {
              pair2_rank = rank;
              if(pair2_rank > pair1_rank) {
                std::swap(pair1_rank, pair2_rank);
              }
            } else {
              pair1_rank = rank;
            }
            ++pairs; 
            break;
          case 3: 
            triple_rank = rank;
            ++triples; 
            break;
          case 4: 
            four_rank = rank;
            ++fours;
            break;            
        }
      }
      if(fours) {
        hand_rank = hr::four_of_a_kind;
      }
      if(triples) {
        hand_rank = hr::three_of_a_kind;
        if(pairs) {
          hand_rank = hr::full_house;
        }
      }
      if(pairs) {
        hand_rank = hr::one_pair;
        if(pairs > 1) {
          hand_rank = hr::two_pairs;
        }
      }
    }
  }

  int compare_by_highest(const hand_t & other) const {
    for(int rank = 12; rank >= 0; --rank) {
      if(per_rank_counts[rank] != other.per_rank_counts[rank]) {
        return per_rank_counts[rank] - other.per_rank_counts[rank];
      }
    }
    return 0;
  }

  hand_t() : m { 0, }, 
             per_suite_counts { 0, }, 
             per_rank_counts { 0, }, 
             chain_prefix { 0, }, 
             num_chains { 0, },
             straight_rank (0),
             pair1_rank (0),
             pair2_rank (0),
             triple_rank (0),
             four_rank (0)
             { }

  void put(string && card) {
    int rank = ranks.find(card[0]);
    int suite = suites.find(card[1]);
    m[rank][suite] = 1;
  }

  void precalc() {
    for(int rank = 0; rank < 13; ++rank) {
      for(int suite = 0; suite < 4; ++suite) {
        per_suite_counts[suite] += m[rank][suite];
        per_rank_counts[rank] += m[rank][suite];
      }
      if(per_rank_counts[rank]) {
        chain_prefix[rank] = 1;
        if(rank && per_rank_counts[rank-1]) {
          chain_prefix[rank] = chain_prefix[rank-1] + 1;
          chain_prefix[rank-1] = 0;
        }
      }
    }
    for(int rank = 0; rank < 13; ++rank) {
      if(chain_prefix[rank]) {
        num_chains[chain_prefix[rank]-1]++;
      }
    }
    calc_hand_rank();
  }
};

void process(string line) {
#ifdef TEST
  std::cout << "s = '" << line << "'\n";
#endif //#ifdef TEST
  std::istringstream ss { line };
  hand_t left;
  hand_t right;
  for(int i = 0; i < 5; ++i) {
    string card;
    ss >> card;
    left.put(std::move(card));
  }
  for(int i = 0; i < 5; ++i) {
    string card;
    ss >> card;
    right.put(std::move(card));
  }
  left.precalc();
  right.precalc();
  if(left.hand_rank > right.hand_rank) {
    std::cout << "left\n";
  } else if(left.hand_rank < right.hand_rank) {
    std::cout << "right\n";
  } else {
    int comp = 0;
    switch(left.hand_rank) {
      case hr::highest_card:
        comp = left.compare_by_highest(right);
        break;
      case hr::one_pair:
        comp = left.pair1_rank - right.pair1_rank;
        if(!comp) {
          comp = left.compare_by_highest(right);
        }
        break;
      case hr::two_pairs:
        comp = left.pair1_rank - right.pair1_rank;
        if(!comp) {
          comp = left.pair2_rank - right.pair2_rank;
        }
        if(!comp) {
          comp = left.compare_by_highest(right);
        }
        break;
      case hr::three_of_a_kind:
        comp = left.triple_rank - right.triple_rank;
        if(!comp) {
          comp = left.compare_by_highest(right);
        }
        break;
      case hr::flush:
        comp = left.compare_by_highest(right);
        break;
      case hr::full_house:
        comp = left.triple_rank - right.triple_rank;
        if(!comp) {
          comp = left.pair1_rank - right.pair1_rank;
        }
        if(!comp) {
          comp = left.compare_by_highest(right);
        }
        break;
      case hr::four_of_a_kind:
        comp = left.four_rank - right.four_rank;
        if(!comp) {
          comp = left.compare_by_highest(right);
        }
        break;
      case hr::straight:
      case hr::straight_flush:
        comp = left.straight_rank - right.straight_rank;
        break;
    };
    if(comp < 0) {
      std::cout << "right\n";
    } else if(!comp) {
      std::cout << "none\n";
    } else {
      std::cout << "left\n";
    }
  }
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
