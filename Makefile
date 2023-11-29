all:
	g++ -Isrc/include -L src/lib -o main main.cpp GameObject.cpp  RandomLetter.cpp TextureManager.cpp -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf

fclean:
	del main.exe
