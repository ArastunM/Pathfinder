/**
 * implements appWindow.hpp header file
 */

#include "appWindow.hpp"


/* === pathfinder app window class implementation ========================== */
AppWindow::AppWindow(unsigned int width, unsigned int height) {
    window.create(sf::VideoMode(width, height),"Pathfinder");
    isPaused = false;
    this->loadNewMap();
}

void AppWindow::launchWin() {
    // main loop
    sf::Time elapsed;
    while (window.isOpen()) {
        // event loop (checks for triggered events in every iteration)
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close(); // window close
            if (event.type == sf::Event::MouseButtonPressed) checkPixelClick(); detectShortestPath(); // runner selection
            if (event.type == sf::Event::KeyPressed) { // keyboard presses
                if (event.key.code == sf::Keyboard::W) map.updateRunnerPosition(UP); detectShortestPath();
                if (event.key.code == sf::Keyboard::S) map.updateRunnerPosition(DOWN); detectShortestPath();
                if (event.key.code == sf::Keyboard::D) map.updateRunnerPosition(RIGHT); detectShortestPath();
                if (event.key.code == sf::Keyboard::A) map.updateRunnerPosition(LEFT); detectShortestPath();
                if (event.key.code == sf::Keyboard::Enter) detectShortestPath(); // calculates path
                if (event.key.code == sf::Keyboard::RShift) loadNewMap(); // loads a new map
                if (event.key.code == sf::Keyboard::Space) isPaused = !isPaused; // pauses/unpauses the game
            }
        }

        // pixel update operations
        if (clock.getElapsedTime().asSeconds() > 0.5) {
            elapsed = clock.getElapsedTime();
            clock.restart();
            // make a tracker move every 1 second
            if (!isPaused && !shortestPath.empty()) updateTrackerPos();

        }
        window.clear(sf::Color::Black);
        window.draw(map);
        window.display();
    }
}


/* === pathfinder app window operation functions ========================== */
void AppWindow::loadNewMap() {
    map = Map(window.getSize().x/TILE_SIZE, window.getSize().y/TILE_SIZE);
    if (!map.load(TEXTURE_PATH)) {
        fprintf(stderr, "Map could not be loaded\n");
        exit(-1); // map could not be loaded
    }
}

void AppWindow::detectShortestPath() {
    shortestPath = getShortestPath(map);
    // removing the first and last from tracker (so that tracker and runner are not shown as path)
    shortestPath.pop_back(); shortestPath.erase(shortestPath.begin());
    // display the shortestPath if a path has been found
    if (!shortestPath.empty()) map.displayPath(shortestPath);
}

void AppWindow::updateTrackerPos() {
    sf::Vector2u trackerPos = Pixel::getTracker(this->map.getPixels()).getPos();
    sf::Vector2u pathPos = shortestPath.front().getPos();
    sf::Vector2i directionI = sf::Vector2i {static_cast<int>(pathPos.x - trackerPos.x),
                                            static_cast<int>(pathPos.y - trackerPos.y)};
    shortestPath.erase(shortestPath.begin());
    this->map.displayPath(shortestPath);
    this->map.updateTrackerPosition(directionI);
}


void AppWindow::checkPixelClick() {
    for (auto & pixelRow : map.getPixels()) {
        for (auto & pixel : pixelRow) {
            if (pixel.clickedOn(event.mouseButton)) {
                map.updatePixel(pixel.getPos().x, pixel.getPos().y);
            }
        }
    }
}
