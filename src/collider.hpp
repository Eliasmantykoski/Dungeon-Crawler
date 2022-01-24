#ifndef COLLIDER_CLASS
#define COLLIDER_CLASS

#include "SFML/Graphics.hpp"
/**Class for handling collisions*/
class Collider {
 public:
  /**Constructor*/
  Collider(sf::RectangleShape& body);
  /**Default destructor*/
  ~Collider();
  /**Moves object out of collision */
  void Move(float dx, float dy) { body_.move(dx, dy); }
  /**Checks if two colliders are colliding*/
  bool checkCollider(Collider& other, float push);
  /**Returns collider position*/
  sf::Vector2f getPosition() { return body_.getPosition(); }
  /**Returns collider size divided by 2*/
  sf::Vector2f getHalfSize() { return body_.getSize() / 2.0f; }

 private:
  sf::RectangleShape& body_;
};

#endif
