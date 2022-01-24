#include "HUD.hpp"

#include <sstream>
#include <string>

HUD::HUD(sf::Font font, std::vector<sf::Texture*> hp_textures,
         sf::Texture* background, sf::Texture* inventory_texture,
         std::vector<sf::Texture*> item_textures)
    : font_(font), hp_textures_(hp_textures) {
  // Window size 500x500 assumed
  background_.setSize(sf::Vector2f(500.0f, 150.0f));
  background_.setPosition(sf::Vector2f(0.0f, 500.0f));
  background_.setTexture(background);
  for (unsigned int i = 0; i < 3; i++) {
    sf::RectangleShape heart;
    heart.setSize(sf::Vector2f(50.0f, 50.0f));
    heart.setPosition(sf::Vector2f((i + 1) * 50.0f, 520.0f));
    heart.setTexture(hp_textures_[0]);
    hp_bar_.push_back(heart);
  }
  for (unsigned int i = 0; i < 3; i++) {
    sf::Text text;
    text.setString("0");
    text.setCharacterSize(24);
    text.setPosition(sf::Vector2f((i + 5) * 50.0f, 570.0f));
    texts_.push_back(text);
  }
  for (unsigned int i = 4; i < 7; i++) {
    sf::RectangleShape inv_slot;
    inv_slot.setSize(sf::Vector2f(50.0f, 50.0f));
    inv_slot.setPosition(sf::Vector2f((i + 1) * 50.0f, 520.0f));
    inv_slot.setTexture(inventory_texture);
    inventory_bar_.push_back(inv_slot);
  }
  for (unsigned int i = 0; i < item_textures.size(); i++) {
    sf::RectangleShape item_image;
    item_image.setSize(sf::Vector2f(50.0f, 50.0f));
    item_image.setPosition(sf::Vector2f((i + 5) * 50.0f, 520.0f));
    item_image.setTexture(item_textures[i]);
    hud_items_.push_back(item_image);
  }
}

void HUD::Display(sf::RenderWindow& window) {
  window.draw(background_);
  for (unsigned int i = 0; i < hp_bar_.size(); i++) {
    window.draw(hp_bar_[i]);
  }
  for (unsigned int j = 0; j < inventory_bar_.size(); j++) {
    window.draw(inventory_bar_[j]);
    texts_[j].setFont(font_);
    window.draw(texts_[j]);
  }
  if (hud_items_.size() != 0) {
    for (unsigned int i = 0; i < hud_items_.size(); i++) {
      window.draw(hud_items_[i]);
    }
  }
}

void HUD::Update(std::vector<Item>& items, float playerHP) {
  int hp_pots = 0;
  int spd_pots = 0;
  int coins = 0;
  for (auto i : items) {
    if (i.type == "hp_pot") {
      hp_pots++;
    }
    if (i.type == "speed_pot") {
      spd_pots++;
    }
    if (i.type == "coin") {
      coins++;
    }
  }
  std::stringstream ss;
  ss << hp_pots;
  texts_[0].setString(ss.str());
  std::stringstream ss2;
  ss2 << spd_pots;
  texts_[1].setString(ss2.str());
  std::stringstream ss3;
  ss3 << coins;
  texts_[2].setString(ss3.str());

  int hp = static_cast<int>(playerHP);
  switch (hp) {
    case 6:
      hp_bar_[hp_bar_.size() - 1].setTexture(hp_textures_[0]);
      hp_bar_[hp_bar_.size() - 2].setTexture(hp_textures_[0]);
      hp_bar_[hp_bar_.size() - 3].setTexture(hp_textures_[0]);
      break;
    case 5:
      hp_bar_[hp_bar_.size() - 1].setTexture(hp_textures_[1]);
      hp_bar_[hp_bar_.size() - 2].setTexture(hp_textures_[0]);
      hp_bar_[hp_bar_.size() - 3].setTexture(hp_textures_[0]);
      break;
    case 4:
      hp_bar_[hp_bar_.size() - 1].setTexture(hp_textures_[2]);
      hp_bar_[hp_bar_.size() - 2].setTexture(hp_textures_[0]);
      hp_bar_[hp_bar_.size() - 3].setTexture(hp_textures_[0]);
      break;
    case 3:
      hp_bar_[hp_bar_.size() - 1].setTexture(hp_textures_[2]);
      hp_bar_[hp_bar_.size() - 2].setTexture(hp_textures_[1]);
      hp_bar_[hp_bar_.size() - 3].setTexture(hp_textures_[0]);
      break;
    case 2:
      hp_bar_[hp_bar_.size() - 1].setTexture(hp_textures_[2]);
      hp_bar_[hp_bar_.size() - 2].setTexture(hp_textures_[2]);
      hp_bar_[hp_bar_.size() - 3].setTexture(hp_textures_[0]);
      break;
    case 1:
      hp_bar_[hp_bar_.size() - 1].setTexture(hp_textures_[2]);
      hp_bar_[hp_bar_.size() - 2].setTexture(hp_textures_[2]);
      hp_bar_[hp_bar_.size() - 3].setTexture(hp_textures_[1]);
      break;
    case 0:
      hp_bar_[hp_bar_.size() - 1].setTexture(hp_textures_[2]);
      hp_bar_[hp_bar_.size() - 2].setTexture(hp_textures_[2]);
      hp_bar_[hp_bar_.size() - 3].setTexture(hp_textures_[2]);
      break;
  }
}

HUD::~HUD() {}
