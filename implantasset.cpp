
#include <vector>
#include <string>
#include <fstream>
#include <numeric>

using namespace std;

vector<string> csv(const string &ins) {
  const char *kar = ins.c_str();
  vector<string> rv;
  while(*kar) {
    const char *nxt = kar;
    bool inq = false;
    while(*nxt && (*nxt != ',' || inq)) {
      if(*nxt == '"')
        inq = !inq;
      nxt++;
    }
    if(*kar == '"' && *(nxt - 1) == '"') {
      rv.push_back(string(kar + 1, nxt - 1));
    } else {
      rv.push_back(string(kar, nxt));
    }
    kar = nxt;
    if(*kar)
      kar++;
  }
  return rv;
}

struct Desires {
  char ident[10];
  int nc[4];
  int c[4];
};

const Desires des[] = {
  { "KZF", {10, 6, 8, 6}, {10, 4, 5, 4} },
  { "77S", {10, 6, 8, 6}, {10, 4, 5, 4} },
  { "FAT", {10, 4, 6, 4}, {10, 3, 4, 3} },
  { "319", {70, 8, 12, 6}, {70, 5, 7, 4} },
};
const int des_size = sizeof(des) / sizeof(*des);

const char *impname[] = {"Cyb", "Mem", "Neu", "Ocu", "Soc"};
const char *impadd[] = {"Limited", "Beta", "Basic", "Standard"};

const int impname_size = sizeof(impname) / sizeof(*impname);
const int impadd_size = sizeof(impadd) / sizeof(*impadd);

int main() {
  string beef;
  ifstream ass("assets.csv"); // ASS
  
  int impcount[des_size][impadd_size][impname_size];
  memset(impcount, 0, sizeof(impcount));
  
  while(getline(ass, beef)) { // ass beef
    vector<string> tok = csv(beef);
    
    if(tok[2] != "ZorbaTHut")
      continue;
    
    int match = -1;
    for(int i = 0; i < des_size; i++)
      if(strstr(tok[20].c_str(), des[i].ident))
        match = i;
    
    if(match == -1)
      continue;
    
    string iname = tok[9];
    
    int in = -1;
    for(int i = 0; i < impname_size; i++) {
      if(strstr(iname.c_str(), impname[i])) {
        in = i;
        break;
      }
    }
    
    if(in == -1)
      continue;
    
    int ia = -1;
    for(int i = impadd_size - 1; i >= 0; i--) {
      if(strstr(iname.c_str(), impadd[i])) {
        ia = i;
        break;
      }
    }
    
    if(ia == -1)
      continue;
    
    impcount[match][ia][in] += atoi(tok[5].c_str());
    printf("%s %d/%d: %d\n", des[match].ident, in, ia, atoi(tok[5].c_str()));
    
    /*
    printf("Chunk\n");
    for(int i = 0; i < tok.size(); i++)
      printf("  %s\n", tok[i].c_str());*/
  }
  
  for(int i = 0; i < des_size; i++) {
    printf("%s:\n", des[i].ident);
    for(int j = 0; j < impadd_size; j++) {
      printf("  +%d:\n", j);
      for(int k = 0; k < impname_size; k++) {
        printf("    %s: %d\n", impname[k], impcount[i][j][k]);
      }
    }
  }
  
  int impdes[des_size][impadd_size][impname_size];
  for(int i = 0; i < des_size; i++) {
    for(int j = 0; j < impadd_size; j++) {
      for(int k = 0; k < impname_size; k++) {
        impdes[i][j][k] = des[i].nc[j];
      }
      impdes[i][j][4] = des[i].c[j];
    }
  }
  
  int want[des_size + 1][impadd_size][impname_size];
  memset(want, 0, sizeof(want));
  for(int i = 0; i < des_size; i++) {
    for(int j = 0; j < impadd_size; j++) {
      for(int k = 0; k < impname_size; k++) {
        want[i][j][k] = max(impdes[i][j][k] - impcount[i][j][k], 0);
        want[des_size][j][k] += want[i][j][k];
      }
    }
  }
  
  for(int i = 0; i < des_size + 1; i++) {
    if(i == des_size)
      printf("Want total:\n");
    else
      printf("Want %s:\n", des[i].ident);
    for(int j = 0; j < impadd_size; j++) {
      printf("  +%d:\n", j + 1);
      for(int k = 0; k < impname_size; k++) {
        printf("    %s: %d\n", impname[k], want[i][j][k]);
      }
    }
  }
  
}
