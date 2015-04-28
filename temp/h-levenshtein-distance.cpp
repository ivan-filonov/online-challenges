#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <vector>

#include <string.h>

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
template<typename V> using vector = std::vector<V>;
template<typename V> using set = std::set<V>;
template<typename K, typename V> using map = std::map<K,V>;

using std::move;
using std::swap;

vector<char> wbuf;
vector<size_t> wofs;

vector<string> seed_words;

const string SEED_END { "END OF INPUT" };
bool reading_seeds = true;
void add_line(string line) {
  if(reading_seeds) {
    if(SEED_END != line) {
      seed_words.emplace_back(move(line));
    } else {
      reading_seeds = false;
    }
  } else {
    const auto npos = wbuf.size();
    wbuf.resize(npos + line.length() + 1);
    strcpy(wbuf.data() + npos, line.c_str());
    const auto widx = wofs.size();
    wofs.push_back(npos);
  }
}

bool fw(const char * w1, const char * w2) {
  while(*w1 == *w2 && *w1) {
    ++w1;
    ++w2;
  }
  if(!*w2) {
    // word1 equals word2
    return true;
  }
  int l1 = strlen(w1); 
  int l2 = strlen(w2); 
  if(l1 == l2) {
    ++w1;
    ++w2;
    while(*w1 == *w2 && *w1) {
      ++w1;
      ++w2;
    }
    return 0 == *w1 && 0 == *w2;
  }
  if(l2 > l1) {
    swap(w1, w2);
    swap(l1, l2);
  }
  if(l1 - l2 != 1) {
    return false;
  }
  ++w1;
  while(*w1 == *w2 && *w1) {
    ++w1;
    ++w2;
  }
  return 0 == *w1 && 0 == *w2;
}

bool hit(const char * w, map<int,vector<char*>> & clu) {
  bool h = false;
  int wl = strlen(w);
  for(int cwl = wl - 1; cwl != wl + 2 ; ++cwl) {
    if(!clu.count(cwl)) {
      continue;
    }
    for(auto cw : clu[cwl]) {
      if(fw(w, cw)) {
        return true;
      }
    }
  }
  return false;
}

void run() {
  vector<char*> words (wofs.size());
  for(size_t i = 0; i != wofs.size(); ++i) {
    words[i] = wbuf.data() + wofs[i];
  }
  std::sort(words.begin(), words.end(), [](const char * a, const char * b) {
    int la = strlen(a), lb = strlen(b); 
    if(la == lb) {
      return strcmp(a,b) < 0;
    }
    return la < lb; });
  vector<map<int,vector<char*>>> clusters;
  vector<int> civ;
  for(auto w : words) {
    civ.clear();

    const int wl = strlen(w);
    for(size_t  ci = 0; ci != clusters.size(); ++ci) {
      if(hit(w, clusters[ci])) {
        civ.push_back(ci);
      }
    }
    if(civ.empty()) {
      // new cluster
      clusters.emplace_back();
      clusters.back()[wl].push_back(w);
    } else {
      clusters[civ.front()][wl].push_back(w);
      if(civ.size() > 1) {
        // combine clusters
        for(size_t i = 1; i != civ.size(); ++i) {
          // add cluster civ[i] to cluster civ[0]
          for(auto & p : clusters[civ[i]]) {
            auto & src = p.second;
            auto & dst = clusters[civ[0]][p.first];
            dst.insert(dst.end(), src.begin(), src.end());
          }
          clusters[civ[i]].clear();
        }
      }
    }
  }
  {
    decltype(clusters) temp;
    for(auto & x : clusters) {
      if(!x.empty()) {
        temp.emplace_back(move(x));
      }
    }
    temp.swap(clusters);
  }
  for(auto & sw : seed_words) {
    int c = 1;
    for(auto & cl : clusters) {
      if(hit(sw.c_str(), cl)) {
        for(auto & p : cl) {
          c += p.second.size();
        }
        break;
      }
    }
    std::cout << c << "\n";
  }
}

#ifdef TEST
void test() {
  vector<string> v_test {
    "recursiveness",    "elastic",
    "macrographies",    
    
    "END OF INPUT",
    
    "aa",    "aah",    "aahed",    "aahing",
    "aahs",    "aal",    "aalii",    "aaliis",
    "aals",    "aardvark",    "aardvarks",    "aardwolf",
    "elaborations",    "elaborative",    "elaborator",    "elaboratories",
    "elaborators",    "elaboratory",    "elaeolite",    "elaeolites",
    "elain",    "elains",    "elan",    "elance",
    "elands",    "elanet",    "elanets",    "elans",
    "elaphine",    "elapid",    "elapids",    "elapine",
    "elapse",    "elapsed",    "elapses",    "elapsing",
    "elasmobranch",    "elasmobranches",    "elasmobranchs",    "elastance",
    "elastances",    "elastase",    "elastases",    "elastic",
    "elastically",    "elasticate",    "elasticated",    "elasticates",
    "elasticating",    "elasticise",    "elasticised",    "elasticises",
    "elasticising",    "elasticities",    "elasticity",    "elasticize",
    "elasticized",    "elasticizes",    "elasticizing",    "elasticness",
    "elasticnesses",    "elastics",    "elastin",    "elastins",
    "elastomer",    "elastomeric",    "elastomers",    "elate",
    "electromyography",    "electron",    "electronegative",    "electronegativities",
    "electronegativity",    "electronic",    "electronically",    "electronics",
    "electrooculography",    "electrooptics",    "electroosmoses",    "electroosmosis",
    "electrophiles",    "electrophilic",    "electrophilicities",    "electrophilicity",
    "electrophoresis",    "electrophoretic",    "electrophoretically",    "electrophoretogram",
    "electrophotographic",    "electrophotographies",    "electrophotography",    "electrophysiologic",
    "electrophysiologists",    "electrophysiology",    "electroplate",    "electroplated",
    "zymosan",    "zymosans",    "zymoses",    "zymosimeter",
    "zymotechnics",    "zymotic",    "zymotically",    "zymotics",
    "zythums",    "zyzzyva",    "zyzzyvas",
  };
  for(auto & t : v_test) {
    add_line(t);
  }
  run();
  for(int i = 0; i < 80; ++i)std::cout<<'-';std::cout<<"\n";
  vector<string> v_expect {
    "1",
    "3",
    "1",
  };
  for(auto & e : v_expect) {
    std::cout << e << "\n";
  }
}
#endif //#ifdef TEST

void process_file(char* path) {
  std::ifstream stream(path);
  for(string line; std::getline(stream, line); ) {
    add_line(line);
  }
  run();
}
