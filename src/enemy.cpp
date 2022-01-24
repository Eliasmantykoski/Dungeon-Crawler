#include "enemy.hpp"
#include "projectile.hpp"

#include <stdlib.h> /* srand, rand, RAND_MAX */
#include <time.h>   /* time */
#include <math.h>   /* sqrt, pow */

#include <iostream>

Enemy::Enemy(Animation *enemy_animation, sf::Vector2f spawnPos, float speed) 
  : speed_(speed)  {
  body_.setSize(sf::Vector2f(40.0f, 40.0f));
  body_.setOrigin(body_.getSize() / 2.0f);
  body_.setPosition(spawnPos);
  enemy_animations_.push_back(*enemy_animation);
  body_.setTexture(&(enemy_animation->animation_frames_[0]));
};

Enemy::~Enemy() {};

sf::Vector2f Enemy::getPosition() { return body_.getPosition(); };

bool Enemy::isRanged() { return is_ranged_type_; };

void Enemy::Draw(sf::RenderWindow& window) { window.draw(body_); };

void Enemy::Damage(float damage) {
  hp_ -= damage;
  if (hp_ <= 0) {alive_ = false;} //std::cout << "The NPC has died!" << std::endl;
}

void Enemy::Update2(float time, sf::Vector2f player_position, std::vector<Projectile>& active_projectiles) {};

void Enemy::MoveRoom(int direction) {
  sf::Vector2f dir;
  if (direction == 0) {
    dir = sf::Vector2f(0, -10 * 50);
  }
  if (direction == 1) {
    dir = sf::Vector2f(10 * 50, 0);
  }
  if (direction == 2) {
    dir = sf::Vector2f(0, 10 * 50);
  }
  if (direction == 3) {
    dir = sf::Vector2f(-10 * 50, 0);
  }
  body_.move(dir);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ChasingEnemy::ChasingEnemy(Animation *enemy_animation, sf::Vector2f spawnPos,
                           float speed)
    : Enemy(enemy_animation, spawnPos, speed){};

ChasingEnemy::~ChasingEnemy() {};

void ChasingEnemy::Update(float time, sf::Vector2f player_position) {
  currentSpeed_ = speed_;
  actionTimer_ += time;
  //std::cout << "Interval timer: " << actionTimer_ << "\n";

  /* Change walk direction to some random direction at random intervals */

  if(actionTimer_ >= walkInterval_){
    actionTimer_ = 0.0f;
    walk_ = walk_ ? false : true; //tenary operator

    /*
    walkInterval_ is a float between 0.5 and 2.5,
    this is how it is generated:

    rand() gives a number between 0 and RAND_MAX,
    so divide by RAND_MAX to get a float between 0 and 1,
    or in this case 0 and 2, because it is multiplied by 2,
    and then plus 0.5 of that to get float between 0.5 and 2.5.
    */
    walkInterval_ = ((float(rand()) / float(RAND_MAX)) * 2) + 0.5f;

    if(walk_ == false){
      enemy_animations_[0].resetCurrentFrame();
      body_.setTexture(enemy_animations_[0].getCurrentFrame());
    } else{
      attacking_player_ = (rand() % 3) == 1; //Enemies randomly attack sometimes
      walkInterval_ = 0.7;
    }

    sf::Vector2f movement(currentSpeed_, currentSpeed_);
    if(attacking_player_ == true) {
      sf::Vector2f direction_vector = player_position - body_.getPosition();
      float vector_magnitude = sqrt(pow(direction_vector.x, 2.0) + pow(direction_vector.y, 2.0));
      direction_vector.x = direction_vector.x / vector_magnitude;
      direction_vector.y = direction_vector.y / vector_magnitude;
      //std::cout << direction_vector.x << " " << direction_vector.y << " " << vector_magnitude << "\n";
      movement.x *= time * direction_vector.x * 2.f;
      movement.y *= time * direction_vector.y * 2.f;
    } else {
      float random_float_x = ((float(rand()) / float(RAND_MAX)) * 2) - 1.0f;
      float random_float_y = ((float(rand()) / float(RAND_MAX)) * 2) - 1.0f;
      movement.x *= random_float_x * time;
      movement.y *= random_float_y * time;
    }
    direction_ = movement;
  }
  if(walk_){
    if(enemy_animations_[0].Update(time)){
      body_.setTexture(enemy_animations_[0].getCurrentFrame());
    }
    body_.move(direction_);
    /*Collider col = this->getCollider();
    for (auto room : rooms) {
      for (auto wall : room.walls) {
        for (auto w : wall) {
          if (w.type_ == 1) {
            Collider wallCol = w.getCollider();
            wallCol.checkCollider(col, 1);
          }
        }
      }
    }*/
  }
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

RangedEnemy::RangedEnemy(Animation *enemy_animation, sf::Vector2f spawnPos, float speed,
sf::Texture* enemy_projectile_texture)
 : Enemy(enemy_animation, spawnPos, speed), enemy_projectile_texture_(enemy_projectile_texture)/*, is_ranged_type_(true)*/ {
     is_ranged_type_ = true;
     //enemy_projectile_ = std::make_unique<Projectile>(enemy_projectile_texture);
 };

RangedEnemy::~RangedEnemy() {};

void RangedEnemy::Update(float time, sf::Vector2f player_position) {
  currentSpeed_ = speed_;
  actionTimer_ += time;

  /* Change walk direction to some random direction at random intervals */

  if(actionTimer_ >= walkInterval_){
    actionTimer_ = 0.0f;
    walk_ = walk_ ? false : true; //tenary operator

    /*
    walkInterval_ is a float between 0.5 and 2.5,
    this is how it is generated:

    rand() gives a number between 0 and RAND_MAX,
    so divide by RAND_MAX to get a float between 0 and 1,
    or in this case 0 and 2, because it is multiplied by 2,
    and then plus 0.5 of that to get float between 0.5 and 2.5.
    */
    walkInterval_ = ((float(rand()) / float(RAND_MAX)) * 2) + 0.5f;

    if(walk_ == false){
      enemy_animations_[0].resetCurrentFrame();
      body_.setTexture(enemy_animations_[0].getCurrentFrame());
    }

    sf::Vector2f movement(currentSpeed_, currentSpeed_);
    float random_float_x = ((float(rand()) / float(RAND_MAX)) * 2) - 1.0f;
    float random_float_y = ((float(rand()) / float(RAND_MAX)) * 2) - 1.0f;
    movement.x *= random_float_x * time;
    movement.y *= random_float_y * time;
    direction_ = movement;
  }

  if(walk_){
    if(enemy_animations_[0].Update(time)){
      body_.setTexture(enemy_animations_[0].getCurrentFrame());
    }
    body_.move(direction_);
    /*Collider col = this->getCollider();
    for (auto room : rooms) {
      for (auto wall : room.walls) {
        for (auto w : wall) {
          if (w.type_ == 1) {
            Collider wallCol = w.getCollider();
            wallCol.checkCollider(col, 1);
          }
        }
      }
    }*/
  }
};

void RangedEnemy::Update2(float time, sf::Vector2f player_position, std::vector<Projectile>& active_projectiles) {
  currentSpeed_ = speed_;
  actionTimer_ += time;

  /* Change walk direction to some random direction at random intervals */

  if(actionTimer_ >= walkInterval_){
    actionTimer_ = 0.0f;
    walk_ = walk_ ? false : true; //tenary operator

    /*
    walkInterval_ is a float between 0.5 and 2.5,
    this is how it is generated:

    rand() gives a number between 0 and RAND_MAX,
    so divide by RAND_MAX to get a float between 0 and 1,
    or in this case 0 and 2, because it is multiplied by 2,
    and then plus 0.5 of that to get float between 0.5 and 2.5.
    */
    walkInterval_ = ((float(rand()) / float(RAND_MAX)) * 2) + 0.5f;

    if(walk_ == false){
      enemy_animations_[0].resetCurrentFrame();
      body_.setTexture(enemy_animations_[0].getCurrentFrame());
    }

    sf::Vector2f movement(currentSpeed_, currentSpeed_);
    float random_float_x = ((float(rand()) / float(RAND_MAX)) * 2) - 1.0f;
    float random_float_y = ((float(rand()) / float(RAND_MAX)) * 2) - 1.0f;
    movement.x *= random_float_x * time;
    movement.y *= random_float_y * time;
    direction_ = movement;

    if(rand() % 2) { //Shoot somewhere: 50% chance
      sf::Vector2f shoot_direction(10, 10);
      if(rand() % 4 == 1) { //Shoot at random direction: 25% chance
        random_float_x = ((float(rand()) / float(RAND_MAX)) * 2) - 1.0f;
        random_float_y = ((float(rand()) / float(RAND_MAX)) * 2) - 1.0f;
        shoot_direction.x *= random_float_x;
        shoot_direction.y *= random_float_y;
      } else { //Shoot at direction of player: 75% chance
        sf::Vector2f direction_vector = player_position - body_.getPosition();
        float vector_magnitude = sqrt(pow(direction_vector.x, 2.0) + pow(direction_vector.y, 2.0));
        shoot_direction.x *= direction_vector.x / vector_magnitude;
        shoot_direction.y *= direction_vector.y / vector_magnitude;
      }
      Projectile new_projectile = this->ShootAtDirection(shoot_direction);
      active_projectiles.push_back(new_projectile);
    }
  }
  if(walk_){
    if(enemy_animations_[0].Update(time)){
      body_.setTexture(enemy_animations_[0].getCurrentFrame());
    }
    body_.move(direction_);
    /*Collider col = this->getCollider();
    for (auto room : rooms) {
      for (auto wall : room.walls) {
        for (auto w : wall) {
          if (w.type_ == 1) {
            Collider wallCol = w.getCollider();
            wallCol.checkCollider(col, 1);
          }
        }
      }
    }*/
  }
};

Projectile RangedEnemy::ShootAtDirection(sf::Vector2f direction) {
  //enemy_projectile_ = std::make_unique<Projectile>(enemy_projectile_texture);
  //Projectile Weapon::Fire(sf::Vector2f fire_position, sf::Vector2f fire_trajectory)
  Projectile proj(this->enemy_projectile_texture_);
  //proj.friendly_[0] = 0;
  proj.active_ = true;
  proj.body_.setTexture(proj.projectile_texture_);
  proj.body_.setOrigin(proj.body_.getSize() / 2.0f);
  proj.body_.setPosition(body_.getPosition());
  proj.body_.setSize(sf::Vector2f(25.0f, 25.0f));
  proj.projectile_trajectory_ = direction/* * proj.velocity_multiplier_*/;
  proj.body_.move(proj.projectile_trajectory_);
  return proj;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

FinalBoss::FinalBoss(Animation *enemy_animation, sf::Vector2f spawnPos,
                     float speed, sf::Texture* enemy_projectile_texture)
    : Enemy(enemy_animation, spawnPos, speed), enemy_projectile_texture_(enemy_projectile_texture) {
  boss = true;
  is_ranged_type_ = true;
  walk_ = true;
  body_.setSize(sf::Vector2f(100.0f, 100.0f));
  body_.setOrigin(body_.getSize() / 2.0f);
  hp_ = 5000;
};

void FinalBoss::Update(float time, sf::Vector2f player_position) {
  currentSpeed_ = speed_;
  actionTimer_ += time;
  //std::cout << "Interval timer: " << actionTimer_ << "\n";

  /* Change walk direction to some random direction at random intervals */

  if(actionTimer_ >= walkInterval_){
    actionTimer_ = 0.0f;

    /*
    walkInterval_ is a float between 0.5 and 2.5,
    this is how it is generated:

    rand() gives a number between 0 and RAND_MAX,
    so divide by RAND_MAX to get a float between 0 and 1,
    or in this case 0 and 2, because it is multiplied by 2,
    and then plus 0.5 of that to get float between 0.5 and 2.5.
    */
    walkInterval_ = ((float(rand()) / float(RAND_MAX)) * 2) + 0.5f;

    attacking_player_ = (rand() % 3) == 1; //Enemies randomly attack sometimes
    walkInterval_ = 0.7;

    sf::Vector2f movement(currentSpeed_, currentSpeed_);
    if(attacking_player_ == true) {
      sf::Vector2f direction_vector = player_position - body_.getPosition();
      float vector_magnitude = sqrt(pow(direction_vector.x, 2.0) + pow(direction_vector.y, 2.0));
      direction_vector.x = direction_vector.x / vector_magnitude;
      direction_vector.y = direction_vector.y / vector_magnitude;
      //std::cout << direction_vector.x << " " << direction_vector.y << " " << vector_magnitude << "\n";
      movement.x *= time * direction_vector.x * 2.f;
      movement.y *= time * direction_vector.y * 2.f;
    } else {
      float random_float_x = ((float(rand()) / float(RAND_MAX)) * 2) - 1.0f;
      float random_float_y = ((float(rand()) / float(RAND_MAX)) * 2) - 1.0f;
      movement.x *= random_float_x * time;
      movement.y *= random_float_y * time;
    }
    direction_ = movement;
  }
  if(enemy_animations_[0].Update(time)){
    body_.setTexture(enemy_animations_[0].getCurrentFrame());
  }
  body_.move(direction_);
    /*Collider col = this->getCollider();
    for (auto room : rooms) {
      for (auto wall : room.walls) {
        for (auto w : wall) {
          if (w.type_ == 1) {
            Collider wallCol = w.getCollider();
            wallCol.checkCollider(col, 1);
          }
        }
      }
    }*/
};

void FinalBoss::Update2(float time, sf::Vector2f player_position, std::vector<Projectile>& active_projectiles) {
  
  currentSpeed_ = speed_;
  actionTimer_ += time;

  /* Change walk direction to some random direction at random intervals */

  if(actionTimer_ >= walkInterval_) {
    actionTimer_ = 0.0f;
    /*
    walkInterval_ is a float between 0.5 and 2.5,
    this is how it is generated:

    rand() gives a number between 0 and RAND_MAX,
    so divide by RAND_MAX to get a float between 0 and 1,
    or in this case 0 and 2, because it is multiplied by 2,
    and then plus 0.5 of that to get float between 0.5 and 2.5.
    */
    walkInterval_ = ((float(rand()) / float(RAND_MAX)) * 2) + 0.5f;

    sf::Vector2f movement(currentSpeed_, currentSpeed_);
    float random_float_x = ((float(rand()) / float(RAND_MAX)) * 2) - 1.0f;
    float random_float_y = ((float(rand()) / float(RAND_MAX)) * 2) - 1.0f;
    movement.x *= random_float_x * time;
    movement.y *= random_float_y * time;
    direction_ = movement;

    if(rand() % 2) {
      sf::Vector2f shoot_direction1(10, 10);
      random_float_x = ((float(rand()) / float(RAND_MAX)) * 2) - 1.0f;
      random_float_y = ((float(rand()) / float(RAND_MAX)) * 2) - 1.0f;
      shoot_direction1.x *= random_float_x;
      shoot_direction1.y *= random_float_y;
      Projectile new_projectile1 = this->ShootAtDirection(shoot_direction1);
      active_projectiles.push_back(new_projectile1);
    }
    if(rand() % 2) {
      sf::Vector2f shoot_direction2(7, 7);
      sf::Vector2f direction_vector = player_position - body_.getPosition();
      float vector_magnitude = sqrt(pow(direction_vector.x, 2.0) + pow(direction_vector.y, 2.0));
      shoot_direction2.x *= direction_vector.x / vector_magnitude;
      shoot_direction2.y *= direction_vector.y / vector_magnitude;
      Projectile new_projectile2 = this->ShootAtDirection(shoot_direction2);
      active_projectiles.push_back(new_projectile2);
    }
  }
  if(enemy_animations_[0].Update(time)) {
    body_.setTexture(enemy_animations_[0].getCurrentFrame());
  }
  body_.move(direction_);
    /*Collider col = this->getCollider();
    for (auto room : rooms) {
      for (auto wall : room.walls) {
        for (auto w : wall) {
          if (w.type_ == 1) {
            Collider wallCol = w.getCollider();
            wallCol.checkCollider(col, 1);
          }
        }
      }
    }*/
};

Projectile FinalBoss::ShootAtDirection(sf::Vector2f direction) {
  //enemy_projectile_ = std::make_unique<Projectile>(enemy_projectile_texture);
  //Projectile Weapon::Fire(sf::Vector2f fire_position, sf::Vector2f fire_trajectory)
  Projectile proj(this->enemy_projectile_texture_);
  //proj.friendly_[0] = 0;
  proj.active_ = true;
  proj.body_.setTexture(proj.projectile_texture_);
  proj.body_.setOrigin(proj.body_.getSize() / 2.0f);
  proj.body_.setPosition(body_.getPosition());
  proj.body_.setSize(sf::Vector2f(25.0f, 25.0f));
  proj.projectile_trajectory_ = direction/* * proj.velocity_multiplier_*/;
  proj.body_.move(proj.projectile_trajectory_);
  return proj;
};
