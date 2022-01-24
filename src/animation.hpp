#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>
#include <string>

#ifndef ANIMATION_CLASS
#define ANIMATION_CLASS

class Animation {
 public:
  /**Animation constructor*/
  Animation(const std::string sprite_sheets_path, int frame_x_pos, int frame_y_pos);
  /**Default destructor*/
  ~Animation();
  /**Sets the animation speed to new_speed value*/
  void setAnimationSpeed(float new_speed);
  /**Returns current frame of the animation image as texture*/
  sf::Texture* getCurrentFrame();
  /**Resets the current frame*/
  void resetCurrentFrame();
  /**Updates the frame*/
  bool Update(float time);

  std::vector<sf::Texture> animation_frames_;
  unsigned int current_frame_ = 0;
  unsigned int last_frame_;
  float animation_speed_ = 0.2; //Animation speed in frames per second
  float actionTimer_;
};

#endif
