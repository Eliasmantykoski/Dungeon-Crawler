#include "map.hpp"

#include <stdio.h>  /* printf, scanf, puts, NULL */
#include <stdlib.h> /* srand, rand */
#include <time.h>   /* time */

#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <cstddef>
#include <iostream>
#include <map>
#include <numeric>
#include <utility>

#include "projectile.hpp"
//#include <cmath> /* abs */

Map::Map(sf::Texture *wall_texture, Animation *enemy_animation,
         Animation *enemy_animation2, Animation *boss_animation,
         sf::Texture *enemy_projectile_texture) {
  std::vector<std::pair<int, int>> visited;
  visited.push_back({100, 100});
  int depth = 0;
  Generate(wall_texture, enemy_animation, enemy_animation2, boss_animation,
           enemy_projectile_texture, 0, 0, {0, 1, 1, 0, 4}, visited);

  int highest_depth = 0;
  unsigned int highest_depth_index = 0;
  for (unsigned int i = 0; i < rooms.size(); i++) {
    if (rooms[i].depth_ > highest_depth) {
      highest_depth = rooms[i].depth_;
      highest_depth_index = i;
    }
  }
  sf::Vector2f boss_spawn_pos(rooms[highest_depth_index].xBound1,
                              rooms[highest_depth_index].yBound1);
  sf::Vector2f enemy_spawn_pos;
  enemy_spawn_pos.x = boss_spawn_pos.x + 50 + (rand() % 350);
  enemy_spawn_pos.y = boss_spawn_pos.y + 50 + (rand() % 350);

  FinalBoss *new_enemy = new FinalBoss(boss_animation, enemy_spawn_pos, 75,
                                       enemy_projectile_texture);
  // std::unique_ptr<Enemy> new_enemy(new FinalBoss(boss_animation,
  // enemy_spawn_pos, 75));
  enemies.push_back(new_enemy);
};

bool Map::Generate(sf::Texture *wall_texture, Animation *enemy_animation,
                   Animation *enemy_animation2, Animation *boss_animation,
                   sf::Texture *enemy_projectile_texture, int x, int y,
                   std::vector<int> openings,
                   std::vector<std::pair<int, int>> &visited) {
  static int calls = 0;
  calls++;
  bool vi = false;
  for (auto m : visited) {
    if (x == m.first && y == m.second) {
      vi = true;
    }
  }

  if (x < mapSize.x && y < mapSize.y && x >= 0 && y >= 0 &&
      std::accumulate(openings.begin(), openings.end() - 1, 0) > 1 &&
      vi == false) {
    visited.push_back({x, y});
    std::vector<int> new_openings;
    for (unsigned int i = 0; i < openings.size() - 1; i++) {
      if (openings[i] == 1 && openings[4] != int(i)) {
        new_openings.clear();
        for (auto j = 0; j < 4; j++) {
          new_openings.push_back(rand() % 2);
        }
        if (i < 2) {
          new_openings[i + 2] = 1;
          new_openings.push_back(i + 2);
        } else {
          new_openings[i - 2] = 1;
          new_openings.push_back(i - 2);
        }
        if (i == 0) {
          bool success = Generate(
              wall_texture, enemy_animation, enemy_animation2, boss_animation,
              enemy_projectile_texture, x, y - 1, new_openings, visited);
          if (success == false) {
            openings[0] = 0;
          }
        } else if (i == 1) {
          bool success = Generate(
              wall_texture, enemy_animation, enemy_animation2, boss_animation,
              enemy_projectile_texture, x + 1, y, new_openings, visited);
          if (success == false) {
            openings[1] = 0;
          }
        } else if (i == 2) {
          bool success = Generate(
              wall_texture, enemy_animation, enemy_animation2, boss_animation,
              enemy_projectile_texture, x, y + 1, new_openings, visited);
          if (success == false) {
            openings[2] = 0;
          }
        } else {
          bool success = Generate(
              wall_texture, enemy_animation, enemy_animation2, boss_animation,
              enemy_projectile_texture, x - 1, y, new_openings, visited);
          if (success == false) {
            openings[3] = 0;
          }
        }
      }
    }

    /*
    The enemies are added inside the map and they are going to be located with
    respect to the room spawn positions. That means that every room is going to
    have a random amount of enemies located at random coordinates in the room,
    but the enemy location is not limited to the rooms, because enemies are
    located in the map, not in the rooms.
    */
    sf::Vector2f room_spawn_pos =
        sf::Vector2f(25.0f + 50 * 10 * x, 25.0f + 50 * 10 * y);
    // int random_enemy_amount = std::abs(rand() % 5);
    // int random_enemy_amount = rand() % 5;
    // int random_enemy_amount2 = rand() % 3;
    int random_enemy_amount = rand() % 4;
    int random_enemy_amount2 = rand() % 2;
    // int random_enemy_amount2 = 0;
    for (int i = 0; i < random_enemy_amount; i++) {
      sf::Vector2f enemy_spawn_pos;
      enemy_spawn_pos.x = room_spawn_pos.x + 50 + (rand() % 350);
      enemy_spawn_pos.y = room_spawn_pos.y + 50 + (rand() % 350);
      // std::unique_ptr<Enemy> new_enemy =
      // std::make_unique<ChasingEnemy>(enemy_animation, enemy_spawn_pos, 100);
      // ChasingEnemy new_enemy(enemy_animation, enemy_spawn_pos, 100);
      ChasingEnemy *new_enemy =
          new ChasingEnemy(enemy_animation, enemy_spawn_pos, 100);
      enemies.push_back(new_enemy);
    }
    for (int i = 0; i < random_enemy_amount2; i++) {
      sf::Vector2f enemy_spawn_pos;
      enemy_spawn_pos.x = room_spawn_pos.x + 50 + (rand() % 350);
      enemy_spawn_pos.y = room_spawn_pos.y + 50 + (rand() % 350);
      // std::unique_ptr<Enemy> new_enemy =
      // std::make_unique<ChasingEnemy>(enemy_animation, enemy_spawn_pos, 75);
      // std::unique_ptr<Enemy> new_enemy(new RangedEnemy(enemy_animation2,
      // enemy_spawn_pos, 75, enemy_projectile_texture)); RangedEnemy
      // new_enemy(enemy_animation2, enemy_spawn_pos, 75);
      RangedEnemy *new_enemy = new RangedEnemy(
          enemy_animation2, enemy_spawn_pos, 75, enemy_projectile_texture);
      enemies.push_back(new_enemy);
    }

    Room room(wall_texture,
              sf::Vector2f(25.0f + 50 * 10 * x, 25.0f + 50 * 10 * y), openings,
              calls);
    rooms.push_back(room);
    calls--;
    return true;
  }
  calls--;
  return false;
};

void Map::Display(sf::RenderWindow &window, Collider playerCollider) {
  for (auto r : rooms) {
    r.Display(window, playerCollider, enemies);
  }
  for (auto i : items) {
    i.Draw(window);
  }
};

void Map::NextRoom(int direction) {
  for (unsigned int i = 0; i < rooms.size(); i++) {
    rooms[i].MoveRoom(direction, items);
  }
  for (unsigned int i = 0; i < enemies.size(); i++) {
    enemies[i]->MoveRoom(direction);
  }
}

void Map::SpawnItem(unsigned int &i, sf::Texture *hp_text,
                    sf::Texture *speed_text, sf::Texture *coin_text) {
  sf::Vector2f pos = enemies[i]->getPosition();
  int item_index = rand() % 3;
  if (item_index == 0) {
    Item drop(hp_text, pos, "hp_pot");
    items.push_back(drop);
  } else if (item_index == 1) {
    Item drop(speed_text, pos, "speed_pot");
    items.push_back(drop);
  } else {
    Item drop(coin_text, pos, "coin");
    items.push_back(drop);
  }
}

void Map::removeItem(std::vector<Item>::iterator i) { items.erase(i); };
