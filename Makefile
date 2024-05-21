FLAGS := -std=c++17 -Wall -Wextra -O3
srcdir := ./src
outdir := ./out

OBJECTS := $(outdir)/strutil.o $(outdir)/randutil.o $(outdir)/node.o $(outdir)/boardtries.o $(outdir)/board.o $(outdir)/solver.o

main: $(outdir)/main.o
	g++ $(FLAGS) $(outdir)/main.o $(OBJECTS) -o main

$(outdir)/main.o: $(srcdir)/main.cpp $(OBJECTS)
	g++ $(FLAGS) -c $< -o $@

$(outdir)/%.o: $(srcdir)/%.cpp $(srcdir)/%.h $(outdir)
	g++ $(FLAGS) -c $< -o $@

$(outdir)/%.o: $(srcdir)/utils/%.cpp $(srcdir)/utils/%.h $(outdir)
	g++ $(FLAGS) -c $< -o $@

$(outdir):
	mkdir out

.PHONY: clean

clean: 
	rm -rf $(outdir)
	rm main
