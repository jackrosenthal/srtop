CXX = g++
LD = $(CXX)
SRCFILES = $(wildcard src/*.cpp)
OBJFILES = $(patsubst src/%.cpp, bin/%.o, $(SRCFILES))
OBJDEBUG = $(patsubst src/%.cpp, bin/%.debug.o, $(SRCFILES))
OUTFILE = srtop
CXXFLAGS = -Wall -std=c++11 -DPROC_ROOT='"'/proc'"'
LDFLAGS = -lncurses

all: $(OUTFILE)

$(OUTFILE): $(OBJFILES)
	$(LD) $(LDFLAGS) $(OBJFILES) -o $(OUTFILE)

bin/:
	mkdir -p bin

bin/%.o: src/%.cpp bin/
	$(CXX) $(CXXFLAGS) -o $@ -c $<

debug: $(OUTFILE).debug
	gdb -tui $(OUTFILE).debug

$(OUTFILE).debug: $(OBJDEBUG)
	$(LD) $(LDFLAGS) $(OBJDEBUG) -o $(OUTFILE).debug

bin/%.debug.o: src/%.cpp bin/
	$(CXX) $(CXXFLAGS) -ggdb3 -o $@ -c $<

.PHONY: clean
clean:
	rm -rf bin
	rm -f $(OUTFILE)
	rm -f $(OUTFILE).debug
