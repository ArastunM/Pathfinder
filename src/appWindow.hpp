/**
 * main SFML window of the Pathfinder
 */

#ifndef PATHFINDER_APP_WINDOW_HPP
#define PATHFINDER_APP_WINDOW_HPP
#define TEXTURE_PATH "assets/images/pathfinder_tileset2.png"

#include <SFML/Graphics.hpp>
#include "map.hpp"
#include "pathfinder.hpp"


/* === pathfinder app window class definition ========================== */
class AppWindow {
    sf::RenderWindow window;
    sf::Event event{};
    sf::Clock clock;
    Map map; // pathfinder map
    std::vector<Pixel> shortestPath; // last updated path between tracker and runner
    bool isPaused; // whether the tracker should be paused

public:
    /* === constructors and destructor === */
    explicit AppWindow (unsigned int, unsigned int);
    void launchWin ();

    /* === app window operation functions === */
    void loadNewMap ();
    void detectShortestPath ();
    void updateTrackerPos ();
    void checkPixelClick ();
};


#endif
