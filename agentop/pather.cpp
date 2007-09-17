
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <map>
#include <set>
#include <deque>
#include <stdarg.h>
#include <windows.h>

#define CHECK(x) do { if(!(x)) { printf("Error at %s:%d - %s\n", __FILE__, __LINE__, #x); *(int*)0 = 0; } } while(0)

using namespace std;

const long long thresh = 1000000;
const int REGIONCOUNT = 20;
const int SYSTEMCOUNT = 930;

unsigned char avail[SYSTEMCOUNT][1 << REGIONCOUNT];

map<pair<string, float>, long long> distill(const map<pair<string, float>, long long> &v) {
  map<pair<string, float>, long long> rv = v;
  map<pair<string, float>, long long>::iterator it = rv.begin();
  
  while(it != rv.end()) {
    map<pair<string, float>, long long>::iterator nit = it;
    nit++;
    
    if(nit == rv.end()) {
      if(it->second < thresh)
        rv.erase(it);
      it = nit;
      continue;
    }
    
    if(it->second < thresh) {
      if(it->first.first == nit->first.first)
        nit->second += it->second;
      rv.erase(it);
    }
    
    it = nit;
  }
  
  return rv;
}

class Filedescr {
public:
    string fname;
    bool directory;
};

bool operator<( const Filedescr &lhs, const Filedescr &rhs ) {
    return lhs.fname < rhs.fname; };

vector< Filedescr > getDirList( const string &dr ) {
    char buf[ 256 ];
    sprintf( buf, "%s\\*", dr.c_str() );
    WIN32_FIND_DATA wfd;
    HANDLE hnd = FindFirstFile( buf, &wfd );
    vector< Filedescr > oot;
    if( hnd == INVALID_HANDLE_VALUE )
        return oot;
    do {
        Filedescr desc;
        desc.fname = wfd.cFileName;
        desc.directory = ( ( wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) != 0 );
        if( desc.fname == ".." || desc.fname == "." )
            continue;
        oot.push_back( desc );
    } while( FindNextFile( hnd, &wfd ) );
	FindClose( hnd );
    //sort( oot.begin(), oot.end() );
    return oot;
};

static bool inthread = false;
string StringPrintf(const char *bort, ...) {
  CHECK(!inthread);
  inthread = true;
  
  static vector< char > buf(2);
  va_list args;

  int done = 0;
  bool noresize = false;
  do {
    if(done && !noresize)
      buf.resize(buf.size() * 2);
    va_start(args, bort);
    done = vsnprintf(&(buf[0]), buf.size() - 1,  bort, args);
    if(done >= (int)buf.size()) {
      CHECK(noresize == false);
      CHECK(buf[buf.size() - 2] == 0);
      buf.resize(done + 2);
      done = -1;
      noresize = true;
    } else {
      CHECK(done < (int)buf.size());
    }
    va_end(args);
  } while(done == buf.size() - 1 || done == -1);

  CHECK(done < (int)buf.size());

  string rv = string(buf.begin(), buf.begin() + done);
  
  CHECK(inthread);
  inthread = false;
  
  return rv;
};

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

vector< int > sti(const vector< string > &foo) {
  int i;
  vector< int > bar;
  for(i = 0; i < foo.size(); i++) {
    bar.push_back(atoi(foo[i].c_str()));
  }
  return bar;
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

struct Station {
  string name;
  int oid;
  int ss;
  vector<int> agents;
};

struct Agent {
  string name;
  int corp;
  int level;
  int quality;
  int station;
};

struct Solarsystem {
  string name;
  int security;
  string region;
  vector<int> stations;
  vector<int> links;
};

class Scoring {
  int score;
  vector<string> text;
  string name;
public:
  void addscore(const string &str, int val) {
    char beef[2048];
    sprintf(beef, "%+6d: %s", val, str.c_str());
    text.push_back(beef);
    score += val;
  }
  void printscore() const {
    printf("%s total score: %d\n", name.c_str(), score);
    for(int i = 0; i < text.size(); i++)
      printf("%s\n", text[i].c_str());
    printf("\n");
  }
  int getscore() const {
    return score;
  }
  
  Scoring() {
    score = -1;
    name = "invalid";
  }
  Scoring(const string &in_name) {
    name = in_name;
    score = 0;
  }
};

bool operator<(const Scoring &lhs, const Scoring &rhs) {
  return lhs.getscore() < rhs.getscore();
}

void countAgents(const Station &station, map<int, Agent> &agents, Scoring *scoring, int mult, const string &descr) {
  for(int i = 0; i < station.agents.size(); i++) {
    CHECK(agents.count(station.agents[i]));
    Agent &agent = agents[station.agents[i]];
    if(agent.level == 1) {
      scoring->addscore("Level 1 agent " + agent.name + " " + descr, 10 * mult);
    } else if(agent.level == 2) {
      scoring->addscore("Level 2 agent " + agent.name + " " + descr, 15 * mult);
    } else if(agent.level == 3) {
      scoring->addscore("Level 3 agent " + agent.name + " " + descr, 20 * mult);
    } else if(agent.level == 4) {
      scoring->addscore("Level 4 agent " + agent.name + " " + descr, 25 * mult);
    }
    
    if(agent.quality >= 10) {
      scoring->addscore("  High quality bonus", 10 * mult);
    }
  }
}

int main() {
  
  map<int, Solarsystem> solarinfo;
  map<string, int> solarname;
  {
    ifstream mss("raw/dbo_mapSolarSystems.csv");
    string lin;
    getline(mss, lin);
    map<string, int> cts = gencats(lin);
    
    int ssid = snag(cts, "solarSystemID");
    int ssname = snag(cts, "solarSystemName");
    int security = snag(cts, "security");
    int region = snag(cts, "regionID");
    
    while(getline(mss, lin)) {
      vector<string> sopts = optize(lin);
      vector<int> opts = sti(sopts);
      CHECK(!solarinfo.count(opts[ssid]));
      solarinfo[opts[ssid]].name = sopts[ssname];
      solarinfo[opts[ssid]].security = securize(sopts[security]);
      solarinfo[opts[ssid]].region = sopts[region];
      CHECK(!solarname.count(sopts[ssname]));
      solarname[sopts[ssname]] = opts[ssid];
      solarname[sopts[ssname]] = opts[ssid];
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
      vector<int> opts = sti(optize(lin));
      solarinfo[opts[fromid]].links.push_back(opts[toid]);
      linkcount++;
    }
    printf("Parsed %d links\n", linkcount);
  }
  
  {
    set<string> allowed;
    set<string> regions;
    deque<int> id;
    id.push_back(solarname["Jita"]);
    while(id.size()) {
      int tid = id[0];
      id.pop_front();
      if(allowed.count(solarinfo[tid].name))
        continue;
      if(solarinfo[tid].security < 50)
        continue;
      allowed.insert(solarinfo[tid].name);
      regions.insert(solarinfo[tid].region);
      for(int i = 0; i < solarinfo[tid].links.size(); i++)
        id.push_back(solarinfo[tid].links[i]);
    }
    
    printf("found %d allowed systems, %d allowed regions\n", allowed.size(), regions.size());
    
    CHECK(regions.size() == REGIONCOUNT);
    CHECK(allowed.size() == SYSTEMCOUNT);
    const string start = "Jita";
    
    {
      vector<string> route;
      set<string> touchedregions;
      route.push_back(start);
      touchedregions.insert(solarinfo[solarname[start]].region);
      string curpos = start;
      while(touchedregions.size() != regions.size()) {
        deque<int> deq;
        set<int> binthere;
        CHECK(solarname.count(curpos));
        deq.push_back(solarname[curpos]);
        while(deq.size()) {
          int ite = deq[0];
          deq.pop_front();
          if(binthere.count(ite))
            continue;
          if(solarinfo[ite].security < 50)
            continue;
          binthere.insert(ite);
          if(!touchedregions.count(solarinfo[ite].region)) {
            route.push_back(solarinfo[ite].name);
            curpos = solarinfo[ite].name;
            touchedregions.insert(solarinfo[ite].region);
            break;
          }
          for(int i = 0; i < solarinfo[ite].links.size(); i++) {
            deq.push_back(solarinfo[ite].links[i]);
          }
        }
      }
      
      printf("Basic route begin:\n");
      for(int i = 0; i < route.size(); i++)
        printf("%s\n", route[i].c_str());
    }
    
    {
      memset(avail, 254, sizeof(avail));
      
      map<string, int> reglookup;
      for(set<string>::iterator itr = regions.begin(); itr != regions.end(); itr++) {
        int q = reglookup.size();
        reglookup[*itr] = q;
      }
      
      map<string, int> syslookup;
      map<int, string> rsyslookup;
      map<int, int> sysregion;
      for(set<string>::iterator itr = allowed.begin(); itr != allowed.end(); itr++) {
        int q = syslookup.size();
        syslookup[*itr] = q;
        rsyslookup[q] = *itr;
        sysregion[q] = reglookup[solarinfo[solarname[*itr]].region];
      }
      
      vector<vector<int> > links(syslookup.size());
      for(map<int, Solarsystem>::iterator sis = solarinfo.begin(); sis != solarinfo.end(); sis++) {
        if(!allowed.count(sis->second.name))
          continue;
        
        for(int i = 0; i < sis->second.links.size(); i++) {
          if(!allowed.count(solarinfo[sis->second.links[i]].name))
            continue;
          links[syslookup[sis->second.name]].push_back(syslookup[solarinfo[sis->second.links[i]].name]);
        }
      }
      
      deque<pair<short, int> > pos;
      pos.push_back(make_pair(syslookup[start], 1 << reglookup[solarinfo[solarname[start]].region]));
      avail[pos.front().first][pos.front().second] = 0;
      
      int final = -1;
      
      while(pos.size()) {
        pair<short, int> ite = pos.front();
        pos.pop_front();
        
        CHECK(avail[ite.first][ite.second] != 254);
        
        if(ite.second == (1 << REGIONCOUNT) - 1) {
          final = ite.first;
          break;
        }
        
        for(int i = 0; i < links[ite.first].size(); i++) {
          if(avail[links[ite.first][i]][ite.second | (1 << sysregion[links[ite.first][i]])] > avail[ite.first][ite.second] + 1) {
            avail[links[ite.first][i]][ite.second | (1 << sysregion[links[ite.first][i]])] = avail[ite.first][ite.second] + 1;
            pos.push_back(make_pair(links[ite.first][i], ite.second | (1 << sysregion[links[ite.first][i]])));
          }
        }
      }
      
      printf("Done, terminates at %d cost %d\n", final, avail[final][(1 << REGIONCOUNT) - 1]);
      pair<short, int> loc = make_pair(final, (1 << REGIONCOUNT) - 1);
      vector<string> rstat;
      while(loc.first != syslookup[start] && loc.second != 1 << reglookup[solarinfo[solarname[start]].region]) {
        pair<short, int> nloc = loc;
        for(int i = 0; i < links[loc.first].size(); i++) {
          if(avail[links[loc.first][i]][loc.second] == avail[loc.first][loc.second] - 1) {
            nloc = make_pair(links[loc.first][i], loc.second);
          }
          
          if(avail[links[loc.first][i]][loc.second & ~(1 << sysregion[loc.first])] == avail[loc.first][loc.second] - 1) {
            nloc = make_pair(links[loc.first][i], loc.second & ~(1 << sysregion[loc.first]));
          }
        }
        
        CHECK(nloc != loc);
        
        if(loc.second != nloc.second) {
          rstat.push_back(StringPrintf("%s (region waypoint)", rsyslookup[loc.first].c_str()));
        } else {
          rstat.push_back(StringPrintf("%s", rsyslookup[loc.first].c_str()));
        }
        
        loc = nloc;
      }
      
      rstat.push_back(StringPrintf("%s (origin)", rsyslookup[loc.first].c_str()));
      
      reverse(rstat.begin(), rstat.end());
      for(int i = 0; i < rstat.size(); i++)
        printf("%s\n", rstat[i].c_str());
      
    }
  }
  
}
