CXX = g++
LD = $(CXX)
SRCFILES = $(wildcard src/*.cpp)
OBJDIR = bin
OBJFILES = $(patsubst src/%.cpp, $(OBJDIR)/%.o, $(SRCFILES))
OBJDEBUG = $(patsubst src/%.cpp, $(OBJDIR)/%.debug.o, $(SRCFILES))
OUTFILE = srtop
CXXFLAGS = -Wall -std=c++11 -DPROC_ROOT='"'/proc'"'
LDFLAGS = -lncurses

.PHONY: all
all: $(OUTFILE)

$(OUTFILE): $(OBJFILES)
	$(LD) $(LDFLAGS) $(OBJFILES) -o $(OUTFILE)

$(OBJDIR):
	mkdir -p $@

bin/%.o: src/%.cpp | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -o $@ -c $<

.PHONY: debug
debug: $(OUTFILE).debug
	gdb -tui $(OUTFILE).debug

$(OUTFILE).debug: $(OBJDEBUG)
	$(LD) $(LDFLAGS) $(OBJDEBUG) -o $(OUTFILE).debug

bin/%.debug.o: src/%.cpp | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -ggdb3 -o $@ -c $<

.PHONY: clean
clean:
	rm -rf $(OBJDIR)
	rm -f $(OUTFILE)
	rm -f $(OUTFILE).debug
