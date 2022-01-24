#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <vector>

#include "map.hpp"
#include "enemy.hpp"

#ifndef PROJECTILE_CLASS
#define PROJECTILE_CLASS

class Player; //Forward declaration is used here to avoid circular dependency with player class
/**Class for projectiles */
class Projectile {
 public:
  /**Constructor*/
  Projectile(sf::Texture* texture);
  /**Default destructor*/
  ~Projectile();
  /**Renders the projectile*/
  void Draw(sf::RenderWindow& window);
  /**Updates attributes*/
  bool Update(float time, Map& map, Player& player);
  /**Changes active_ to true*/
  void activate();
  /**Changes active_ to false*/
  void deActivate();
  /**Returns the projectile collider*/
  Collider getCollider() {
    return Collider(body_);
  }

  sf::Texture* projectile_texture_;
  sf::RectangleShape body_;
  sf::Vector2f projectile_trajectory_;
  bool active_ = false;
  float actionTimer_ = 0.0;
};

#endif
