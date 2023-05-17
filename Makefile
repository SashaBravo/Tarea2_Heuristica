# Variables de compilación
CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra

# Objetivos principales
all: Akari

# Reglas de compilación para los archivos individuales

Akari: Akari.cpp Akari.h
	$(CXX) $(CXXFLAGS) -o3 $< -o $@

# Limpieza de archivos intermedios y el programa compilado
clean:
	rm -f *.o