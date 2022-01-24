#include "room.hpp"

#include <math.h>

#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/System/Vector2.hpp>
#include <cmath>
#include <iostream>
#include <ostream>

Room::Room(sf::Texture *texture, sf::Vector2f spawnPos,
           std::vector<int> openings, int depth)
    : active(false), depth_(depth) {
  xBound1 = spawnPos.x;
  xBound2 = spawnPos.x + maxSize.x * 50.0f;

  yBound1 = spawnPos.y;
  yBound2 = spawnPos.y + maxSize.y * 50.0f;

  sf::Texture *floortexture = new sf::Texture();
  floortexture->loadFromFile("libs/images/floor_1.png");

  for (int x = 0; x < maxSize.x; x++) {
    walls.push_back(std::vector<Wall>());

    for (int y = 0; y < maxSize.y; y++) {
      if (y == floor(maxSize.y / 2) &&
          ((x == 0 && openings[3] == 1) ||
           (x == maxSize.x - 1 && openings[1] == 1))) {
        Wall wall(floortexture, sf::Vector2f(50.0f, 50.f),
                  spawnPos + sf::Vector2f(50.0f * x, 50.0f * y), 2);
        walls[x].push_back(wall);
      } else if (x == floor(maxSize.x / 2) &&
                 ((y == 0 && openings[0] == 1) ||
                  (y == maxSize.y - 1 && openings[2] == 1))) {
        Wall wall(floortexture, sf::Vector2f(50.0f, 50.f),
                  spawnPos + sf::Vector2f(50.0f * x, 50.0f * y), 2);
        walls[x].push_back(wall);
      }

      else if (x == 0 || x == maxSize.x - 1 || y == 0 || y == maxSize.y - 1) {
        Wall wall(texture, sf::Vector2f(50.0f, 50.f),
                  spawnPos + sf::Vector2f(50.0f * x, 50.0f * y), 1);
        walls[x].push_back(wall);

      } else {
        Wall wall(floortexture, sf::Vector2f(50.0f, 50.f),
                  spawnPos + sf::Vector2f(50.0f * x, 50.0f * y), 0);
        walls[x].push_back(wall);
      }
    }
  }
};

/*
 *Method for displaying individual walls
 *Checks collider with player and enemies
 */
void Room::Display(sf::RenderWindow &window, Collider playerCollider,
                   std::vector<Enemy *> &enemies) {
  for (auto r : walls) {
    for (auto w : r) {
      w.Draw(window);
      Collider wallCollider = w.getCollider();
      if (w.type_ == 1) {
        wallCollider.checkCollider(playerCollider, 1);

        for (auto i = 0; i < enemies.size(); i++) {
          Collider enemyCollider = enemies[i]->getCollider();
          wallCollider.checkCollider(enemyCollider, 1);
        }
      }
      if (w.type_ == 2) {
        for (auto i = 0; i < enemies.size(); i++) {
          Collider enemyCollider = enemies[i]->getCollider();
          wallCollider.checkCollider(enemyCollider, 1);
        }
      }
    }
  }
};

// void Room::checkCollision(sf::RectangleShape &other_body) {
//   for (unsigned int i = 0; i < walls.size(); i++) {
//     for (unsigned int j = 0; j < walls[i].size(); j++) {
//       if (walls[i][j].checkCollision(other_body) &&
//           walls[i][j].body.getTexture() != nullptr) {
//         float x = other_body.getPosition().x -
//         walls[i][j].body.getPosition().x; float y =
//         other_body.getPosition().y - walls[i][j].body.getPosition().y;
//         other_body.move(sf::Vector2f(0.05 * x, 0.05 * y));
//       }
//     }
//   }
// }

void Room::MoveRoom(int direction, std::vector<Item> &items) {
  sf::Vector2f dir;
  if (direction == 0) {
    dir = sf::Vector2f(0, -10 * 50);
  }
  if (direction == 1) {
    dir = sf::Vector2f(10 * 50, 0);
  }
  if (direction == 2) {
    dir = sf::Vector2f(0, 10 * 50);
  }
  if (direction == 3) {
    dir = sf::Vector2f(-10 * 50, 0);
  }

  for (auto i = 0; i < walls.size(); i++) {
    for (auto j = 0; j < walls[i].size(); j++) {
      walls[i][j].Move2(dir);
    }
  }
  for (auto i = 0; i < items.size(); i++) {
    items[i].body.move(dir);
  }
}

void Room::Activate() { active = true; };
void Room::Deactivate() { active = false; }
