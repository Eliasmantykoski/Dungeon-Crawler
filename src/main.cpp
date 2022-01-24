// https://github.com/SFML/SFML/issues/1673
#include <stdlib.h>
#include <time.h>

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <cstdlib>
#include <iostream>
#include <vector>

#include "HUD.hpp"
#include "animation.hpp"
#include "collider.hpp"
#include "enemy.hpp"
#include "menu.hpp"
#include "player.hpp"
#include "projectile.hpp"
using namespace sf;

const int WIDTH = 500;
const int HEIGHT = 650;

/*
Here is a simple example setup for SFML code, which shows how SFML works, this
main file will be modified as new classes and functionalities are introduced
into the project. But until that this code can be used to test wether SFML works
on the local machine or not.

This code can be run by calling the command "make" in the VSCode terminal after
make is installed and then the local folder should create an executable .exe
file, which can be run by double clicking it in the project file path.

In order to run SFML, SFML and mingw32 have to be downloaded from the SFML
website. After this the downloaded folders have to be put in some file path that
matches the one in the Makefile(SFML_DOWNLOAD_PATH and MINGW_DOWNLOAD_PATH). The
bin folders inside mingw32 and SFML-2.5.1 have to be added into environment file
variables: On your system search for Edit environment variables for your account
--> from user variables select Path and edit --> click on new and copy the bin
folder filepaths there
*/
// TODO: negative y direction collision, player damage cooldown
int main() {
  RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "SFML window");
  window.setFramerateLimit(30);  // Setting the framerate limit lower helps the
                                 // program be less CPU intensive.
  srand(time(NULL));
  // Font for text
  sf::Font font;
  if (!font.loadFromFile("libs/fonts/arial.ttf")) {
    std::cout << "Error loading font."
              << "\n";
  }
  sf::Text text;
  text.setFont(font);
  text.setString("GAME OVER");
  text.setCharacterSize(24);
  text.setPosition(WIDTH / 2, HEIGHT / 2);

  // Music
  sf::Music music;
  if (!music.openFromFile("libs/audio/music.ogg")) {
    std::cout << "Error loading music."
              << "\n";
  }
  music.setRelativeToListener(true);
  music.setVolume(20);
  music.setLoop(true);
  music.play();

  // Sounds
  sf::SoundBuffer punchbuf;
  punchbuf.loadFromFile("libs/audio/punch.ogg");
  sf::Sound punch;
  punch.setBuffer(punchbuf);

  sf::SoundBuffer deathbuf;
  deathbuf.loadFromFile("libs/audio/death.ogg");
  sf::Sound death;
  death.setBuffer(deathbuf);

  sf::SoundBuffer shootbuf;
  shootbuf.loadFromFile("libs/audio/shot.ogg");
  sf::Sound shoot;
  shoot.setBuffer(shootbuf);

  sf::SoundBuffer drinkbuf;
  drinkbuf.loadFromFile("libs/audio/drink.ogg");
  sf::Sound drink;
  drink.setBuffer(drinkbuf);

  sf::SoundBuffer pickbuf;
  pickbuf.loadFromFile("libs/audio/pickup.ogg");
  sf::Sound pickup;
  pickup.setBuffer(pickbuf);

  // Textures
  sf::Texture* walltexture = new sf::Texture();
  walltexture->loadFromFile("libs/images/wall_left.png");

  // sf::Texture enemytexture;
  // sf::IntRect enemytexture_crop(64, 11, 17, 21);
  // enemytexture.loadFromFile("libs/images/pumpkin_dude.png",
  // enemytexture_crop);
  Animation enemyAnimation("libs/images/pumpkin_dude.png", 16, 23);
  // sf::Texture enemytexture2;
  // sf::IntRect enemytexture_crop2(64, 11, 17, 21);
  // enemytexture2.loadFromFile("libs/images/skeleton_dude.png",
  // enemytexture_crop2);
  Animation enemyAnimation2("libs/images/skeleton_dude.png", 16, 20);

  Animation boss_animation("libs/images/boss.png", 32, 36);

  Texture weapon_texture;
  weapon_texture.loadFromFile("libs/images/simple gun.png");
  Texture projectile_texture;
  projectile_texture.loadFromFile("libs/images/simple bullet.png");
  Texture enemy_projectile_texture;
  enemy_projectile_texture.loadFromFile("libs/images/enemy bullet.png");
  sf::Texture playerTexture;
  playerTexture.loadFromFile("libs/images/character.png");
  sf::Texture* heart_texture = new sf::Texture();
  heart_texture->loadFromFile("libs/images/ui_heart_full.png");
  sf::Texture* heart_half_texture = new sf::Texture();
  heart_half_texture->loadFromFile("libs/images/ui_heart_half.png");
  sf::Texture* heart_empty_texture = new sf::Texture();
  heart_empty_texture->loadFromFile("libs/images/ui_heart_empty.png");
  sf::Texture* backgroundtexture = new sf::Texture();
  backgroundtexture->loadFromFile("libs/images/floor_1.png");
  sf::Texture player_death_texture;
  player_death_texture.loadFromFile("libs/images/character_grave.png");

  sf::Texture healPotTexture;
  healPotTexture.loadFromFile("libs/images/flask_big_red.png");
  sf::Texture speedPotTexture;
  speedPotTexture.loadFromFile("libs/images/flask_big_blue.png");
  sf::Texture coinTexture;
  coinTexture.loadFromFile("libs/images/coin.png");
  sf::Texture* healPotTexture2 = new sf::Texture();
  healPotTexture2->loadFromFile("libs/images/flask_big_red.png");
  sf::Texture* speedPotTexture2 = new sf::Texture();
  speedPotTexture2->loadFromFile("libs/images/flask_big_blue.png");
  sf::Texture* coinTexture2 = new sf::Texture();
  coinTexture2->loadFromFile("libs/images/coin.png");
  std::vector<sf::Texture*> item_textures = {healPotTexture2, speedPotTexture2,
                                             coinTexture2};
  std::vector<sf::Texture*> heart_textures = {heart_texture, heart_half_texture,
                                              heart_empty_texture};
  Menu menu(font, walltexture);

  // Generate map and player
  HUD hud(font, heart_textures, backgroundtexture, backgroundtexture,
          item_textures);
  // Map map(walltexture, &enemyAnimation, &enemyAnimation2, &boss_animation,
  // senemy_projectile_texture);

  std::vector<Projectile> active_projectiles;  // Keep track of projectiles that
                                               // exist inside the SFML window.

  // Generate map and player
  Map map(walltexture, &enemyAnimation, &enemyAnimation2, &boss_animation,
          &enemy_projectile_texture);

  Weapon player_starting_weapon = Weapon(&weapon_texture, &projectile_texture);

  float player_speed = 200;
  Vector2f spawnPos = Vector2f(100.0f, 100.0f);
  Player player(&playerTexture, &player_death_texture, spawnPos, player_speed,
                &player_starting_weapon);

  Clock clock;
  int resetCounter = 1;
  // This while loop runs as long as window is open. In this case it runs until
  // window.close(); is called.
  while (window.isOpen()) {
    Event event;
    if (menu.active_) {
      while (window.pollEvent(event)) {
        if (event.type == Event::Closed) {
          window.close();
        }  // This event closes the window when close button is pressed.
      }
      resetCounter = 0;
      window.clear();
      menu.Display(window);
      window.display();
    } else if (resetCounter == 0) {
      srand(clock.restart().asMilliseconds());
      map = Map(walltexture, &enemyAnimation, &enemyAnimation2, &boss_animation,
                &enemy_projectile_texture);
      player = Player(&playerTexture, &player_death_texture, spawnPos,
                      player_speed, &player_starting_weapon);
      active_projectiles.clear();
      resetCounter++;
    } else {
      float time = clock.restart().asSeconds();

      // This while loop goes through all window events, such as key presses
      // and mouse presses.

      while (window.pollEvent(event)) {
        if (event.type == Event::Closed) {
          window.close();
        }  // This event closes the window when close button is pressed.
      }

      std::vector<int> sounds = player.Update(time, map, active_projectiles);
      if (sounds[0]) {
        shoot.play();
      }
      if (sounds[1]) {
        punch.play();
      }
      if (sounds[2]) {
        drink.play();
      }
      if (sounds[3]) {
        pickup.play();
      }
      // update enemies and remove them if they are dead
      for (auto& enemy : map.enemies) {
        if (enemy->isRanged()) {
          enemy->Update2(time, player.getPosition(), active_projectiles);
        } else {
          enemy->Update(time, player.getPosition());
        }
        for (unsigned int i = 0; i < map.enemies.size(); i++) {
          if (!map.enemies[i]->alive_) {
            if (map.enemies[i]->boss) {
              menu.texts_[0].setString("You have defeated the boss!");
              menu.texts_[0].setFillColor(sf::Color::Green);
              menu.active_ = true;
            }
            map.SpawnItem(i, &healPotTexture, &speedPotTexture, &coinTexture);
            map.enemies.erase(map.enemies.begin() + i);
            death.play();
            
          }
        }
      }

      // update active projectiles and remove them if they have hit a wall or
      // an enemy
      for (auto& proj : active_projectiles) {
        // sf::Vector2u secret_size = proj.projectile_texture_->getSize();
        // std::cout << secret_size.x << " " << secret_size.y << "\n";
        if (proj.Update(time, map, player)) {
          proj.deActivate();
          for (unsigned int i = 0; i < active_projectiles.size(); i++) {
            if (!active_projectiles[i].active_) {
              active_projectiles.erase(active_projectiles.begin() + i);
            }
          }
        }
      }
      // std::cout << active_projectiles.size() << "\n";

      window.clear();
      Collider playerCollider = player.getCollider();
      map.Display(window, playerCollider);
      player.Draw(window);

      for (auto& enemy : map.enemies) {
        enemy->Draw(window);
      }
      for (auto& proj : active_projectiles) {
        proj.Draw(window);
      }

      hud.Display(window);
      hud.Update(player.items_, player.hp_);

      if (player.isAlive() == false) {
        // window.draw(text);
        // window.clear();
        menu.texts_[0].setString("You Died!");
        menu.active_ = true;
      }
      window.display();
    }
  }

  // for (auto& enemy : map.enemies) {
  //   delete enemy;
  // }
  delete walltexture;
  delete backgroundtexture;
  return 0;
}
