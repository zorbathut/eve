
#include <string>
#include <vector>
#include <cstdio>
#include <memory.h>
#include <algorithm>

using namespace std;

const int t1 = 0;
const int t2 = 5;

const string names[] = { "EM", "Thermal", "Kinetic", "Explosive", "Invincibility" };

const int base[] = { 0, 20, 40, 60 };
//const int base[] = { 25, 40, 55, 70 };

float dps[] = { 0, 0, 0, 0 };
const float generaldps = 100;

const float stack[] = {1.0, 0.8691, 0.5706, 0.2830, 0.1060, 0.03000, 0.006410, 0.001031};

int curresist[10] = {0,};
int bestresist[10] = {0,};
float dpstaken = 1e1000;

void procfinal(bool verbose = false) {
  /*
  for(int i = 0; i < 10; i++)
    printf("%d ", curresist[i]);
  printf("\n");
  */
  float totdps = 0;
  for(int i = 0; i < 4; i++) {
    vector<float> mults;
    for(int j = 0; j < curresist[i]; j++)
      mults.push_back(0.5);
    for(int j = 0; j < curresist[i+5]; j++)
      mults.push_back(0.55);
    for(int j = 0; j < curresist[4]; j++)
      mults.push_back(0.25);
    for(int j = 0; j < curresist[9]; j++)
      mults.push_back(0.30);
    sort(mults.begin(), mults.end());
    reverse(mults.begin(), mults.end());
    if(verbose && mults.size()) {
      printf("%s: ", names[i].c_str());
      for(int j = 0; j < mults.size(); j++)
        printf("%0.3f, ", mults[j]);
      printf("\n");
    }
    for(int j = 0; j < mults.size(); j++)
      mults[j] *= stack[j];
    if(verbose && mults.size()) {
      printf("%s: ", names[i].c_str());
      for(int j = 0; j < mults.size(); j++)
        printf("%0.3f, ", mults[j]);
      printf("\n");
    }
    
    float cres = (100.0 - base[i]) / 100;
    for(int j = 0; j < mults.size(); j++)
      cres *= 1.0 - mults[j];
    
    totdps += dps[i] * cres;
  }
  
  if(totdps < dpstaken) {
    dpstaken = totdps;
    memcpy(bestresist, curresist, sizeof(curresist));
  }
}

void dorec(int t1left, int t2left, int lowt1, int lowt2) {
  if(t1left) {
    for(int i = lowt1; i <= 4; i++) {
      curresist[i]++;
      dorec(t1left - 1, t2left, i, 0);
      curresist[i]--;
    }
  } else if(t2left) {
    for(int i = lowt2; i <= 4; i++) {
      curresist[i + 5]++;
      dorec(0, t2left - 1, 4, i);
      curresist[i + 5]--;
    }
  } else {
    procfinal();
  }
}

int main() {
  for(int i = 0; i < 4; i++)
    dps[i] += generaldps / 4;
  
  dorec(t1, t2, 0, 0);
  
  memcpy(curresist, bestresist, sizeof(curresist));
  procfinal(true);
  
  for(int i = 0; i < 10; i++) {
    if(bestresist[i]) {
      printf("%d t%d %s hardener%s\n", bestresist[i], i / 5 + 1, names[i % 5].c_str(), bestresist[i] == 1 ? "" : "s");
    }
  }
  printf("final dps: %f\n", dpstaken);
}

