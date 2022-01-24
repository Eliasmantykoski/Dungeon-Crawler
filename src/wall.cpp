#include "wall.hpp"

#include <SFML/System/Vector2.hpp>

Wall::Wall(sf::Texture *texture, sf::Vector2f size, sf::Vector2f position,
           int type) {
  body.setSize(size);
  body.setPosition(position);
  body.setTexture(texture);
  body.setOrigin(size / 2.0f);
  type_ = type;
};
void Wall::Draw(sf::RenderWindow &window) { window.draw(body); };

void Wall::Move2(sf::Vector2f dir) { body.move(dir); }

bool Wall::checkCollision(sf::RectangleShape other_body) {
  return body.getGlobalBounds().intersects(other_body.getGlobalBounds());
}
