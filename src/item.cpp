#include "item.hpp"

#include <SFML/System/Vector2.hpp>
Item::Item(sf::Texture *texture, sf::Vector2f position, std::string type)
    : type(type) {
  body.setSize(sf::Vector2f(30, 30));
  body.setPosition(position);
  body.setTexture(texture);
  body.setOrigin(sf::Vector2f(30, 30) / 2.0f);
};
void Item::Draw(sf::RenderWindow &window) { window.draw(body); }
