#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>
#include <vector>

// include "item.hpp"

#ifndef MENU_CLASS
#define MENU_CLASS
/**Class for main menu*/
class Menu {
 public:
  /**Constructor for menu*/
  Menu(sf::Font font, sf::Texture* menubackground);
  /**Default destructor*/
  ~Menu();
  /**Renders the main menu*/
  void Display(sf::RenderWindow& window);
  /**Polls for actions in main menu*/
  void Update(sf::RenderWindow& window);
  sf::RectangleShape background_;
  sf::Font font_;
  std::vector<sf::Text> texts_;
  bool active_;
};
#endif
