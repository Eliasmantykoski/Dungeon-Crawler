# When SFML and mingw32 have been downloaded from the SFML website, their locations should match these filepaths.
SFML_DOWNLOAD_PATH = C:/SFML-2.5.1
MINGW_DOWNLOAD_PATH = C:/mingw32

all: main.o map.o room.o wall.o weapon.o projectile.o player.o enemy.o animation.o collider.o item.o HUD.o menu.o link

output: main.o link
	g++ main.o -o output

main.o: ./src/main.cpp ./src/map.hpp ./src/room.hpp ./src/wall.hpp ./src/weapon.hpp ./src/projectile.hpp ./src/player.hpp ./src/enemy.hpp ./src/animation.hpp ./src/collider.hpp ./src/item.hpp ./src/HUD.hpp ./src/menu.hpp
	g++ -c -ggdb ./src/main.cpp -I libs/include

map.o: ./src/map.cpp ./src/map.hpp
	g++ -c -ggdb ./src/map.cpp -I libs/include

room.o: ./src/room.cpp ./src/room.hpp
	g++ -c -ggdb ./src/room.cpp -I libs/include

wall.o: ./src/wall.cpp ./src/wall.hpp
	g++ -c -ggdb ./src/wall.cpp -I libs/include

player.o: ./src/player.cpp ./src/player.hpp
	g++ -c ./src/player.cpp -I libs/include

enemy.o: ./src/enemy.cpp ./src/enemy.hpp
	g++ -c ./src/enemy.cpp -I libs/include

weapon.o: ./src/weapon.cpp ./src/weapon.hpp
	g++ -c ./src/weapon.cpp -I libs/include

projectile.o: ./src/projectile.cpp ./src/projectile.hpp
	g++ -c ./src/projectile.cpp -I libs/include

animation.o: ./src/animation.cpp ./src/animation.hpp
	g++ -c ./src/animation.cpp -I libs/include

collider.o:
	g++ -c ./src/collider.cpp -I libs/include

HUD.o: ./src/HUD.cpp ./src/HUD.hpp
	g++ -c ./src/HUD.cpp -I libs/include

item.o: ./src/item.cpp ./src/item.hpp
	g++ -c ./src/item.cpp -I libs/include

menu.o: ./src/menu.hpp ./src/menu.cpp
	g++ -c ./src/menu.cpp -I libs/include

link:
	g++ main.o map.o room.o wall.o player.o projectile.o weapon.o enemy.o animation.o collider.o item.o HUD.o menu.o -o sfml-app -L $(SFML_DOWNLOAD_PATH)/lib -l sfml-graphics -l sfml-window -l sfml-system -l sfml-audio
#g++ main.o -o sfml-app -L $(SFML_DOWNLOAD_PATH)/lib -l sfml-graphics -l sfml-window -l sfml-system -l sfml-audio -l sfml-network

clean:
	rm *.o sfml-app

memcheck:
	valgrind --leak-check=yes ./sfml-app

# requires installing clang-format and cpplint
format:
	clang-format -style=Google -i ./src/main.cpp ./src/map.cpp ./src/room.cpp ./src/wall.cpp ./src/player.cpp ./src/collider.cpp ./src/HUD.cpp ./src/item.cpp ./src/menu.cpp ./src/map.hpp ./src/room.hpp ./src/wall.hpp ./src/player.hpp ./src/collider.hpp ./src/item.hpp ./src/HUD.hpp ./src/menu.hpp
