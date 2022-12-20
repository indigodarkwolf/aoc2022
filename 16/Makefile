cflags := --std=c++20 $(dflags)
ldflags := $(dflags)
objdir=obj/$(build)

aoc_srcs := $(wildcard *.cpp)
aoc_objs := $(patsubst %.cpp,$(objdir)/%.o,$(aoc_srcs))

default:
	$(MAKE) optimized

debug:
	$(MAKE) -j8 build build="debug" dflags="-g"

optimized:
	$(MAKE) -j8 build build="optimized" dflags="-O3"

build: aoc

clean:
	rm -rf obj

$(objdir):
	mkdir -p $@

$(objdir)/%.o: %.cpp | $(objdir)
	g++ $(cflags) -c $< -o $@

aoc: $(aoc_objs)
	g++ $^ -o $@ $(ldflags)