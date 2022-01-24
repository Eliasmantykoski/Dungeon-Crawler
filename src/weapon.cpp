#include "weapon.hpp"

/*
* Constructor of weapon with textures for weapon displayed on ground/inventory and projectile texture that
* is shown on the projectile when the weapon is fired.
*/
Weapon::Weapon(sf::Texture* texture, sf::Texture* proj_texture) {
  weapon_body_.setTexture(texture);
  Projectile new_projectile = Projectile(proj_texture);
  weapon_projectile_.push_back(new_projectile);
};

Weapon::~Weapon() {};

/*
* Method for firing a single projectile that corresponds to the weapon.
*
* As the first parameter this method receives the position from where the weapon is being fired at, 
* the projectile will be located at this location initially.
*
* As the second parameter this method receives the trajectory for projectile,
* it describes the speed and direction of the projectile.
*/
Projectile Weapon::Fire(sf::Vector2f fire_position, sf::Vector2f fire_trajectory) {
  Projectile proj(weapon_projectile_[0].projectile_texture_);
  proj.active_ = true;
  proj.body_.setTexture(proj.projectile_texture_);
  proj.body_.setOrigin(proj.body_.getSize() / 2.0f);
  proj.body_.setPosition(fire_position);
  proj.body_.setSize(sf::Vector2f(25.0f, 25.0f));
  proj.projectile_trajectory_ = fire_trajectory/* * proj.velocity_multiplier_*/;
  proj.body_.move(proj.projectile_trajectory_);
  return proj;
};
