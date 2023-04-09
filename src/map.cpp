/**
 * implements the map.hpp header file
 */

#include "map.hpp"


/* === pathfinder map class implementation ========================== */
Map::Map() = default;
Map::Map(unsigned int width, unsigned int height) {
    this->size = sf::Vector2u{width, height};
    randMap(); // sets up a random map
}

void Map::draw(sf::RenderTarget & target, sf::RenderStates states) const {
    states.transform *= getTransform();
    states.texture = &m_tilesSet;
    target.draw(m_vertices, states);
}


/* === pathfinder map operation functions ========================== */
bool Map::load(const std::string & tileSet) {
    // loading the tileSet texture
    if (!m_tilesSet.loadFromFile(tileSet)) {
        fprintf(stderr, "map tile set texture could not be loaded\n");
        return false;
    }
    // resizing the vertex array: map width * height * corners
    m_vertices.setPrimitiveType(sf::Quads);
    m_vertices.resize(size.x * size.y * 4);

    // populating the vertex array (quad per tile)
    for (unsigned int i = 0; i <size.x; i++) {
        for (unsigned int j = 0; j < size.y; j++) {
            // getting the current tile/pixel type
            Pixel & pixel = this->m_pixels[i][j];
            // setting up positions and texture
            updatePosition(pixel);
            updateTexture(pixel);
        }
    } return true;
}

void Map::randMap() {
    m_pixels = std::vector<std::vector<Pixel>>(size.x); // allocation
    for (unsigned int x = 0; x < size.x; x++) {
        m_pixels[x] = std::vector<Pixel>(size.y);
        for (unsigned int y = 0; y < size.y; y++) {
            m_pixels[x][y] = Pixel::randPixel();
            m_pixels[x][y].setPos(x, y);
        }
    }
    this->setRandPlayers(); // adds the players onto the map
}

void Map::printMap() {
    printf("\nTHE PATHFINDER MAP\n");
    for (auto & pixelRow : m_pixels) {
        printf("%s\n", std::string(pixelRow.size() * 2, '_').c_str());
        for (auto & pixel : pixelRow) {
            pixel.printPixel();
        } printf("|\n"); // ending the row
    } printf("%s\n", std::string(m_pixels[0].size() * 2, '_').c_str());
}

void Map::updatePixel(unsigned int x, unsigned int y) {
    // changing selected pixel: TRACKER<==>RUNNER, OPEN=PATH<===>CLOSED
    Pixel & pixel = this->m_pixels[x][y];
    sf::Vector2u runnerPos = Pixel::getRunner(this->m_pixels).getPos();

    if (pixel.getType() == Pixel::TRACKER) {
        this->m_pixels[runnerPos.x][runnerPos.y].setType(Pixel::TRACKER);
        updateTexture(this->m_pixels[runnerPos.x][runnerPos.y]);
        pixel.setType(Pixel::RUNNER); updateTexture(pixel);
    }
    else if (pixel.getType() == Pixel::OPEN || pixel.getType() == Pixel::PATH) {
        pixel.setType(Pixel::CLOSED);
        updateTexture(pixel);
    }
    else if (pixel.getType() == Pixel::CLOSED) {
        pixel.setType(Pixel::OPEN);
        updateTexture(pixel);
    }
}

void Map::displayPath(std::vector<Pixel> & pixelPath) {
    // clearing up the existing path
    for (auto & pixelRow : this->m_pixels) {
        for (auto & pixel : pixelRow) {
            if (pixel.getType() == Pixel::PATH) {
                pixel.setType(Pixel::OPEN);
                updateTexture(pixel);
            }
        }
    }
    for (auto & pixel : pixelPath) {
        m_pixels[pixel.getPos().x][pixel.getPos().y].setType(Pixel::PATH);
        updateTexture(m_pixels[pixel.getPos().x][pixel.getPos().y]);
    }
}

void Map::updatePosition(Pixel & pixel) { // upon reloading
    // selecting the quad to be updated
    sf::Vertex * quad = &m_vertices[(pixel.getPos().x + pixel.getPos().y * size.x) * 4];
    auto fI = static_cast<float>(pixel.getPos().x); auto fJ = static_cast<float>(pixel.getPos().y);

    // defining quad positions (4 corners)
    quad[0].position = sf::Vector2f(fI * TILE_SIZE, fJ * TILE_SIZE); // 0, 0
    quad[1].position = sf::Vector2f((fI + 1) * TILE_SIZE, fJ * TILE_SIZE); // 500, 0
    quad[2].position = sf::Vector2f((fI + 1) * TILE_SIZE, (fJ + 1) * TILE_SIZE); // 500, 500
    quad[3].position = sf::Vector2f(fI * TILE_SIZE, (fJ + 1) * TILE_SIZE); // 0, 500
}

void Map::updatePosition(Pixel & pixel, sf::Vector2i direction) { // updates tracker and runner positions
    // only updates position if given pixel is a runner/tracker, and target is traversable
    sf::Vector2u runnerPos = Pixel::getRunner(this->m_pixels).getPos();
    sf::Vector2u trackerPos = Pixel::getTracker(this->m_pixels).getPos();
    if (!(pixel.getPos() == runnerPos || pixel.getPos() == trackerPos)) return;

    // only update position if the target is traversable
    sf::Vector2u targetPos = sf::Vector2u{pixel.getPos().x + direction.x, pixel.getPos().y + direction.y};
    if (!Pixel::liesOnMap(targetPos, this->size)) return;
    Pixel & targetPixel = this->m_pixels[targetPos.x][targetPos.y];
    if (!targetPixel.isTraversable(this->size)) return;

    // swapping and updating the 2 pixels
    Pixel temp = pixel;
    pixel.setType(targetPixel.getType());
    targetPixel.setType(temp.getType());
    updateTexture(pixel);
    updateTexture(targetPixel);
}

void Map::updateTrackerPosition(sf::Vector2i direction) {
    sf::Vector2u trackerPos = Pixel::getTracker(this->m_pixels).getPos();
    Pixel & tracker = this->m_pixels[trackerPos.x][trackerPos.y];
    updatePosition(tracker, direction);
}

void Map::updateRunnerPosition(sf::Vector2i direction) {
    sf::Vector2u runnerPos = Pixel::getRunner(this->m_pixels).getPos();
    Pixel & runner = this->m_pixels[runnerPos.x][runnerPos.y];
    updatePosition(runner, direction);
}

void Map::updateTexture(Pixel & pixel) {
    // selecting the quad to be updated
    sf::Vertex * quad = &m_vertices[(pixel.getPos().x + pixel.getPos().y * size.x) * 4];
    // setting texture start and end locations
    if ((m_tilesSet.getSize().x / m_tilesSet.getSize().y) < pixel.getType()) {
        fprintf(stderr, "tile set texture not provided for pixel type %d\n", pixel.getType());
        exit(-1);
    }
    float textureStart = static_cast<float>(pixel.getType() * m_tilesSet.getSize().y);
    float textureEnd = textureStart + static_cast<float>(m_tilesSet.getSize().y);

    // defining corresponding texture coordinates
    quad[0].texCoords = sf::Vector2f(textureStart, 0);
    quad[1].texCoords = sf::Vector2f(textureEnd, 0);
    quad[2].texCoords = sf::Vector2f(textureEnd, static_cast<float>(m_tilesSet.getSize().y));
    quad[3].texCoords = sf::Vector2f(textureStart, static_cast<float>(m_tilesSet.getSize().y));
}


/* === pathfinder map class getters and setters ========================== */
std::vector<Pixel *> Map::getOpenPixels() {
    std::vector<Pixel *> openPixels;
    for (auto & pixelRow : m_pixels) {
        for (auto & pixel : pixelRow) {
            if (pixel.getType() == Pixel::OPEN) openPixels.push_back(&pixel);
        }
    } return openPixels;
}
std::vector<std::vector<Pixel>> &Map::getPixels() {
    return m_pixels;
}
sf::Vector2u Map::getSize() const {
    return size;
}

void Map::setSize(sf::Vector2u & newSize) {
    this->size = newSize;
}
void Map::setRandPlayers() {
    std::vector<Pixel *> openPixels = getOpenPixels();
    // random index of the 2 players
    int index1 = std::rand() % openPixels.size();
    int index2 = std::rand() % openPixels.size();
    openPixels[index1]->setType(Pixel::TRACKER);
    openPixels[index2]->setType(Pixel::RUNNER);
}

Map::~Map() = default;
