include Make.inc
#include Make_msys2.inc
#include Make_osx.inc

CXXFLAGS = -std=c++11 -fPIC
ifdef DEBUG
CXXFLAGS += -g -O0 -Wall -fbounds-check -pedantic -D_GLIBCXX_DEBUG
else
CXXFLAGS += -O3 -march=native -Wall
endif

default:	help

all:	hello_world.exe circulation_jeton.exe calcul_de_pi.exe diffusion_hypercube.exe

hello_world.exe:	hello_world.cpp
	$(CXX) $(CXXFLAGS) -o hello_world.exe hello_world.cpp $(LIB)

circulation_jeton.exe:	circulation_jeton.cpp
	$(CXX) $(CXXFLAGS) -o circulation_jeton.exe circulation_jeton.cpp $(LIB)

calcul_de_pi.exe:	calcul_de_pi.cpp
	$(CXX) $(CXXFLAGS) -o calcul_de_pi.exe calcul_de_pi.cpp $(LIB)

diffusion_hypercube.exe:	diffusion_hypercube.cpp
	$(CXX) $(CXXFLAGS) -o diffusion_hypercube.exe diffusion_hypercube.cpp $(LIB)

help: 
	@echo "Available targets : "
	@echo "    all                      : compile all executables"
	@echo "    hello_world.exe           : compile HelloWorld executable"
	@echo "    circulation_jeton.exe    : compile Circulation_jeton executable"
	@echo "    calcul_de_pi.exe         : compile Calcul_de_pi executable"
	@echo "    diffusion_hypercube.exe  : compile Diffusion_hypercube executable"
	@echo "Add DEBUG=yes to compile in debug"
	@echo "Configuration :"
	@echo "    CXX      :    $(CXX)"
	@echo "    CXXFLAGS :    $(CXXFLAGS)"

clean:
	@rm -f *.exe *~ 

