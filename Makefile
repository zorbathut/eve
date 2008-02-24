
raw/%.csv: raw/%.csv.bz2
	bunzip2 -k $<

%.o: %.cpp
	nice g++ -O2 -c -o $@ $<

agentop.exe: agentop/agentop.o raw/dbo_mapSolarSystems.csv raw/dbo_mapSolarSystemJumps.csv raw/dbo_staServices.csv raw/dbo_staOperationServices.csv raw/dbo_staStations.csv raw/dbo_crpNPCDivisions.csv raw/dbo_agtAgents.csv
	nice g++ -o $@ agentop/agentop.o

pather.exe: agentop/pather.o raw/dbo_mapSolarSystems.csv raw/dbo_mapSolarSystemJumps.csv
	nice g++ -o $@ agentop/pather.o

implantlist.exe: implantlist.o
	nice g++ -o $@ implantlist.o

agentop: agentop.exe
	agentop.exe

download:
	wget "http://eve-central.com/home/quicklook.html?sethours=10000&setminQ=1&typeid=34" -O ecd.txt

implants: implantlist.exe
	implantlist.exe
