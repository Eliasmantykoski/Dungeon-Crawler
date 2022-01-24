# Meeting Notes
In this file, you are required to take notes for your weekly meetings. 
In each meeting, you are required to discuss:

1. What each member has done during the week?
2. Are there challenges or problems? Discuss the possible solutions
3. Plan for the next week for everyone
4. Deviations and changes to the project plan, if any


# Meeting 01.12.2021 14::00

**Participants**: 
1. Urho Hakonen
2. Sampo Haikonen
3. Joona Kukkonen
4. Elias Mäntykoski 

## Summary of works
Different types of enemies, final boss, enemies have ranged attack, HUD updates, items are usable, more items added.


## Challenges
The project has still some memory leaks. SFML library itself may be the source of some issues detected by valgrind.
Program lags due to projectile class.
Enemies camp at door entrance.
Can not restart after game over(for ex. when player dies).

## Actions
The goal of the following week:
Documentation.
Main menu, finish additional features.


> Please reflect these action decisions in your git commit messages so that 
> your group members and advisor can follow the progress.

## Project status 
Player character with movement and ability to shoot, rooms with walls.
Basic enemies and animation. Enemies have a collider, can be killed and drop an item when killed. Enemies deal damage to the player.
Map generation and room traversing.
Basic HUD with hp and inventory.

### TODOs
1. Urho: character leveling, documentation
2. Sampo: Final boss and winning condition(killing the boss), shop keeper, main menu, consider weapon modification documentation
3. Elias: sound effects, documentation
4. Joona: sound effects, make the images transparent, documentation
