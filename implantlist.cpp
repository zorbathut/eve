
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <cstdio>
#include <fstream>
#include <cmath>

using namespace std;


vector<string> tokenize(const string &in, const string &kar) {
  string::const_iterator cp = in.begin();
  vector<string> oot;
  while(cp != in.end()) {
    while(cp != in.end() && count(kar.begin(), kar.end(), *cp))
      cp++;
    if(cp != in.end())
      oot.push_back(string(cp, find_first_of(cp, in.end(), kar.begin(), kar.end())));
    cp = find_first_of(cp, in.end(), kar.begin(), kar.end());
  };
  return oot;
};

#define CHECK(x)   if(!x) { printf("FAILURE at %s:%d (%s)\n", __FILE__, __LINE__, #x); exit(1); } else { }


bool isnotimplant(string imp) {
  bool hasalpha = false;
  bool haslower = false;
  for(int i = 0; i < imp.size(); i++) {
    if(isalpha(imp[i])) {
      hasalpha = true;
      if(islower(imp[i])) {
        haslower = true;
      }
    }
  }
  
  return !hasalpha || haslower;
}

int main() {
  map<string, float> pric;
  {
    ifstream ifs("prices.txt");
    string lin;
    while(getline(ifs, lin)) {
      vector<string> v = tokenize(lin, " ");
      CHECK(v.size() == 2);
      CHECK(!pric.count(v[0]));
      pric[v[0]] = atof(v[1].c_str());
      
      printf("%s is %f\n", v[0].c_str(), pric[v[0]]);
    }
  }
  
  {
    ifstream ifs("layout.txt");
    string lin;
    
    bool inside = false;
    
    while(getline(ifs, lin)) {
      if(lin == "[indent]")
        inside = true;
      else if(lin == "[/indent]")
        inside = false;
      
      if(inside) {
        vector<string> toki = tokenize(lin, " ");
        
        string impid;
        if(toki.size() >= 2 && pric.count(toki[1]))
          impid = toki[1];
        else if(toki.size() >= 4 && pric.count(toki[3]))
          impid = toki[3];
        
        if(impid.size()) {
          int cost = int(ceil(pric[impid] * 1.3));
          char deep[20];
          sprintf(deep, "%dm", cost);
          
          if(strstr(toki.back().c_str(), "[/indent]"))
            toki.back() = deep + string("[/indent]");
          else
            toki.back() = deep;
        } else {
          CHECK((toki.size() < 2 || isnotimplant(toki[1])) && (toki.size() < 4 || isnotimplant(toki[3])));
        }
        
        for(int i = 0; i < toki.size(); i++)
          printf("%s ", toki[i].c_str());
        printf("\n");
      } else {
        printf("%s\n", lin.c_str());
      }
    }
  }

};
