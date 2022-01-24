#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>

#include "projectile.hpp"

#ifndef WEAPON_CLASS
#define WEAPON_CLASS
/**Class for weapon*/
class Weapon {
 public:
  /**Constructor*/
  Weapon(sf::Texture* texture, sf::Texture* proj_texture);
  /**Default destructor*/
  ~Weapon();

  /**Creates projectile*/
  Projectile Fire(sf::Vector2f fire_position, sf::Vector2f fire_trajectory);

  sf::RectangleShape weapon_body_;
  //Projectile weapon_projectile_*;
  std::vector<Projectile> weapon_projectile_; //This vector is used instead of a pointer, because pointers give compile errors
  float fire_rate_ = 0.1;
};

#endif
