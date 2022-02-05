LIBS = -L./lib -lglfw3dll -lglfw3 -lopengl32 -lglew32 -lmingw32 -lSDL2main -lSDL2 -lSDL2_image

# ****************************************************

Bomberman.exe: image.o main.o
	g++ *.o $(LIBS) -o ../Bomberman.exe
main.o: image.h main.cpp
	g++ $(LIBS) -c main.cpp
image.o: image.h image.cpp
	g++ $(LIBS) -c image.cpp


