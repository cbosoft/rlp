CXX = g++
CFLAGS = -g -pg -Wall -Wextra -Werror -std=c++17 -O2

PARTICLE = \
					 obj/particle/particle.o

UTIL = \
			 obj/util/vec.o \
			 obj/util/args.o \
			 obj/util/help.o \
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

SIEVE = \
				obj/sieve/mono.o \
				obj/sieve/bi.o \
				obj/sieve/normal.o \
				obj/sieve/uniform.o \
				obj/sieve/altbi.o

TEST = \
			 obj/test/run_tests.o \
			 obj/test/line/creation.o \
			 obj/test/line/interaction.o \
			 obj/test/vec/angle.o \
			 obj/test/vec/cross.o \
			 obj/test/vec/dot.o \
			 obj/test/vec/mag.o \
			 obj/test/vec/ortho.o \
			 obj/test/vertex/creation.o \
			 obj/test/vertex/interaction.o \
			 obj/test/multi/1.o \
			 obj/test/triangle/creation.o \
			 obj/test/triangle/trilateration.o \
			 obj/test/volume/basic.o \
			 obj/test/volume/cap.o \
			 obj/test/volume/wall.o

HDR = $(shell ls src/**/*.hpp)
OBJ = $(PARTICLE) $(UTIL) $(CONFIG) $(BOX) $(ARRANGEMENT) $(SIEVE) $(TEST)
LINK =
DEFS =

.SECONDARY:

obj/%.o: src/%.cpp $(HDR)
	@echo -e "\u001b[33mASSEMBLING OBJECT $@\u001b[0m"
	@mkdir -p `dirname $@`
	@$(CXX) $(CFLAGS) $(DEFS) $< -c -o $@

obj/test/%.o: src/test/%.cpp $(HDR)
	@echo -e "\u001b[33mASSEMBLING OBJECT $@\u001b[0m"
	@mkdir -p `dirname $@`
	@$(CXX) $(CFLAGS) $(DEFS) $< -c -o $@


rlp: obj/main.o $(OBJ) $(HDR)
	@echo -e "\u001b[34mLINKING OBJECTS TO EXECUTABLE $@\u001b[0m"
	@$(CXX) $(CFLAGS) $(DEFS) obj/main.o $(OBJ) -o rlp $(LINK)

tests: src/test/main.cpp $(OBJ) $(HDR)
	@echo -e "\u001b[34mLINKING OBJECTS TO EXECUTABLE $@\u001b[0m"
	@$(CXX) $(CFLAGS) $(DEFS) src/test/main.cpp $(OBJ) -o $@ $(LINK)

prof_pdf:
	gprof $(EXE) gmon.out > analysis.txt
	gprof2dot -o d.dot analysis.txt
	dot -Tpdf d.dot > prof.pdf

clean:
	rm -rf obj rlp tests
