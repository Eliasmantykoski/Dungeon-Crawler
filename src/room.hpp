#ifndef ROOM_CLASS
#define ROOM_CLASS

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>
#include <vector>

#include "collider.hpp"
#include "enemy.hpp"
#include "item.hpp"
#include "wall.hpp"
class Room {
 public:
  /*
   *Constructor for Room. Populates room edges with texture and floor with
   *nullptr walls
   */
  Room(sf::Texture* texture, sf::Vector2f spawnPos, std::vector<int> openings,
       int depth);
  /*
   *Method for displaying individual walls
   */
  void Display(sf::RenderWindow& window, Collider playerCollider,
               std::vector<Enemy*>& enemies);
  /**Checks collisions with walls*/
  void checkCollision(sf::RectangleShape& other_body);
  /**Moves the room when player traverses*/
  void MoveRoom(int direction, std::vector<Item>& items);
  /**Default destructor*/
  ~Room(){};
  void Activate();
  void Deactivate();
  bool active;
  sf::Vector2f maxSize = sf::Vector2f(10.0f, 10.0f);
  float xBound1;
  float xBound2;
  float yBound1;
  float yBound2;
  std::vector<std::vector<Wall>> walls;
  int depth_;
};

#endif
