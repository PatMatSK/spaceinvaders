CXX=g++
FLAGS = -std=c++17 -pedantic -O2

.PHONY: all compile run clean deps doc

all: src/*.cpp
	mkdir -p build
	$(CXX) $(FLAGS)  -c src/imports.cpp -o build/imports.o
	$(CXX) $(FLAGS)  -c src/Object.cpp -o build/Object.o
	$(CXX) $(FLAGS)  -c src/SpaceShip.cpp -o build/SpaceShip.o
	$(CXX) $(FLAGS)  -c src/Enemy.cpp -o build/Enemy.o
	$(CXX) $(FLAGS)  -c src/EnemyBasic.cpp -o build/EnemyBasic.o
	$(CXX) $(FLAGS)  -c src/EnemySnake.cpp -o build/EnemySnake.o
	$(CXX) $(FLAGS)  -c src/EnemyCrazy.cpp -o build/EnemyCrazy.o
	$(CXX) $(FLAGS)  -c src/EnemyArmy.cpp -o build/EnemyArmy.o
	$(CXX) $(FLAGS)  -c src/Bullet.cpp -o build/Bullet.o
	$(CXX) $(FLAGS)  -c src/Obstacle.cpp -o build/Obstacle.o
	$(CXX) $(FLAGS)  -c src/Level.cpp -o build/Level.o
	$(CXX) $(FLAGS)  -c src/main.cpp -o build/main.o

compile: all
	$(CXX) $(FLAGS) -o matulpat build/*.o -lncurses

clean:
	rm -rf build Makefile.d matulpat

deps:
	$(CXX) -MM src/*.cpp > Makefile.d

-include Makefile.d

run: compile
	./matulpat

doc:
	echo "Documenation already exists."

.PHONY: clean
