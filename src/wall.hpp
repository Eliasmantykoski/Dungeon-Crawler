#ifndef WALL_CLASS
#define WALL_CLASS

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>

#include "collider.hpp"
/**Class for walls*/
class Wall {
 public:
  /**Constructor*/
  Wall(sf::Texture* texture, sf::Vector2f size, sf::Vector2f position,
       int type);
  /**Renders the wall*/
  void Draw(sf::RenderWindow& window);
  /**Moves the wall*/
  void Move2(sf::Vector2f dir);
  /**Checks for collisions*/
  bool checkCollision(sf::RectangleShape other_body);
  /**Returns collider*/
  Collider getCollider() { return Collider(body); }

  sf::RectangleShape body;
  int type_;
};

#endif
