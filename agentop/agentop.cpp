
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <map>

#define CHECK(x) do { if(!(x)) { printf("Error at %s:%d - %s\n", __FILE__, __LINE__, #x); *(int*)0 = 0; } } while(0)

using namespace std;

vector< string > tokenize(const string &in, const string &kar) {
  string::const_iterator cp = in.begin();
  vector< string > oot;
  while(cp != in.end()) {
    while(cp != in.end() && count(kar.begin(), kar.end(), *cp))
      cp++;
    if(cp != in.end())
      oot.push_back(string(cp, find_first_of(cp, in.end(), kar.begin(), kar.end())));
    cp = find_first_of(cp, in.end(), kar.begin(), kar.end());
  };
  return oot;
};

vector<string> optize(const string &lin) {
  vector<string> rv = tokenize(lin, ";\r");
  for(int i = 0; i < rv.size(); i++) {
    //printf("%d, %c, %c, %s\n", rv[i].size(), rv[i][0], rv[i][rv[i].size() - 1], rv[i].c_str());
    CHECK(rv[i].size() >= 2 && rv[i][0] == '"' && rv[i][rv[i].size() - 1] == '"');
    string tp(rv[i].begin() + 1, rv[i].end() - 1);
    rv[i].clear();
    for(int j = 0; j < tp.size(); j++) {
      if(tp[j] == '.') {
      } else if(tp[j] == ',') {
        rv[i] += '.';
      } else {
        rv[i] += tp[j];
      }
    }
  }
  return rv;
}

int snag(const map<string, int> &cat, const string &name) {
  CHECK(cat.count(name));
  return cat.find(name)->second;
}

int securize(const string &in) {
  string tp;
  for(int i = 0; i < in.size(); i++)
    if(in[i] != '.')
      tp += in[i];
  return atoi(tp.c_str());
}

map<string, int> gencats(const string &catline) {
  vector<string> cats = optize(catline);
  map<string, int> cts;
  for(int i = 0; i < cats.size(); i++) {
    CHECK(!cts.count(cats[i]));
    cts[cats[i]] = i;
  }
  return cts;
}

int main() {
  
  map<int, pair<string, int> > solarinfo;
  {
    ifstream mss("raw/dbo_mapSolarSystems.csv");
    string lin;
    getline(mss, lin);
    map<string, int> cts = gencats(lin);
    
    int ssid = snag(cts, "solarSystemID");
    int ssname = snag(cts, "solarSystemName");
    int security = snag(cts, "security");
    
    while(getline(mss, lin)) {
      vector<string> opts = optize(lin);
      CHECK(!solarinfo.count(atoi(opts[ssid].c_str())));
      solarinfo[atoi(opts[ssid].c_str())] = make_pair(opts[ssname], securize(opts[security]));
    }
    printf("Parsed %d solar systems\n", solarinfo.size());
  }
  
  map<int, vector<int> > solarlink;
  {
    ifstream mss("raw/dbo_mapSolarSystemJumps.csv");
    string lin;
    getline(mss, lin);
    map<string, int> cts = gencats(lin);
    
    int fromid = snag(cts, "fromSolarSystemID");
    int toid = snag(cts, "toSolarSystemID");
    
    int linkcount = 0;
    while(getline(mss, lin)) {
      vector<string> opts = optize(lin);
      solarlink[atoi(opts[fromid].c_str())].push_back(atoi(opts[toid].c_str()));
      linkcount++;
    }
    printf("Parsed %d links\n", linkcount);
  }
  
  int reprocid = -1;
  int factoryid = -1;
  {
    ifstream mss("raw/dbo_staServices.csv");
    string lin;
    getline(mss, lin);
    map<string, int> cts = gencats(lin);
    
    int sid = snag(cts, "serviceID");
    int sna = snag(cts, "serviceName");
    
    int linkcount = 0;
    while(getline(mss, lin)) {
      vector<string> opts = optize(lin);
      if(opts[sna] == "Factory") {
        CHECK(factoryid == -1);
        factoryid = atoi(opts[sid].c_str());
      } else if(opts[sna] == "Reprocessing Plant") {
        CHECK(reprocid == -1);
        reprocid = atoi(opts[sid].c_str());
      }
    }
    CHECK(reprocid != -1);
    CHECK(factoryid != -1);
    printf("Parsed reprocid and factoryid\n", linkcount);
  }
}
