#include "collider.hpp"

Collider::Collider(sf::RectangleShape& body) : body_(body) {}

Collider::~Collider() {}

// checks of collider intersects witch collider given as parameter
// moves one if necessary
bool Collider::checkCollider(Collider& other, float push) {
  // get positions
  sf::Vector2f otherPosition = other.getPosition();
  sf::Vector2f otherHalfSize = other.getHalfSize();
  sf::Vector2f thisPosition = getPosition();
  sf::Vector2f thisHalfSize = getHalfSize();

  // find differences in location
  float xdif = otherPosition.x - thisPosition.x;
  float ydif = otherPosition.y - thisPosition.y;

  float intersectX = abs(xdif) - (otherHalfSize.x + thisHalfSize.x);
  float intersectY = abs(ydif) - (otherHalfSize.y + thisHalfSize.y);

  // if colliders intersect, move one of them based on push parameter and return
  // true
  if (intersectX < 0.0f && intersectY < 0.0f) {
    push = std::min(std::max(push, 0.0f), 1.0f);

    if (abs(intersectX) < abs(intersectY)) {
      if (xdif > 0.0f) {
        Move(intersectX * (1.0f - push), 0.0f);
        other.Move(-intersectX * push, 0.0f);
      } else {
        Move(-intersectX * (1.0f - push), 0.0f);
        other.Move(intersectX * push, 0.0f);
      }
    } else {
      if (ydif > 0.0f) {
        Move(0.0f, intersectY * (1.0f - push));
        other.Move(0.0f, -intersectY * push);
      } else {
        Move(0.0f, -intersectY * (1.0f - push));
        other.Move(0.0f, intersectY * push);
      }
    }

    return true;
  }

  return false;
}
