#include "animation.hpp"

/*
The constructor of the animation class.

Parameters:
    sprite_sheets_path: the path for the sprite sheeth that contains the frames for the animation,
    for example: "libs/images/pumpkin_dude.png"

    texture_crop_scale_x_: Integer describing how many pixels in the x direction is one frame in the sprite sheet cropped.

    texture_crop_scale_y_: Integer describing how many pixels in the y direction is one frame in the sprite sheet cropped.

How the constructor works:
    This constructor creates the frames using the sprite sheets, and then pushes the frames to vector animation_frames_.
    This class uses animation_frames_ to animate an object frame by frame, or to "step" one frame forward repeatedly.
*/
Animation::Animation(const std::string sprite_sheets_path, int texture_crop_scale_x_, int texture_crop_scale_y_) {
  sf::Texture temp_texture;
  temp_texture.loadFromFile(sprite_sheets_path);
  sf::Vector2u texture_size = temp_texture.getSize();
  int amount_of_x_offsets = (int(texture_size.x) / texture_crop_scale_x_);
  int amount_of_y_offsets = (int(texture_size.y) / texture_crop_scale_y_);
  int x_offset = 0; int y_offset = 0;
  for (int j = 0; j < amount_of_y_offsets; j++) {
    y_offset = j*texture_crop_scale_y_;
    for (int i = 0; i < amount_of_x_offsets; i++) {
      x_offset = i*texture_crop_scale_x_;
      sf::IntRect cropping(x_offset, y_offset, texture_crop_scale_x_, texture_crop_scale_y_);
      sf::Texture frame_texture;
      frame_texture.loadFromFile(sprite_sheets_path, cropping);
      animation_frames_.push_back(frame_texture);
    }
  }
  last_frame_ = animation_frames_.size() - 1;
};

/* Destructor. */
Animation::~Animation() {};

/* Sets the animation speed. */
void Animation::setAnimationSpeed(float new_speed) {animation_speed_ = new_speed;};

/*
This function is called to get the current frame that the animation is meant to display.
Call example:
body_.setTexture(animation01.getCurrentFrame());
*/
sf::Texture* Animation::getCurrentFrame() {return &animation_frames_[current_frame_];};

/*
Simply resets the animation to start from the beggining,
useful for ex. when character stops walking to stand still instead.
*/
void Animation::resetCurrentFrame() {current_frame_ = 0;};

/*
Updates the animation with respect to SFML window time in seconds.
Uses time to determine wether or not the next frame in the animation is going to be displayed.
Returns true if frame was updated, returns false otherwise.
*/
bool Animation::Update(float time) {
  actionTimer_ += time;
  if(actionTimer_ >= animation_speed_){
    actionTimer_ = 0;
    current_frame_ += 1;
    if(current_frame_ > last_frame_){
      current_frame_ = 0;
    }
    return true;
  }
  return false;
};
