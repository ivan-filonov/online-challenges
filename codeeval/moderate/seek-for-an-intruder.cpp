#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

//#define CPP_REGEX

#ifdef CPP_REGEX
#include <regex>
#else//def CPP_REGEX
// codeeval's compiler is TOO slow and fails to compile C++ code with regex from STL in time.
#include <regex.h>
#endif//def CPP_REGEX

namespace {
  void test();
  void process_file(char*);
}

int main(int argc, char ** argv) {
    if( 0 ) {
        test();
    } else {
        process_file(argv[1]);
    }
	return 0;
}

namespace {
  using string_t = std::string;
  template<typename T> using vector_t = std::vector<T>;

  vector_t<string_t> test_data3 {
    R"DELIM(m*M}Qz`\fz/We}e[`md;Puuat-;UP|Yi64iXh%{Hnul8&onq0p*mY+4x\/{ZTw[gXeJV2&.P*Ywe)DELIM",
    R"DELIM(VA,8Z%z-AYzp6o{qeX3Q|\`Zw7{78:Y80qP-,b0BDVvZh60x59.0xe5.0x82.0xe1uptW8eF8C]nKJ9c(AtXa9>Dy}nF'Jr)DELIM",
    R"DELIM(Cq2ox2Mmr7PuaPO023244514100.@({-mER/yhWg)wsf"`Fu_tp.C]6$!?(^+wzLBxi,PJ41Hdu`m>Bz=v*^~N|h)DELIM",
    R"DELIM(jfZ&y9w'XkrrO6JDoZOyZ864.599.341.917JBJ5u(^i%BjecAd"$4UKtPnbtvx^01540.01127.0525.01625tU$HY/,Uw(/CJP]L+/XohV2hD&])DELIM",
    R"DELIM(9Pl.1011001111001011000001011100001,Y,HNAiSzL;?BU_UQlCvyzRU^"R]{kVJ"[+3%PK`]\"V?;Y'8CjJ<&QGmESP6W7&P,@$tFtL)DELIM",
    R"DELIM(`z6DR}/>gLfLX[1&]Vr8"EG-_+wy?sw4beHIp^oTtZzvWBwY{[89.229.130.225R,?B;"?[ix4^9D$fVaJ_V\)N`B)DELIM",
    R"DELIM(=ddalCNOM)FnA=/r,?}#idpo1E#eeMq.wyfu/2viz;c_[kHppMh,K|\`Q1_R(`jRNvCZW2Niz7Q#)DELIM",
    R"DELIM(w:b21f[rsnj^Rgg[t!(<5v`Iup^&]o@489.229.130.225gw4\SwBEbN222.137.104.206[Jo<)lj36bB.034062405073xx37d;~wKi/D"I'AeVfeBO)DELIM",
    R"DELIM(|7$mi3k]f}9N*Vjq5aMy[Xd+3a$n$paB?5p9^01000000.11101110.00100000.01100101u@0:7&J;8FDZ<LuN-ecftQ%XU2urHk=N"y}1Zt+)DELIM",
    R"DELIM(|lLN|l8ZlMi()kN29/B!uj~l8#1o?Kcp/1{FfL]1/NsK$<@`)0P+LVgv4ziP>t840131.0345.0202.0341`}Z*Xz[8IH?')DELIM",
    R"DELIM(^~H5]JqL#?>d8V5JPP1101100000.1001010111.101010101.11100101011Funfr=3*E\pEa"3YV^?J+;dLA#t)$3Lvi5J<MSF]LB&yOXgO't/E864.599.341.917Ze033642264316z~7)DELIM",
    R"DELIM(W7+1&dt2#Ek&<am6G|!!18P>?|?qQyV(0k?>KPB:t{IRUm>cuN0^[YSOsixxF"zzl5LALPIaXFM.)DELIM",
    R"DELIM(jMfgpfH+w>M8\`r{;`XdEYm0Rc7o>Aqq4k?gP>,O^2I^]OF#zN\cLSUQ(x!(oxA0l<xjM=-qcIXE)DELIM",
    R"DELIM(_L)a,<}f4u9dc]'440h0y7Yu=&NOfz-k%hk/FLpVZBsX|+P5YjaNBH]TCG~{,k]Dc\7p$)s&4D+Y)DELIM",
    R"DELIM(xI~0JkBd]}c#]!4eGec7oz>d:yYl*K_AK^Hd_<c+hjD4w:-[90?}lBZ]^@iT2A&i9=9tsjg3muNC)DELIM",
    R"DELIM(,6ze^#eqtjrd`P&?WY,J-]L)_gVR*NJ~]Q(#l"Yu~Jpl*ui"9JtZ=&2B{!6"iP@Y@3I%Zft>cpd`)DELIM",
    R"DELIM(OwEK!d?y\M(_L~|=lm1++BLA<&PnOnBAfga>t},x{T$*&/}+wX{j/pm'|N~Cq1000000111011100010000001100101x~[*Scc=lb82`K~)DELIM",
    R"DELIM(HydTS#@@864.599.341.917Q&.DVb\ails}C101100100110010011011000101110100&@KyFK"7}u3\63?u][~zz>-r$_OqUbE*uv,\ccCnUmP\<p,o4E6[7c])DELIM",
    R"DELIM(v<md%aG2z8n'}"9}qwZYnHdo>`4/Ht!3puL.A\'].BA>reos{U0x360.0x257.0x155.0x395y^UGXh^'`|I.CV}R>a}RAhO%Vw)DELIM",
    R"DELIM(uQ#27/z^B8q:x(I|$k9dmF{\<ofV5sg[F>P(t!ui5[<mpXTzO%0wF|E2Sh6bl5[YZ:Tm%JsE*5pUO)DELIM",
  };
  vector_t<string_t> test_result1 { "89.229.130.225" };

  void process(string_t s);
  void print_results();

  void test() {
    for(auto & s : test_data3) {
      process(s);
    }

    print_results();

    for(auto & s : test_result1) {
      std::cout << s << "\n";
    }
  }

  void process_file(char* path) {
    std::ifstream stream(path);
    for(string_t line; std::getline(stream, line); ) {
      process(line);
    }
    print_results();
  }

  const string_t r_all_str {  
    "(" R"(0x[0-9a-fA-F]{1,2}\.0x[0-9a-fA-F]{1,2}\.0x[0-9a-fA-F]{1,2}\.0x[0-9a-fA-F]{1,2})" ")|"
    "(" R"([0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3})" ")|"
    "(" R"(0[0-7]{1,3}\.0[0-7]{1,3}\.0[0-7]{1,3}\.0[0-7]{1,3})" ")|"
    "(" R"([01]{8}\.[01]{8}\.[01]{8}\.[01]{8})" ")|"
    "(" "0x[0-9a-fA-F]{7,8}" ")|"
    "(" "[0-9]{8,10}" ")|"
    "(" "0[0-7]{9,11}" ")|"
    "(" "([01]{25,32})" ")" 
  };
#ifdef CPP_REGEX
  std::regex r_all { r_all_str };
#else//def CPP_REGEX
  regex_t rx_all;
  int rx_compiled = regcomp(&rx_all, r_all_str.c_str(), REG_EXTENDED);
#endif//def CPP_REGEX
  
  vector_t<string_t> found;

  void process(string_t line) {
#ifdef CPP_REGEX
    std::smatch m;
    while(std::regex_search(line, m, r_all)) {
      found.push_back(m.str(0));
      line = m.suffix().str();
    }
#else//def CPP_REGEX
    regmatch_t m;
    while(!regexec(&rx_all, line.c_str(), 1, &m, 0)) {
      found.push_back(line.substr(m.rm_so, m.rm_eo - m.rm_so));
      line = line.substr(m.rm_eo);
    }
#endif//def CPP_REGEX
  }

  void print_results() {
    std::map<unsigned long,int> ac;
    for(auto & m : found) {
      int base = 2;
      if('0' == m[0] && ('x' == m[1] || 'X' == m[1])) {
        base = 16;
      } else if(m.find_first_of("23456789" ) != string_t::npos) {
        if('0' == m[0]) {
          base = 8;
        } else {
          base = 10;
        }
      }
      unsigned long addr = 0;
      if(m.find('.') == string_t::npos) {
        addr = std::stoul(m, nullptr, base);
      } else {
        size_t ofs = 0;
        bool fail = false;
        string_t s { m };
        for(int i = 0; i < 4 && !fail; ++i) {
          ofs = s.find('.');
          unsigned long j = std::stoul(s.substr(0, ofs), nullptr, base);
          fail |= j > 255;
          s = s.substr(ofs + 1);
          addr = addr * 256 + j;
        }
        if(fail) {
          addr = 0;
        }
      }

      if(addr < 0x1000000UL || addr > 0xFFFFFFFE) {
        continue;
      }

      ++ac[addr];
    }
    found.clear();
    
    if(ac.empty()) {
      return;
    }

    unsigned long ra = ac.begin()->first;
    int rc = ac.begin()->second;
    for(auto &p : ac) {
      if(p.second > rc) {
        ra = p.first;
        rc = p.second;
      }
    }
    std::cout << ((ra>>24)&0xFF) << '.' << ((ra>>16)&0xFF) << '.' << ((ra>>8)&0xFF) << '.' << (ra&0xFF) << "\n";
  }
}
