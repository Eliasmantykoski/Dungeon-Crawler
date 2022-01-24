#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>
#include <vector>

#include "item.hpp"

#ifndef HUD_CLASS
#define HUD_CLASS

class HUD {
 public:
  /**Constructs HUD*/
  HUD(sf::Font font, std::vector<sf::Texture*> hp, sf::Texture* background,
      sf::Texture* inventory, std::vector<sf::Texture*> item_textures);
  /**Default destructor*/
  ~HUD();
  /**Rendrers the HUD*/
  void Display(sf::RenderWindow& window);
  /**Updates the HUD*/
  void Update(std::vector<Item>&, float);
  sf::RectangleShape background_;
  std::vector<sf::RectangleShape> hp_bar_;
  std::vector<sf::RectangleShape> inventory_bar_;
  std::vector<sf::RectangleShape> hud_items_;
  std::vector<sf::Text> texts_;
  sf::Font font_;
  std::vector<sf::Texture*> hp_textures_;
};
#endif
