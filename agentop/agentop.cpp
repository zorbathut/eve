
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
  
  int reprocid = -1;
  int factoryid = -1;
  {
    ifstream mss("raw/dbo_staServices.csv");
    string lin;
    getline(mss, lin);
    map<string, int> cts = gencats(lin);
    
    int sid = snag(cts, "serviceID");
    int sna = snag(cts, "serviceName");
    
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
    printf("Parsed reprocid and factoryid\n");
  }
  
  map<int, pair<bool, bool> > services;
  {
    ifstream mss("raw/dbo_staOperationServices.csv");
    string lin;
    getline(mss, lin);
    map<string, int> cts = gencats(lin);
    
    int sid = snag(cts, "operationID");
    int sna = snag(cts, "serviceID");
    
    while(getline(mss, lin)) {
      vector<int> opts = sti(optize(lin));
      services[opts[sid]];
      if(opts[sna] == reprocid) {
        services[opts[sid]].first = true;
      } else if(opts[sna] == factoryid) {
        services[opts[sid]].second = true;
      }
    }
    printf("Parsed %d operation services\n", services.size());
  }
  
  map<int, Station> stations;
  {
    ifstream mss("raw/dbo_staStations.csv");
    string lin;
    getline(mss, lin);
    map<string, int> cts = gencats(lin);
    
    int sid = snag(cts, "stationID");
    int oid = snag(cts, "operationID");
    int ssid = snag(cts, "solarSystemID");
    int sname = snag(cts, "stationName");
    
    while(getline(mss, lin)) {
      vector<string> sopts = optize(lin);
      vector<int> opts = sti(sopts);
      if(!opts[oid])
        continue;
      CHECK(!stations.count(opts[sid]));
      CHECK(services.count(opts[oid]));
      CHECK(solarinfo.count(opts[ssid]));
      stations[opts[sid]].oid = opts[oid];
      stations[opts[sid]].ss = opts[ssid];
      stations[opts[sid]].name = sopts[sname];
      solarinfo[opts[ssid]].stations.push_back(opts[sid]);
    }
    printf("Parsed %d stations\n", stations.size());
  }
  
  set<int> divs;
  {
    ifstream mss("raw/dbo_crpNPCDivisions.csv");
    string lin;
    getline(mss, lin);
    map<string, int> cts = gencats(lin);
    
    int did = snag(cts, "divisionID");
    int dname = snag(cts, "divisionName");
    
    while(getline(mss, lin)) {
      vector<string> sopts = optize(lin);
      vector<int> opts = sti(sopts);
      if(sopts[dname] == "Command" || sopts[dname] == "Internal Security" || sopts[dname] == "Security" || sopts[dname] == "Surveillance") {
        divs.insert(opts[did]);
      }
    }
    CHECK(divs.size() == 4);
    printf("Parsed %d agent divisions\n", divs.size());
  }
  
  map<int, Agent> agents;
  {
    ifstream mss("raw/dbo_agtAgents.csv");
    string lin;
    getline(mss, lin);
    map<string, int> cts = gencats(lin);
    
    int aid = snag(cts, "agentID");
    int did = snag(cts, "divisionID");
    int cid = snag(cts, "corporationID");
    int sid = snag(cts, "stationID");
    int lev = snag(cts, "level");
    int qual = snag(cts, "quality");
    int type = snag(cts, "agentTypeID");
    
    while(getline(mss, lin)) {
      vector<int> opts = sti(optize(lin));
      if(opts[type] != 2)
        continue;
      if(!divs.count(opts[did]))
        continue;
      CHECK(!agents.count(opts[aid]));
      agents[opts[aid]].corp = opts[cid];
      agents[opts[aid]].station = opts[sid];
      agents[opts[aid]].level = opts[lev];
      agents[opts[aid]].quality = opts[qual];
      stations[opts[sid]].agents.push_back(opts[aid]);
    }
    printf("Parsed %d agents\n", agents.size());
  }
  
  if(0) {
    // Various rewards:
    // Level X agent: 40/60/80/100 points
    // High-QL agent: +40 points
    // manufacturing plant there: +400 points
    // reproc plant there: +200 points
    // agents in same system: half value
    // reproc plant in same system: half value if there isn't one already
    // agents in adjacent systems: 1/4 value
    // reproc plant in adjacent systems: 1/4 value if there isn't one already
    // security standing?
    
    vector<Scoring> scoring;
    
    for(map<int, Station>::const_iterator itr = stations.begin(); itr != stations.end(); itr++) {
      
      const Station &stat = itr->second;
      const Solarsystem &ss = solarinfo[stat.ss];
      
      Scoring score(stat.name);
      
      if(services[stat.oid].second)
        score.addscore("Has manufacturing plant", 400);
      
      bool hasrep = false;
      
      if(services[stat.oid].first) {
        score.addscore("Has reprocessing plant", 400);
        hasrep = true;
      }
      
      countAgents(stat, agents, &score, 4, "in station");
      
      for(int i = 0; i < ss.stations.size(); i++) {
        if(!hasrep && services[stations[ss.stations[i]].oid].first) {
          score.addscore("Has reprocessing plant in system", 300);
          hasrep = true;
        }
        if(&stations[ss.stations[i]] != &stat)
          countAgents(stations[ss.stations[i]], agents, &score, 2, "in system");
      }
      
      for(int i = 0; i < ss.links.size(); i++) {
        const Solarsystem &tss = solarinfo[ss.links[i]];
        for(int i = 0; i < tss.stations.size(); i++) {
          if(!hasrep && services[stations[tss.stations[i]].oid].first) {
            score.addscore("Has reprocessing plant adjacent", 200);
            hasrep = true;
            break;
          }
          countAgents(stations[tss.stations[i]], agents, &score, 1, "adjacent");
        }
      }
      
      scoring.push_back(score);
    }
    
    sort(scoring.begin(), scoring.end());
    reverse(scoring.begin(), scoring.end());
    for(int i = 0; i < 20; i++)
      scoring[i].printscore();
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
    
    printf("found %d allowed systems, %d allowed regions\n", allowed.size());
    
    vector<string> rv;
    ifstream ifs("ecd.txt");
    string borf;
    while(getline(ifs, borf)) {
      if(borf == " <td>") {
        getline(ifs, borf);
        getline(ifs, borf);
        string aname = tokenize(borf, " ")[0];
        getline(ifs, borf);
        getline(ifs, borf);
        getline(ifs, borf);
        getline(ifs, borf);
        string prik = tokenize(borf, "<> /td")[0];
        if(!allowed.count(aname))
          continue;
        rv.push_back(StringPrintf("%s: %d (%s)", prik.c_str(), solarinfo[solarname[aname]].security, aname.c_str()));
      }
      if(borf == "<h3>Buy Orders</h3>")
        break;
    }
    
    reverse(rv.begin(), rv.end());
    for(int i = 0; i < rv.size(); i++)
      printf("%s\n", rv[i].c_str());
    
    const string path = "C:\\Documents and Settings\\zorba\\My Documents\\EVE\\logs\\Marketlogs";
    vector<Filedescr> fd = getDirList(path);
    for(int i = 0; i < fd.size(); i++) {
      ifstream ifs((path + "\\" + fd[i].fname).c_str());
      
      map<int, int> avail;
      map<int, float> cost;
      string beef;
      getline(ifs, beef);
      while(getline(ifs, beef)) {
        vector<string> vals = tokenize(beef, ",");
        if(vals[7] != "False")
          continue;
        int sid = atoi(vals[12].c_str());
        float tcost = atof(vals[0].c_str());
        int quant = atoi(vals[1].c_str());
        avail[sid] += quant;
        if(cost.count(sid))
          cost[sid] = min(cost[sid], tcost);
        else
          cost[sid] = tcost;
      }
      
      vector<pair<pair<float, int>, string> > dat;
      for(map<int, int>::const_iterator itr = avail.begin(); itr != avail.end(); itr++) {
        if(itr->second < 1000000)
          continue;
        if(!allowed.count(solarinfo[itr->first].name))
          continue;
        dat.push_back(make_pair(make_pair(cost[itr->first], itr->second), solarinfo[itr->first].name));
      }
      sort(dat.begin(), dat.end());
      reverse(dat.begin(), dat.end());
      
      for(int i = 0; i < dat.size(); i++)
        printf("%f: %d (%s)\n", dat[i].first.first, dat[i].first.second, dat[i].second.c_str());
    }
    
    {
      const string start = "Eystur";
      vector<string> route;
      set<string> touchedregions;
      route.push_back(start);
      touchedregions.insert(solarinfo[solarname[start]].region);
      string curpos = "Josekorn";
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
      
      printf("Route begin:\n");
      for(int i = 0; i < route.size(); i++)
        printf("%s\n", route[i].c_str());
    }
  }
  
}
