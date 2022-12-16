cflags := --std=c++20 -Werror $(dflags)
ldflags := 
objdir=obj

aoc_srcs := $(wildcard *.cpp)
aoc_objs := $(patsubst %.cpp,$(objdir)/%.o,$(aoc_srcs))

default: aoc

clean:
	rm -rf obj

$(objdir):
	mkdir -p $@

$(objdir)/%.o: %.cpp | $(objdir)
	g++ $(cflags) -c $< -o $@

aoc: $(aoc_objs)
	g++ $^ -o $@ $(ldflags)