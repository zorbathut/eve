
raw/%.csv: raw/%.csv.bz2
	bunzip2 -k $<

%.o: %.cpp
	nice g++ -O2 -c -o $@ $<

agentop.exe: agentop/agentop.o raw/dbo_mapSolarSystems.csv raw/dbo_mapSolarSystemJumps.csv
	nice g++ -o $@ agentop/agentop.o

run: agentop.exe
	agentop.exe
