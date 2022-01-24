#include "player.hpp"

#include <SFML/Audio/Sound.hpp>
#include <SFML/System/Vector2.hpp>
#include <iostream>

Player::Player(sf::Texture* texture, sf::Texture* dead_texture,
               sf::Vector2f spawnPos, float speed, Weapon* starting_weapon)
    : speed_(speed) {
  death_texture_ = dead_texture;
  player_weapons_.push_back(*starting_weapon);
  body_.setSize(sf::Vector2f(40.0f, 40.0f));
  body_.setOrigin(body_.getSize() / 2.0f);
  body_.setPosition(spawnPos);
  body_.setTexture(texture);
};

Player::~Player(){};

void Player::Draw(sf::RenderWindow& window) { window.draw(body_); };

sf::Vector2f Player::getPosition() { return body_.getPosition(); };

bool Player::isAlive() { return alive_; };

float Player::getHp() { return hp_; }

float Player::getShield() { return shield_; }

bool Player::useItem() {
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1)) {
    auto i = items_.begin();
    while (i != items_.end()) {
      if (i->type == "hp_pot" && itemTimer > 0.5) {
        if (hp_ < 6) {
          hp_ += 1;
          i = items_.erase(i);
          itemTimer = 0;
          return true;
        } else {
          return false;
        }
      } else {
        ++i;
      }
    }
  }

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2)) {
    auto i = items_.begin();
    while (i != items_.end()) {
      if (i->type == "speed_pot" && itemTimer > 0.5) {
        speed_ += 10;
        speedTimer = 15;
        i = items_.erase(i);
        itemTimer = 0;
        return true;
      } else {
        ++i;
      }
    }
  }
  return false;
}

void Player::Damage(float damage) {
  if (shield_ > 0) {
    shield_ -= damage;
    if (shield_ < 0) shield_ = 0;
  } else
    hp_ -= damage;
  if (hp_ <= 0) {
    std::cout << "The player has died!" << std::endl;
    alive_ = false;
  }
}

std::vector<int> Player::Update(float time, Map& map,
                                std::vector<Projectile>& active_projectiles) {
  std::vector<int> ret = {0, 0, 0, 0};
  if (alive_ == false) {
    if (dead_ == false) {
      sf::Vector2u texture_size = death_texture_->getSize();
      sf::Vector2f texture_size_f(float(texture_size.x), float(texture_size.y));
      body_.setSize(texture_size_f);
      body_.setOrigin(body_.getSize() / 2.0f);
      sf::Vector2f temp_position = body_.getPosition();
      std::cout << "Death position: " << temp_position.x << " "
                << temp_position.y << std::endl;
      body_.setPosition(temp_position);
      body_.setTexture(death_texture_);
      dead_ = true;
    }
    // return active_projectiles;
  }
  if (this->useItem()) {
    ret[2] = 1;
  }
  currentSpeed_ = speed_;
  actionTimer_ += time;
  shot_ += time;  // cooldown for firing weapon
  itemTimer += time;
  sf::Vector2f movement(0.0f, 0.0f);
  float firerate = player_weapons_[chosen_weapon_].fire_rate_;

  if (speedTimer > 0) {
    speedTimer -= time;
  } else if (speedTimer < 0) {
    speedTimer = 0;
    speed_ -= 10;
  }

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
    movement.y -= currentSpeed_ * time;
    direction_ = movement;
  }

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
    movement.x -= currentSpeed_ * time;
    direction_ = movement;
  }

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
    movement.y += currentSpeed_ * time;
    direction_ = movement;
  }

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
    movement.x += currentSpeed_ * time;
    direction_ = movement;
  }

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) &&
      shot_ >= firerate) {  // Up arrow key is used to shoot projectiles.
    sf::Vector2f direction = sf::Vector2f(0.0f, -10.0f);
    Projectile new_projectile = this->UseWeapon(direction);
    active_projectiles.push_back(new_projectile);
    shot_ = 0.0;
    ret[0] = 1;
  }
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) &&
      shot_ >= firerate) {  // Down arrow key is used to shoot projectiles.
    sf::Vector2f direction = sf::Vector2f(0.0f, 10.0f);
    Projectile new_projectile = this->UseWeapon(direction);
    active_projectiles.push_back(new_projectile);
    shot_ = 0.0;
    ret[0] = 1;
  }
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) &&
      shot_ >= firerate) {  // Left arrow key is used to shoot projectiles.
    sf::Vector2f direction = sf::Vector2f(-10.0f, 0.0f);
    Projectile new_projectile = this->UseWeapon(direction);
    active_projectiles.push_back(new_projectile);
    shot_ = 0.0;
    ret[0] = 1;
  }
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) &&
      shot_ >= firerate) {  // Right arrow key is used to shoot projectiles.
    sf::Vector2f direction = sf::Vector2f(10.0f, 0.0f);
    Projectile new_projectile = this->UseWeapon(direction);
    active_projectiles.push_back(new_projectile);
    shot_ = 0.0;
    ret[0] = 1;
  }

  Traverse(map);
  body_.move(movement);

  // check that player is not colliding with any of the enemies and take damage
  // if colliding
  damaged_ += time;
  Collider collider = this->getCollider();
  for (auto& enemy : map.enemies) {
    Collider EnemyCollider = enemy->getCollider();
    if (collider.checkCollider(EnemyCollider, 0.5) && damaged_ >= 1.5) {
      this->Damage(enemy->damage_);
      damaged_ = 0.0;
      ret[1] = 1;
    }
  }
  for (unsigned int i = 0; i < map.items.size(); i++) {
    Collider itemCollider = map.items[i].getCollider();
    if (collider.checkCollider(itemCollider, 0)) {
      items_.push_back(map.items[i]);
      map.items.erase(map.items.begin() + i);
      ret[3] = 1;
    }
  }
  return ret;
};

Projectile Player::UseWeapon(sf::Vector2f& direction) {
  return player_weapons_[chosen_weapon_].Fire(body_.getPosition(), direction);
};

void Player::Traverse(Map& map) {
  sf::Vector2f pos = body_.getPosition();

  if (pos.x < 25) {
    map.NextRoom(1);
    body_.move(sf::Vector2f(500, 0));
  }
  if (pos.x > 25 + 10 * 50) {
    map.NextRoom(3);
    body_.move(sf::Vector2f(-500, 0));
  }
  if (pos.y < 25) {
    map.NextRoom(2);
    body_.move(sf::Vector2f(0, 500));
  }
  if (pos.y > 25 + 10 * 50) {
    map.NextRoom(0);
    body_.move(sf::Vector2f(0, -500));
  }
}
