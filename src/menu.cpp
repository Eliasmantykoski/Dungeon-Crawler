#include "menu.hpp"

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Mouse.hpp>
#include <iostream>

Menu::Menu(sf::Font font, sf::Texture* menubackground)
    : font_(font), active_(true) {
  background_.setSize(sf::Vector2f(500, 650));
  background_.setPosition(sf::Vector2f(0, 0));
  background_.setTexture(menubackground);

  sf::Text VarText;
  VarText.setString("Dungeon crawler Group 15");
  VarText.setCharacterSize(36);
  VarText.setPosition(35, 200);
  VarText.setFillColor(sf::Color::Red);
  VarText.setFont(font_);
  texts_.push_back(VarText);

  sf::Text StartText;
  StartText.setString("Start New Game");
  StartText.setCharacterSize(36);
  StartText.setPosition(50, 300);
  StartText.setFillColor(sf::Color::Yellow);
  StartText.setFont(font_);
  texts_.push_back(StartText);
  sf::Text QuitText;
  QuitText.setString("Quit Game");
  QuitText.setCharacterSize(36);
  QuitText.setPosition(50, 350);
  QuitText.setFillColor(sf::Color::White);
  QuitText.setFont(font_);
  texts_.push_back(QuitText);
  }
void Menu::Display(sf::RenderWindow& window) {
  if (active_) {
    this->Update(window);
    window.draw(background_);
    for (unsigned int i = 0; i < texts_.size(); i++) {
              window.draw(texts_[i]);
    }
  }
}
Menu::~Menu(){};

void Menu::Update(sf::RenderWindow& relativeTo) {
  if (sf::Mouse::isButtonPressed(sf::Mouse::Left) and active_) {
    sf::Vector2i pos = sf::Mouse::getPosition(relativeTo);
    if (pos.y > 30 && pos.y < (300 + 36)) {
      active_ = false;
    }
    if (pos.y > (350) && pos.y < (350 + 36)) {
      relativeTo.close();
    }
  }
}
