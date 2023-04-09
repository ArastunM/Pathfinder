/**
 * represents the Map data structure of Pathfinder.
 * map stores group of Pixel objects.
 */

#ifndef PATHFINDER_MAP_HPP
#define PATHFINDER_MAP_HPP

#include "pixel.hpp"


/* === pathfinder map class definition ========================== */
class Map: public sf::Drawable, public sf::Transformable {
    sf::Vector2u size; // stores map width and height
    std::vector<std::vector<Pixel>> m_pixels; // structural contents of the map
    sf::VertexArray m_vertices; // vertex set
    sf::Texture m_tilesSet; // texture set

    void draw (sf::RenderTarget&, sf::RenderStates) const override;

public:
    /* === constructors and destructor === */
    Map ();
    explicit Map (unsigned int, unsigned int);
    ~Map () override;

    /* === map operation functions === */
    bool load (const std::string&);
    void randMap ();
    void printMap ();
    void updatePixel (unsigned int, unsigned int); // updates the pixel
    void displayPath (std::vector<Pixel>&);
    void updatePosition (Pixel&); // updates position of a pixel (based on its map position)
    void updatePosition (Pixel&, sf::Vector2i);
    void updateRunnerPosition (sf::Vector2i); // update position of runner pixel (based on movement directions)
    void updateTrackerPosition (sf::Vector2i); // update position of tracker pixel
    void updateTexture (Pixel&); // updates texture of a pixel (based on its type)

    /* === getters and setters === */
    std::vector<Pixel *> getOpenPixels (); // getting a list of open pixel areas
    std::vector<std::vector<Pixel>> & getPixels ();
    sf::Vector2u getSize () const;

    void setSize (sf::Vector2u&);
    void setRandPlayers (); // for now supports setting a runner and tracker
};


#endif
