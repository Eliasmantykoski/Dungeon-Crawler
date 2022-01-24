#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>
#include <vector>
//#include <memory>
#include <string>

#include "animation.hpp"
#include "collider.hpp"

#ifndef ENEMY_CLASS
#define ENEMY_CLASS

class Projectile; //Forward declaration is used here to avoid circular dependency with projectile class

/**Virtual class for enemies*/
class Enemy {
 public:
  /**Constructor*/
  Enemy(Animation *enemy_animation, sf::Vector2f spawnPos, float speed);
  /**Default destructor*/
  ~Enemy();
  /**Returns enemy position*/
  sf::Vector2f getPosition();
  /**Check if enemy is ranged*/
  bool isRanged();
  /**Returns enemy collider*/
  Collider getCollider()
  {
    return Collider(body_);
  };
  /**Moves enemy when player is traversing between rooms*/
  void MoveRoom(int dir);
  /**Used for rendering the enemy*/
  void Draw(sf::RenderWindow& window);
  /**Reduces enemy hp*/
  void Damage(float dmg);
  /**Virtual function for updating enemy position*/
  virtual void Update(float time, sf::Vector2f player_position) = 0;

  virtual void Update2(float time, sf::Vector2f player_position, std::vector<Projectile>& active_projectiles);

  bool walk_ = false;
  bool attacking_player_ = false;
  bool is_ranged_type_ = false;
  float speed_ = 100;
  float hp_ = 100;
  float damage_ = 1;
  float currentSpeed_;
  float actionTimer_;
  float walkInterval_ = 2;
  bool alive_ = true;
  sf::RectangleShape body_;
  sf::Vector2f direction_;
  std::vector<Animation> enemy_animations_;
  bool boss = false;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**Class for enemy that chases the player*/
class ChasingEnemy : public Enemy{
 public:
  ChasingEnemy(Animation *enemy_animation, sf::Vector2f spawnPos, float speed);

  ~ChasingEnemy();

  virtual void Update(float time, sf::Vector2f player_position) override;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Projectile;
/**Class for enemy that shoots projectiles*/
class RangedEnemy : public Enemy{
 public:
  RangedEnemy(Animation *enemy_animation, sf::Vector2f spawnPos, float speed,
  sf::Texture* enemy_projectile_texture);

  ~RangedEnemy();

  virtual void Update(float time, sf::Vector2f player_position) override;

  void Update2(float time, sf::Vector2f player_position, std::vector<Projectile>& active_projectiles);
  /**Used to shoot projetiles towards direction*/
  Projectile ShootAtDirection(sf::Vector2f direction);

  sf::Texture* enemy_projectile_texture_;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**Class for the finall Boss*/
class FinalBoss : public Enemy{
public:

  FinalBoss(Animation *enemy_animation, sf::Vector2f spawnPos, float speed,
  sf::Texture* enemy_projectile_texture);

  virtual void Update(float time, sf::Vector2f player_position) override;

  void Update2(float time, sf::Vector2f player_position, std::vector<Projectile>& active_projectiles);
  /**Used to shoot projetiles towards direction*/
  Projectile ShootAtDirection(sf::Vector2f direction);
  
  sf::Texture* enemy_projectile_texture_;
};
#endif
