/**
 * contains the pathfinder A* algorithm.
 * shortest path between two points (pixels) in any given a map.
 */

#ifndef PATHFINDER_PATHFINDER_HPP
#define PATHFINDER_PATHFINDER_HPP
#define MANHATTAN_DIST 10 // manhattan (orthogonal) distance is 10x
#define DIAGONAL_DIST 14 // diagonal (edge) distance is 14x

#include <cmath>
#include <queue>
#include <iostream>
#include "map.hpp"
#include "pixel.hpp"


/* === pathfinder structure definition ========================== */
typedef std::pair<Pixel, std::vector<Pixel>> NODE;
typedef std::pair<int, NODE> PRIORITY_PAIR;


/* === pathfinder algorithm primary function prototypes ========================== */
std::vector<Pixel> getShortestPath (Map&);
std::vector<Pixel> getShortestPath (Map&, Pixel&, Pixel&);
void extractPriorityPair (std::priority_queue<PRIORITY_PAIR>&, NODE&); // extracts and assigns from priority pair
int getSebastianHeuristic (Pixel&, Pixel&); // returns the heuristic value using Sebastian's method
int getPathHeuristic (std::vector<Pixel>&, Pixel&); // returns heuristic value of a path


/* === pathfinder algorithm helper function prototypes ========================== */
std::vector<Pixel> getNeighbours (Map &map, Pixel &pixel); // returns list of traversable neighbours of given pixel
bool areNeighbours (Pixel&, Pixel&);
bool areDiagonal (Pixel&, Pixel&);
bool areDiagonallyBlocked (Map &, Pixel&, Pixel&);

void printPath (std::vector<Pixel>&);

#endif
