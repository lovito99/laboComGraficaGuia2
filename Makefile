CXX      = g++
CXXFLAGS = -Wall -Wextra -std=c++11
LIBS     = -lGL -lGLU -lglut

TARGETS  = algoritmos_lineas ejercicio_propuesto

.PHONY: all clean run_algoritmos run_ejercicio

all: $(TARGETS)

algoritmos_lineas: algoritmos_lineas.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LIBS)

ejercicio_propuesto: ejercicio_propuesto.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LIBS)

run_algoritmos: algoritmos_lineas
	./algoritmos_lineas

run_ejercicio: ejercicio_propuesto
	./ejercicio_propuesto

clean:
	rm -f $(TARGETS)
