/* Sample code to read in test cases: */
#include <regex.h>
#include <stdio.h>
#include <string.h>
void process(char*);
int main(int argc, const char * argv[]) {
    FILE *file = fopen(argv[1], "r");
    char line[4096];
#if 0
    process(strdup("*7* johab.py gen_probe.ko palmtx.h macpath.py tzp dm-dirty-log.h bh1770.h pktloc faillog.8.gz zconf.gperf"));
    process(strdup("*[0123456789]*[auoei]* IBM1008_420.so zfgrep limits.conf.5.gz tc.h ilogb.3.gz limits.conf CyrAsia-TerminusBold28x14.psf.gz nf_conntrack_sip.ko DistUpgradeViewNonInteractive.pyc NFKDQC"));
    process(strdup("*.??? max_user_watches arptables.h net_namespace Kannada.pl menu_no_no.utf-8.vim shtags.1 unistd_32_ia32.h gettext-tools.mo ntpdate.md5sums linkat.2.gz"));
    process(strdup("*.pdf OldItali.pl term.log plymouth-upstart-bridge rand.so libipw.ko jisfreq.pyc impedance-analyzer xmon.h 1.5.0.3.txt bank"));
    process(strdup("g*.* 56b8a0b6.0 sl.vim digctl.h groff-base.conffiles python-software-properties.md5sums CountryInformation.py use_zero_page session-noninteractive d2i_RSAPublicKey.3ssl.gz container-detect.log.4.gz"));
    process(strdup("*[0123456789]* keyboard.h machinecheck 46b2fd3b.0 libip6t_frag.so timer_defs.h nano-menu.xpm NI vim-keys.conf setjmp.h memcg"));
#endif
      while (fgets(line, 4096, file)) {
      char * pos;
      if((pos = strchr(line, '\n')) != NULL) {
        *pos = 0;
      }
      process(line);
    }
    return 0;
}

void process(char * line) {
  char * pos = strtok(line, " ");
  char * pat = line;
  char pattern[1024] = { 0, };
  for(int i = 0, j = 0; i < strlen(pat); ++i) {
    char c = pat[i];
    switch(c) {
      case '*':
        pattern[j++] = '.';
        pattern[j++] = '*';
        break;
      case '?':
        pattern[j++] = '.';
        break;
      default:
        if(strchr("(){}.\\+", c)) {
          pattern[j++] = '\\';
        }
        pattern[j++] = c;
    };
  }

  ++pos;
  regex_t rx;
  if(regcomp(&rx, pattern, 0)) {
    printf("-\n");
    return;
  }
  int mid = 0;
  while(pos = strtok(0, " ")) {
    regmatch_t m;
    if(regexec(&rx, pos, 1, &m, 0)) {
      continue;
    }
    if(m.rm_so ||  m.rm_eo < strlen(pos)) {
      continue;
    }
    printf("%s%s",mid?" ":"", pos);
    mid = 1;
  }
  printf("%s\n",mid?"":"-");
}
