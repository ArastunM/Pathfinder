# Pathfinder
Pathfinder is C++ based 2D pathfinder simulator. It consists of a *runner* (controlled by the player) and a *tracker* which follows the runner using the shortest available path. 

Shortest path is found using the principles of [A* search algorithm](https://en.wikipedia.org/wiki/A*_search_algorithm). The program also allows placing and removing blocks/tiles, switching between players, pausing (<kbd>space</kbd>) and generating a new random map (<kbd>shift</kbd>).

Map loading uses a [*tileset*](assets/images/) standard to load. New tile sets can be created but should follow existing format.

<p align="center">
  <img src="assets/images/pathfinder_tileset.png"></img>
  <img src="assets/images/pathfinder_tileset2.png"></img>
</p>

<hr>
<p align="center">
  <img src="demo/example1.png" height="300" width="300">
  <img src="demo/example2.png" height="300" width="300">
</p>


## Prerequisites
The program was developed in [C++14](https://en.cppreference.com/w/cpp/14). Additionally, the [SFML](https://www.sfml-dev.org/) library was used for graphics and audio.


## Details
- Author - Arastun Mammadli
- License - [MIT](LICENSE)
