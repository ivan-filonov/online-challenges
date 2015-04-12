#include <fstream>
#include <iostream>
#include <set>
#include <unordered_map>
#include <vector>

namespace {
  using string = std::string;
  template<typename K, typename V> using map = std::unordered_map<K,V>;
  template<typename V> using vector = std::vector<V>;
  template<typename V> using set = std::set<V>;

  bool same(const set<string> &s1, const set<string> &s2) {
    if(s1.size() != s2.size()) {
      return false;
    }
    auto it1 = s1.begin();
    auto it2 = s2.begin();
    auto e1 = s1.end();
    while(it1 != e1) {
      if(*it1 != *it2) {
        return false;
      }
      ++it1;
      ++it2;
    }
    return true;
  }

  struct cluster {
    set<string> core;
    set<string> other;
  };

  struct state {
    void add(const string &line) {
  //    std::istringstream ss { line };
      const auto tab1 = line.find('\t');
      const auto tab2 = line.find('\t', tab1 + 1);
      auto u1 = line.substr(tab1 + 1, tab2 - tab1 - 1);
      auto u2 = line.substr(tab2 + 1);
      add(std::move(u1), std::move(u2));
    }

    set<string> users;
    map<string,set<string>> rels;
    void add(string &&u1, string &&u2) {
      users.insert(u1);
      users.insert(u2);
      rels[u1].insert(u2);
    }

    void run() {
      // 1. build initial clusters
      vector<cluster> clusters;
      for(auto & rp : rels) {
        clusters.emplace_back();
        auto & c = clusters.back();
        c.core.insert(rp.first);
        for(auto & oc : rp.second) {
          if(rels.count(oc) && rels[oc].count(rp.first)) {
            c.other.insert(oc);
          }
        }
      }
      // 2. while it's possible to grow at least one cluster:
      for(bool repeat = true; repeat; ) {
        // for each cluster remove users that cannot be added to it
        for(auto & cl : clusters) {
          for(auto it = cl.other.begin(); it != cl.other.end();) {
            // try to pair (*it) with all core users:
            const auto & other = *it;
            bool rm = false;
            for(auto & cu : cl.core) {
              if(rels[other].count(cu) && rels[cu].count(other)) {
                continue;
              }
              rm = true;
              break;
            }
            if(rm) {
              it = cl.other.erase( it );
            } else {
              ++it;
            }
          }
        }
        // now for each cluster we can add any of 'other' users to 'core'
        vector<cluster> new_clusters;
        for(auto cit = clusters.begin(); cit != clusters.end();) {
          if(cit->other.empty()) {
            ++cit;
            continue;
          }
          for(auto & ou : cit->other) {
            new_clusters.emplace_back( *cit );
            new_clusters.back().core.insert(ou);
            new_clusters.back().other.erase(ou);
          }
          cit = clusters.erase( cit );
        }
        repeat = !new_clusters.empty();
        // now move new clusters to the list
        for(auto & nc : new_clusters) {
          bool dup = false;
          for(auto & c : clusters) {
            if(dup |= same(nc.core, c.core)) {
              break;
            }
          }
          if(!dup) {
            clusters.emplace_back(std::move(nc));
          }
        }
      }
      set<string> res;
      for(auto & c : clusters) {
        if(c.core.size() < 3) {
          continue;
        }
        string s;
        for(auto & u : c.core) {
          if(!s.empty()) s += ", ";
          s += u;
        }
        res.insert(s);
      }
      for(auto & s : res) {
        std::cout << s << "\n";
      }
    }
  };
}

int main(int argc, char ** argv) {
  std::ifstream stream(argv[1]);
  state st;
  for(std::string line; std::getline(stream, line); ) {
    st.add(line);
  }
  st.run();
	return 0;
}

