#include <fstream>
#include <iostream>
#include <memory>
#include <set>
#include <list>
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

// stl types
using std::string;
template<typename V> using shared_ptr = std::shared_ptr<V>;
template<typename V> using weak_ptr = std::weak_ptr<V>;
template<typename V> using set = std::set<V>;
template<typename V> using vector = std::vector<V>;
template<typename V> using list = std::list<V>;

// stl functions
using std::make_shared;
using std::move;
using std::swap;

const string SEED_END { "END OF INPUT" };
bool reading_seeds = true;


void add_line(string line) {
  if(reading_seeds) {
    if(SEED_END != line) {
    } else {
      reading_seeds = false;
    }
  } else {
  }
}

void run() {
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
