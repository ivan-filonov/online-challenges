/*
 * Common base C++ source for codeeval solutions.
 * */
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <map>
#include <memory>
#include <set>
#include <sstream>
#include <string>
#include <vector>

void test();
void process_file(char*);
int main(int argc, char ** argv) {
    if( !1 ) {
	    test();
    } else {
	    process_file(argv[1]);
    }
	return 0;
}

void add_line(const std::string &line);
void print_suggestions();

void test() {
    add_line("Amira:Isaura,Lizzie,Madalyn,Margarito,Shakira,Un:Driving,Mineral collecting");
    add_line("Elliot:Isaura,Madalyn,Margarito,Shakira:Juggling,Mineral collecting");
    add_line("Isaura:Amira,Elliot,Lizzie,Margarito,Verla,Wilford:Juggling");
    add_line("Lizzie:Amira,Isaura,Verla:Driving,Mineral collecting,Rugby");
    add_line("Madalyn:Amira,Elliot,Margarito,Verla:Driving,Mineral collecting,Rugby");
    add_line("Margarito:Amira,Elliot,Isaura,Madalyn,Un,Verla:Mineral collecting");
    add_line("Shakira:Amira,Elliot,Verla,Wilford:Mineral collecting");
    add_line("Un:Amira,Margarito,Wilford:");
    add_line("Verla:Isaura,Lizzie,Madalyn,Margarito,Shakira:Driving,Juggling,Mineral collecting");
    add_line("Wilford:Isaura,Shakira,Un:Driving");

    print_suggestions();

    std::cout << "Isaura:Driving,Mineral collecting"    "\n";
	std::cout << "Lizzie:Juggling"    "\n";
	std::cout << "Madalyn:Juggling"    "\n";
	std::cout << "Margarito:Driving,Juggling"    "\n";
	std::cout << "Shakira:Driving,Juggling"    "\n";
	std::cout << "Un:Driving,Mineral collecting"    "\n";
}

namespace {
    template<typename _1, typename _2> using map_t = std::map<_1,_2>;
    typedef std::pair<int,int> ref_t;
    template<typename _1> using set_t = std::set<_1>;

    map_t<int,std::string> user_by_id;
    map_t<std::string,int> user_by_name;

    map_t<int,std::string> group_by_id;
    map_t<std::string,int> group_by_name;

    map_t<int, set_t<int>> friends;
    map_t<int, set_t<int>> groups;

    int user(const std::string& name) {
        auto it = user_by_name.find(name);
        if(user_by_name.end() != it) {
            return it->second;
        }

        // add user
        auto id = user_by_name[name] = user_by_name.size();
        user_by_id[id] = name;

        // add relations
        friends[id];
        groups[id];

        return id;
    }

    void make_friends(const int uid1, const int uid2) {
        friends[uid1].insert(uid2);
        friends[uid2].insert(uid1);
    }

    int group(const std::string& name) {
        auto it = group_by_name.find(name);
        if(group_by_name.end() != it) {
            return it->second;
        }

        // add group
        auto id = group_by_name[name] = group_by_name.size();
        group_by_id[id] = name;

        return id;
    }
}

void process_file(char* path) {
	std::ifstream stream(path);
	for(std::string line; std::getline(stream, line); ) {
        add_line(line);
	}
    print_suggestions();
}

void add_line(const std::string &line) {
    const auto dname = line.find(':');
    const auto name = line.substr(0, dname);
    const auto uid = user(name);
    const auto dfriends = line.find(':', dname + 1);
    for(int i = dname + 1; i < dfriends;) {
        int j = line.find(',', i + 1);
        if(j > dfriends) {
            j = dfriends;
        }
        const auto fname = line.substr(i, j - i);
        const auto uf = user(fname);
        i = j + 1;

        make_friends(uid, uf);
    }
    for(int i = dfriends + 1; i < line.length();) {
        int j = line.find(',', i + 1);
        if(j > line.length()) {
            j = line.length();
        }
        const auto gname = line.substr(i, j - i);
        i = j + 1;

        groups[uid].insert(group(gname));
    }
}

void print_suggestions() {
    // for each user:
    for(auto it_user = user_by_name.begin(); user_by_name.end() != it_user; ++it_user) {
        const auto uid = it_user->second;
        const auto uname = it_user->first;
        const auto flist = friends[uid];
        const auto glist = groups[uid];
//        std::cout << "UID " << uid << ", UNAME " << uname << ", flist: " << flist.size() << ", glist: " << glist.size() << "\n";

        map_t<int,int> g2c;
        for(auto it_f = flist.begin(); flist.end() != it_f; ++it_f) {
            auto fglist = groups[*it_f];
            for(auto it_fg = fglist.begin(); fglist.end() != it_fg; ++it_fg) {
                g2c[*it_fg]++;
            }
        }
        int threshold = (flist.size() + 1 ) / 2 - 1;

        std::vector<std::string> sgn;
        for(auto it_g = group_by_name.begin(); group_by_name.end() != it_g; ++it_g) {
            const auto gid = it_g->second;
            const auto gname = it_g->first;
            if(glist.count(it_g->second) > 0) {
                continue;
            }
            if(g2c[gid] > threshold) {
                sgn.push_back(gname);
            }
        }

        if(sgn.empty()) {
            continue;
        }

        std::cout << uname << ':';
        for(int i = 0; i < sgn.size(); ++i) {
            if(i) {
                std::cout << ',';
            }
            std::cout << sgn[i];
        }
        std::cout << "\n";
    }
}
