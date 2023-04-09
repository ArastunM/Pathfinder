/**
 * implementation of pixel.hpp header file
 */

#include "pixel.hpp"


/* === pathfinder pixel class implementation ========================== */
Pixel::Pixel(): type(OPEN){}
Pixel::Pixel(Pixel::Type type): type(type){}


/* === pathfinder pixel operation functions ========================== */
Pixel Pixel::randPixel() { // non-player pixels
    unsigned int choice = std::rand() % 100; // (0, 1, ... 99)
    if (choice < CLOSED_FREQ) return Pixel(CLOSED);
    else return Pixel(OPEN);
}

void Pixel::printPixel() const {
    if (type == OPEN) {
        printf("| ");
    } else if (type == CLOSED) {
        printf("|@");
    } else if (type == TRACKER) {
        printf("|*");
    } else if (type == RUNNER) {
        printf("|^");
    } else {
        printf("|+");
    }
}

bool Pixel::clickedOn(sf::Event::MouseButtonEvent clickCoord) const {
    sf::Vector2u winPos = getWinPos();
    return (winPos.x <= clickCoord.x && clickCoord.x < winPos.x + TILE_SIZE &&
            winPos.y <= clickCoord.y && clickCoord.y <  winPos.y + TILE_SIZE);
}
bool Pixel::liesOnMap(sf::Vector2u pixelPos, sf::Vector2u mapSize) {
    return pixelPos.x >= 0 && pixelPos.y >= 0 &&
           pixelPos.x < mapSize.x && pixelPos.y < mapSize.y;
}
bool Pixel::isTraversable(sf::Vector2u mapSize) const {
    // a pixel is traversable if it lies on the map and is not an obstacle (CLOSED)
    return Pixel::liesOnMap(this->getPos(), mapSize) && type != Pixel::CLOSED;
}

bool operator==(const Pixel & pixel1, const Pixel & pixel2) {
    return (pixel1.type == pixel2.type) && (pixel1.pos == pixel2.pos);
}
bool operator>(const Pixel & pixel1, const Pixel & pixel2) {
    return pixel1.type > pixel2.type;
}
bool operator<(const Pixel & pixel1, const Pixel & pixel2) {
    return pixel1.type < pixel2.type;
}
void Pixel::swapTypes(Pixel & pixel1, Pixel & pixel2) {
    Type & tempType = pixel1.type;
    pixel1.type = pixel2.type;
    pixel2.type = tempType;
}


/* === pathfinder pixel class getters and setters ========================== */
Pixel::Type Pixel::getType() const {
    return type;
}
sf::Vector2u Pixel::getPos() const {
    return pos;
}
sf::Vector2u Pixel::getWinPos() const {
    return sf::Vector2u {pos.x * TILE_SIZE, pos.y * TILE_SIZE};
}
sf::Vector2u Pixel::getPosDist(Pixel p1, Pixel p2) {
    // returns the position distance between two pixels
    unsigned int xDistance = p1.getPos().x > p2.getPos().x ?
            p1.getPos().x - p2.getPos().x : p2.getPos().x - p1.getPos().x;
    unsigned int yDistance = p1.getPos().y > p2.getPos().y ?
            p1.getPos().y - p2.getPos().y : p2.getPos().y - p1.getPos().y;
    return sf::Vector2u {xDistance, yDistance};
}

Pixel Pixel::getTracker(std::vector<std::vector<Pixel>> & pixels) {
    Pixel tracker;
    for (auto & pixelRow : pixels) {
        for (auto & pixel : pixelRow) {
            if (pixel.getType() == Pixel::TRACKER) {
                tracker = pixel;
                break;
            }
        }
    }
    if (tracker.getType() != Pixel::TRACKER) {
        fprintf(stderr, "No tracker pixel exists on given map\n");
        exit(-1);
    } return tracker;
}
Pixel Pixel::getRunner(std::vector<std::vector<Pixel>> & pixels) {
    Pixel runner;
    for (auto & pixelRow : pixels) {
        for (auto & pixel : pixelRow) {
            if (pixel.getType() == Pixel::RUNNER) {
                runner = pixel;
                break;
            }
        }
    }
    if (runner.getType() != Pixel::RUNNER) {
        fprintf(stderr, "No runner pixel exists on given map\n");
        exit(-1);
    } return runner;
}

void Pixel::setType(Pixel::Type newType) {
    this->type = newType;
}
void Pixel::setPos(unsigned int x, unsigned int y) {
    pos = sf::Vector2u {x, y};
}

Pixel::~Pixel() = default;
