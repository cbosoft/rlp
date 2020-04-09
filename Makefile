CXX = g++
CFLAGS = -g -pg -Wall -Wextra -Werror -std=c++17 -O2

PARTICLE = \
					 obj/particle/particle.o

UTIL = \
			 obj/util/vec.o \
			 obj/util/random.o

CONFIG = \
				 obj/config/config.o \
				 obj/config/generate.o

BOX = \
			obj/box/box.o \
			obj/box/bounds.o \
			obj/box/arrangements.o \
			obj/box/particle.o \
			obj/box/volume.o

ARRANGEMENT = \
							obj/arrangement/vertex.o \
							obj/arrangement/line.o \
							obj/arrangement/triangle.o

TEST = \
			 obj/test/run_tests.o

HDR = $(shell ls src/**/*.hpp)
OBJ = $(PARTICLE) $(UTIL) $(CONFIG) $(BOX) $(ARRANGEMENT) $(TEST)
LINK =
EXE = rlp
DEFS =

.SECONDARY:

obj/%.o: src/%.cpp $(HDR)
	@echo -e "\u001b[33mASSEMBLING OBJECT $@\u001b[0m"
	mkdir -p `dirname $@`
	$(CXX) $(CFLAGS) $(DEFS) $< -c -o $@

obj/test/%.o: src/test/%.cpp $(HDR)
	@echo -e "\u001b[33mASSEMBLING OBJECT $@\u001b[0m"
	mkdir -p `dirname $@`
	$(CXX) $(CFLAGS) $(DEFS) $< -c -o $@


.PHONY: exe

exe: obj/main.o $(OBJ) $(HDR)
	@echo -e "\u001b[34mLINKING OBJECTS TO EXECUTABLE $@\u001b[0m"
	$(CXX) $(CFLAGS) $(DEFS) obj/main.o $(OBJ) -o $(EXE) $(LINK)

tests: src/test/main.cpp $(OBJ) $(HDR)
	@echo -e "\u001b[34mLINKING OBJECTS TO EXECUTABLE $@\u001b[0m"
	$(CXX) $(CFLAGS) $(DEFS) src/test/main.cpp $(OBJ) -o $@ $(LINK)

prof_pdf:
	gprof $(EXE) gmon.out > analysis.txt
	gprof2dot -o d.dot analysis.txt
	dot -Tpdf d.dot > prof.pdf

clean:
	rm -rf obj $(EXE) tests
