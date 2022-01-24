#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>
#include <memory>
#include <vector>

#include "animation.hpp"
#include "collider.hpp"
#include "item.hpp"
#include "room.hpp"

#ifndef MAP_CLASS
#define MAP_CLASS

class Projectile;  // Forward declaration is used here to avoid circular
                   // dependency with projectile class
/**Class for the map*/
class Map {
 public:
  /**Constructor for map*/
  Map(sf::Texture* wall_texture, Animation* enemy_animation,
      Animation* enemy_animation2, Animation* boss_animation,
      sf::Texture* enemy_projectile_texture);
  /**Generates random dungeon and spawns random amount of enemies to each
  /*room.*/
  bool Generate(sf::Texture* wall_texture, Animation* enemy_animation,
                Animation* enemy_animation2, Animation* boss_animation,
                sf::Texture* enemy_projectile_texture, int x, int y,
                std::vector<int> openings,
                std::vector<std::pair<int, int>>& visited);
  /**Used for rendering the map*/
  void Display(sf::RenderWindow& window, Collider playerCollider);
  /**Used for whern player traverses between rooms.*/
  void NextRoom(int direction);
  /**Used for spawning random item*/
  void SpawnItem(unsigned int& i, sf::Texture* hp_text, sf::Texture* speed_text,
                 sf::Texture* coin_text);
  /**Used for picking up items*/
  void removeItem(std::vector<Item>::iterator i);

  std::vector<Room> rooms;
  // std::vector<unique_ptr<Enemy>> enemies;
  // std::vector<Enemy> enemies;
  std::vector<Enemy*> enemies;
  std::vector<std::vector<Room>> layout;
  sf::Vector2f mapSize = sf::Vector2f(6.0f, 6.0f);
  std::vector<Item> items;
};

#endif
