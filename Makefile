
raw/%.csv: raw/%.csv.bz2
	bunzip2 -k $<

%.o: %.cpp
	nice g++ -O2 -c -o $@ $<

agentop.exe: agentop/agentop.o raw/dbo_mapSolarSystems.csv raw/dbo_mapSolarSystemJumps.csv raw/dbo_staServices.csv raw/dbo_staOperationServices.csv raw/dbo_staStations.csv
	nice g++ -o $@ agentop/agentop.o

run: agentop.exe
	agentop.exe
