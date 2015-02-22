/*
 * Common base C++ source for codeeval solutions.
 * */
#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>

#include <regex.h>
#include <cstring>

void test();
void process_file(char*);
int main(int argc, char ** argv) {
    if( 0 ) {
        test();
    } else {
        process_file(argv[1]);
    }
	return 0;
}

void process(std::string s);

void test() {
  process("*7* johab.py gen_probe.ko palmtx.h macpath.py tzp dm-dirty-log.h bh1770.h pktloc faillog.8.gz zconf.gperf");
  std::cout << "bh1770.h"    "\n";
  
  process("*[0123456789]*[auoei]* IBM1008_420.so zfgrep limits.conf.5.gz tc.h ilogb.3.gz limits.conf CyrAsia-TerminusBold28x14.psf.gz nf_conntrack_sip.ko DistUpgradeViewNonInteractive.pyc NFKDQC");
  std::cout << "IBM1008_420.so"    "\n";
  
  process("*.??? max_user_watches arptables.h net_namespace Kannada.pl menu_no_no.utf-8.vim shtags.1 unistd_32_ia32.h gettext-tools.mo ntpdate.md5sums linkat.2.gz");
  std::cout << "menu_no_no.utf-8.vim"    "\n";
  
  process("*.pdf OldItali.pl term.log plymouth-upstart-bridge rand.so libipw.ko jisfreq.pyc impedance-analyzer xmon.h 1.5.0.3.txt bank");
  std::cout << "-"    "\n";
  
  process("g*.* 56b8a0b6.0 sl.vim digctl.h groff-base.conffiles python-software-properties.md5sums CountryInformation.py use_zero_page session-noninteractive d2i_RSAPublicKey.3ssl.gz container-detect.log.4.gz");
  std::cout << "groff-base.conffiles"    "\n";
  
  process("*[0123456789]* keyboard.h machinecheck 46b2fd3b.0 libip6t_frag.so timer_defs.h nano-menu.xpm NI vim-keys.conf setjmp.h memcg");
  std::cout << "46b2fd3b.0 libip6t_frag.so"    "\n";
}

void process_file(char* path) {
	std::ifstream stream(path);
	for(std::string line; std::getline(stream, line); ) {
		process(line);
	}
}

void process(std::string line) {
  std::vector<char> buf(line.length()+1, 0);
  std::copy(line.begin(), line.end(), buf.begin());
  std::string base_exp = strtok(buf.data(), " ");
  std::vector<std::string> v;
  for(char * t;t = strtok(0," ");) {
      v.push_back(t);
  }

  //prepare regexp
  std::string pattern;
  static const std::string need_escape {".+(){}\\"};
  pattern.reserve(base_exp.length());
  for(auto it = std::begin(base_exp), end = std::end(base_exp); end != it; ++it) {
    if('*' == *it) {
      pattern += ".*";
    } else if('?' == *it) {
      pattern += ".";
    } else if('[' == * it) {
      while(']' != *it) {
        pattern += *it++;
      }
      pattern += ']';
    } else {
      if(need_escape.find(*it) != std::string::npos) {
        pattern += '\\';
      }
      pattern += *it;
    }
  }

  regex_t rx;
  if(regcomp(&rx, pattern.c_str(), 0)) {
    throw std::exception();
  }
//  std::regex rx(pattern);
  bool mid = false;
  for(auto &s : v) {
    regmatch_t m;
    if(regexec(&rx, s.c_str(), 1, &m, 0)) {
//    if(!std::regex_match(s,rx)) {
      continue;
    }
    if(m.rm_so > 0 || m.rm_eo <s.length()) {
      continue;
    }
    if(mid) {
      std::cout << ' ';
    } else {
      mid = true;
    }
    std::cout << s;
  }
  if(!mid) {
    std::cout << '-';
  }
  std::cout << "\n";
}
