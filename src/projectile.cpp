#include "projectile.hpp"
#include "player.hpp"

//#include <iostream>

/*
* Constructor of projectile, texture is the texture of the projectile.
*/
Projectile::Projectile(sf::Texture* texture) {
  //friendly_ = true;
  //friendly_.push_back(1);
  projectile_texture_ = texture;
};

//Projectile::Projectile(sf::Texture* texture, bool friendly)
//: friendly_(friendly), projectile_texture_(texture) {};

Projectile::~Projectile() {};

/*
* Method for drawing the projectile
*/
void Projectile::Draw(sf::RenderWindow& window) { window.draw(body_); };

/*
* Method for updating the projectile location; moves the projectile towards its trajectory
* Checks if projectile hits wall or enemy
*/
bool Projectile::Update(float time, Map& map, Player& player) {
  actionTimer_ += time;
  if(actionTimer_ > 1.0) {
    return 1;
  }
  body_.move(projectile_trajectory_);
  Collider col = this->getCollider();
  //std::cout << friendly_;
  //if(friendly_ == 1) {

    //secret_size is used for determining who the projectile harms, because otherwise there will be memory issues.
  sf::Vector2u secret_size = this->projectile_texture_->getSize();
  if(secret_size.x < 500) { //Enemy projectile textures happen to be, by coincidence, larger than player projectile textures
    for (auto& enemy : map.enemies) {
    Collider enemyCollider = enemy->getCollider();
    if (col.checkCollider(enemyCollider, 0)) {
        enemy->Damage(15);
        return 1;
    }
    }
  } else {
    Collider playerCollider = player.getCollider();
    if (col.checkCollider(playerCollider, 0)) {
        player.Damage(2);
        return 1;
    }
  }
  for (auto room : map.rooms) {
    for (auto wall : room.walls) {
      for (auto w : wall) {
        if (w.type_ == 1) {
          Collider wallCollider = w.getCollider();
          if (wallCollider.checkCollider(col, 1)) {
            return 1;
          }
        }
      }
    }
  }
  return 0;
  //std::cout << projectile_trajectory_.x << "\t" << projectile_trajectory_.y << "\n";
  //std::cout << body_.getPosition().x << "\t" << body_.getPosition().y << "\n";
};

/*
* Method for activating and deactivating the projectile. In main only active projectiles are drawn and updated on the SFML window.
*/
void Projectile::activate() {active_ = true;};
void Projectile::deActivate() {active_ = false;};
