CXX = g++
CFLAGS = -g -pg -Wall -Wextra -Werror -std=c++17 -O2
OBJ = \
			obj/particle.o \
			obj/box.o \
			obj/box_bounds.o \
			obj/box_arrangements.o \
			obj/box_particle.o \
			obj/config.o \
			obj/config_generate.o \
			obj/vec.o \
			obj/vertex.o \
			obj/line.o \
			obj/triangle.o \
			obj/random.o \
			obj/test/run_tests.o
HDR = src/vec.hpp \
			src/particle.hpp \
			src/box.hpp \
			src/config.hpp \
			src/exception.hpp \
			src/arrangement.hpp \
			src/triangle.hpp \
			src/line.hpp \
			src/logger.hpp \
			src/vertex.hpp \
			src/colour.hpp
TEST_HDR = $(shell ls src/test/*.hpp)
LINK =
EXE = rlp
DEFS =

.SECONDARY:

obj/%.o: src/%.cpp $(HDR)
	@echo -e "\u001b[33mASSEMBLING OBJECT $@\u001b[0m"
	mkdir -p `dirname $@`
	$(CXX) $(CFLAGS) $(DEFS) $< -c -o $@

obj/test/%.o: src/test/%.cpp $(HDR) $(TEST_HDR)
	@echo -e "\u001b[33mASSEMBLING OBJECT $@\u001b[0m"
	mkdir -p `dirname $@`
	$(CXX) $(CFLAGS) $(DEFS) $< -c -o $@


.PHONY: exe

exe: obj/main.o $(OBJ) $(HDR) $(TEST_HDR)
	@echo -e "\u001b[34mLINKING OBJECTS TO EXECUTABLE $@\u001b[0m"
	$(CXX) $(CFLAGS) $(DEFS) obj/main.o $(OBJ) -o $(EXE) $(LINK)

tests: src/test/main.cpp $(OBJ) $(HDR) $(TEST_HDR)
	@echo -e "\u001b[34mLINKING OBJECTS TO EXECUTABLE $@\u001b[0m"
	$(CXX) $(CFLAGS) $(DEFS) src/test/main.cpp $(OBJ) -o $@ $(LINK)

prof_pdf:
	gprof $(EXE) gmon.out > analysis.txt
	gprof2dot -o d.dot analysis.txt
	dot -Tpdf d.dot > prof.pdf

clean:
	rm -rf obj $(EXE) tests
