#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <string>

#include "collider.hpp"
#ifndef ITEM_CLASS
#define ITEM_CLASS

/**Class for handling items*/
class Item {
 public:
  /**Constructor for Item oject*/
  Item(sf::Texture* texture, sf::Vector2f position, std::string type);
  /**Default destructor for item*/
  ~Item(){};
  /**Used for rendering the items*/
  void Draw(sf::RenderWindow& window);
  /**Returns the item collider*/
  Collider getCollider() { return Collider(body); }
  sf::RectangleShape body;
  std::string type;
};
#endif
