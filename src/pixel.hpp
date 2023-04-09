/**
 * represents individual Pixel object (part of Map object).
 * pixels have type, color and reference to neighbours.
 */

#ifndef PATHFINDER_PIXEL_HPP
#define PATHFINDER_PIXEL_HPP
#define CLOSED_FREQ 20 // obstacle frequency %
#define TILE_SIZE 25 // size of an individual pixel object (when drawn)
#define UP sf::Vector2i{0, -1}
#define DOWN sf::Vector2i{0, 1}
#define RIGHT sf::Vector2i{1, 0}
#define LEFT sf::Vector2i{-1, 0}

#include <SFML/Graphics.hpp>


/* === pathfinder pixel class definition ========================== */
class Pixel {
public:
    enum Type { // with flags
        OPEN=0, // area open for travel
        CLOSED=1, // area with an obstacle
        TRACKER=2, // area occupied by a tracker (manually set to OPEN types)
        RUNNER=3, // area occupied by a runner (aka selected tracker)
        PATH=4 // area to display a path (from tracker to runner)
    };
private:
    Type type;
    sf::Vector2u pos; // position on Map

public:
    /* === constructors and destructor === */
    explicit Pixel ();
    explicit Pixel (Type);
    ~Pixel ();

    /* === pixel operation functions === */
    static Pixel randPixel ();
    void printPixel () const;
    bool clickedOn (sf::Event::MouseButtonEvent) const;
    static bool liesOnMap (sf::Vector2u, sf::Vector2u);
    bool isTraversable (sf::Vector2u) const;
    friend bool operator==(const Pixel&, const Pixel&);
    friend bool operator>(const Pixel&, const Pixel&);
    friend bool operator<(const Pixel&, const Pixel&);
    static void swapTypes (Pixel &pixel1, Pixel &pixel2);

    /* === getters and setters === */
    Type getType () const;
    sf::Vector2u getPos () const;
    sf::Vector2u getWinPos () const;
    static sf::Vector2u getPosDist (Pixel, Pixel);
    static Pixel getTracker (std::vector<std::vector<Pixel>>&);
    static Pixel getRunner (std::vector<std::vector<Pixel>>&);

    void setType (Type);
    void setPos (unsigned int, unsigned int);
};


#endif
