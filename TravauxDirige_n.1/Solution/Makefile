#include Make.inc
include Make_msys2.inc
#include Make_osx.inc

CXXFLAGS = -std=c++11 -fPIC
ifdef DEBUG
CXXFLAGS += -g -O0 -Wall -fbounds-check -pedantic -D_GLIBCXX_DEBUG
else
CXXFLAGS += -O3 -march=native -Wall
endif

default:	help

all:	HelloWorld.exe HelloWorld_en_fichier.exe \
		Circulation_jeton_synchrone.exe Circulation_jeton_asynchrone.exe \
		Calcul_de_pi.exe \
		Diffusion_hypercube_2proc.exe Diffusion_hypercube_4proc.exe Diffusion_hypercube_nproc.exe

HelloWorld.exe:	HelloWorld.cpp
	$(CXX) $(CXXFLAGS) -o HelloWorld.exe HelloWorld.cpp $(LIB)

HelloWorld_en_fichier.exe:	HelloWorld_en_fichier.cpp
	$(CXX) $(CXXFLAGS) -o HelloWorld_en_fichier.exe HelloWorld_en_fichier.cpp $(LIB)

Circulation_jeton_synchrone.exe:	Circulation_jeton_synchrone.cpp
	$(CXX) $(CXXFLAGS) -o Circulation_jeton_synchrone.exe Circulation_jeton_synchrone.cpp $(LIB)

Circulation_jeton_asynchrone.exe:	Circulation_jeton_asynchrone.cpp
	$(CXX) $(CXXFLAGS) -o Circulation_jeton_asynchrone.exe Circulation_jeton_asynchrone.cpp $(LIB)

Calcul_de_pi.exe:	Calcul_de_pi.cpp
	$(CXX) $(CXXFLAGS) -o Calcul_de_pi.exe Calcul_de_pi.cpp $(LIB)

Diffusion_hypercube_2proc.exe:	Diffusion_hypercube_2proc.cpp
	$(CXX) $(CXXFLAGS) -o Diffusion_hypercube_2proc.exe Diffusion_hypercube_2proc.cpp $(LIB)

Diffusion_hypercube_4proc.exe:	Diffusion_hypercube_4proc.cpp
	$(CXX) $(CXXFLAGS) -o Diffusion_hypercube_4proc.exe Diffusion_hypercube_4proc.cpp $(LIB)

Diffusion_hypercube_nproc.exe:	Diffusion_hypercube_nproc.cpp
	$(CXX) $(CXXFLAGS) -o Diffusion_hypercube_nproc.exe Diffusion_hypercube_nproc.cpp $(LIB)


help: 
	@echo "Available targets : "
	@echo "    all                      : compile all executables"
	@echo "    HelloWorld.exe           : compile HelloWorld executable"
	@echo "    Circulation_jeton.exe    : compile Circulation_jeton executable"
	@echo "    Calcul_de_pi.exe         : compile Calcul_de_pi executable"
	@echo "    Diffusion_hypercube.exe  : compile Diffusion_hypercube executable"
	@echo "Add DEBUG=yes to compile in debug"
	@echo "Configuration :"
	@echo "    CXX      :    $(CXX)"
	@echo "    CXXFLAGS :    $(CXXFLAGS)"

clean:
	@rm -f *.exe *~ 

