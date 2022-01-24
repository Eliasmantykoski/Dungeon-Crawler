#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>
#include <vector>

#include "item.hpp"
#include "map.hpp"
#include "projectile.hpp"
#include "weapon.hpp"

#ifndef PLAYER_CLASS
#define PLAYER_CLASS
/**Class for the player object*/
class Player {
 public:
  /**Constructor for player*/
  Player(sf::Texture* texture, sf::Texture* dead_texture, sf::Vector2f spawnPos,
         float speed, Weapon* starting_weapon);
  /**Default constructor*/
  ~Player();
  /**Renders the player*/
  void Draw(sf::RenderWindow& window);

  // void GiveWeapon(Weapon weapon) {player_weapon_ = weapon;};
  /**Returns player position*/
  sf::Vector2f getPosition();
  /**Return alive*/
  bool isAlive();
  /**Return healh*/
  float getHp();

  float getShield();
  /**Uses item from inventory*/
  bool useItem();
  /**Decrease health*/
  void Damage(float dmg);
  /**Update attributes*/
  std::vector<int> Update(float time, Map& map, std::vector<Projectile>&);
  /**Creates projectile*/
  Projectile UseWeapon(sf::Vector2f&);
  /**Moves the map when player traverses between rooms*/
  void Traverse(Map& map);
  /**Returns the player collider*/
  Collider getCollider() { return Collider(body_); }

  float speed_;
  float hp_ = 6;
  float shield_ = 0;
  float currentSpeed_;
  float actionTimer_;
  float shot_ = 0.0;
  float damaged_ = 0.0;
  int chosen_weapon_ = 0;
  float speedTimer = 0;
  float itemTimer = 0;
  bool alive_ = true;
  bool dead_ = false;
  sf::Texture* death_texture_;
  std::vector<Weapon> player_weapons_;
  std::vector<Item> items_;
  sf::RectangleShape body_;
  sf::Vector2f direction_;
};

#endif
