CXX=g++
FLAGS = -std=c++17 -pedantic -O2

.PHONY: all compile run clean matulpat deps doc

all: matulpat

matulpat: compile
	$(CXX) $(FLAGS) -o matulpat build/*.o -lncurses

compile: src/*.cpp
	mkdir -p build
	$(CXX) $(FLAGS)  -c src/imports.cpp -o build/imports.o
	$(CXX) $(FLAGS)  -c src/main.cpp -o build/main.o
	$(CXX) $(FLAGS)  -c src/Level.cpp -o build/Level.o
	$(CXX) $(FLAGS)  -c src/Object.cpp -o build/Object.o
	$(CXX) $(FLAGS)  -c src/SpaceShip.cpp -o build/SpaceShip.o
	$(CXX) $(FLAGS)  -c src/Enemy.cpp -o build/Enemy.o
	$(CXX) $(FLAGS)  -c src/EnemyArmy.cpp -o build/EnemyArmy.o
	$(CXX) $(FLAGS)  -c src/Bullet.cpp -o build/Bullet.o
	$(CXX) $(FLAGS)  -c src/Obstacle.cpp -o build/Obstacle.o

clean:
	rm -rf build Makefile.d matulpat

deps:
	$(CXX) -MM src/*.cpp > Makefile.d

-include Makefile.d

run: all
	./matulpat

doc:
	echo "Documenation already exists."

.PHONY: clean
