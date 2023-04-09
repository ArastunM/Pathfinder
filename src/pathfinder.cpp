/**
 * implementation of pathfinder.hpp header file
 */

#include "pathfinder.hpp"


/* === pathfinder algorithm implementations ========================== */
std::vector<Pixel> getShortestPath(Map & map) {
    // setting the tracker and runner objects (start and end nodes)
    Pixel tracker = Pixel::getTracker(map.getPixels());
    Pixel runner = Pixel::getRunner(map.getPixels());
    return getShortestPath(map, tracker, runner);
}

std::vector<Pixel> getShortestPath(Map & map, Pixel & origin, Pixel & target) {
    // setting the start node and explored nodes
    NODE currentNode = std::make_pair(origin, std::vector<Pixel>{origin});
    std::vector<Pixel> explored;
    // priority queue: used to store neighbours, and retrieve them based on priority
    std::priority_queue<PRIORITY_PAIR> priorityQueue;
    priorityQueue.push(std::make_pair(0, currentNode));

    // starting the search loop
    while (!(currentNode.first == target) || priorityQueue.empty()) {
        extractPriorityPair(priorityQueue, currentNode);
        explored.push_back(currentNode.first);

        // searching through neighbours
        std::vector<Pixel> neighbours = getNeighbours(map, currentNode.first);
        for (auto & neighbour : neighbours) {
            // if the pixel has not been explored yet, it is added to queue for exploration
            if (!std::count(explored.begin(), explored.end(), neighbour)) {
                std::vector<Pixel> neighbour_path = currentNode.second;
                neighbour_path.push_back(neighbour);

                // priority queue is updated
                int newCost = getPathHeuristic(currentNode.second, neighbour) +
                              getSebastianHeuristic(neighbour, target);
                priorityQueue.push(std::make_pair(newCost, std::make_pair(neighbour, neighbour_path)));
            }
        }
    }

    // returning the result
    if (!(currentNode.first == target)) return std::vector<Pixel>{};
    return currentNode.second;
}

void extractPriorityPair(std::priority_queue<PRIORITY_PAIR> & priorityQueue, NODE & currentNode) {
    // current node is reassigned to the best node in priority queue
    PRIORITY_PAIR priorityPair = priorityQueue.top(); priorityQueue.pop();
    currentNode = priorityPair.second;
}

int getSebastianHeuristic(Pixel & p1, Pixel & p2) {
    /** idea from Sebastian Lague's pathfinder heuristic
     * @see https://youtu.be/nhiFx28e7JY
     * heuristic returns a negative value;
     * since priority queue will prioritize the largest (hence closest to zero)
     */
    sf::Vector2u distance = Pixel::getPosDist(p1, p2);
    int minDist = static_cast<int>(std::min(distance.x, distance.y));
    int remainingDist = static_cast<int>(std::max(distance.x, distance.y) - minDist);
    return -(minDist * DIAGONAL_DIST + remainingDist * MANHATTAN_DIST);
}

int getPathHeuristic(std::vector<Pixel> & pixelPath, Pixel & p2) {
    // returns heuristic value of a path going through pixelPath followed with p2
    int heuristicVal = 0;
    for (unsigned int i = 0; i < pixelPath.size() - 1; i++) {
        heuristicVal += getSebastianHeuristic(pixelPath[i], pixelPath[i + 1]);
    } heuristicVal += getSebastianHeuristic(pixelPath[pixelPath.size() - 1], p2);
    return heuristicVal;
}


/* === pathfinder algorithm helper function implementations ========================== */
std::vector<Pixel> getNeighbours(Map & map, Pixel & pixel) {
    // neighbouring path is: neighbouring pixels that are traversable
    std::vector<Pixel> neighbourList;
    for (auto & mapPixelRow : map.getPixels()) {
        for (auto & mapPixel : mapPixelRow) {
            if (areNeighbours(pixel, mapPixel) && mapPixel.isTraversable(map.getSize())
                && !areDiagonallyBlocked(map, pixel, mapPixel))
                neighbourList.push_back(mapPixel);
        }
    } return neighbourList;
}

bool areNeighbours(Pixel & p1, Pixel & p2) {
    // two pixels are neighbours if:
    // 1) they have a distance of zero or one 2) they are not the same
    sf::Vector2u distance = Pixel::getPosDist(p1, p2);
    return !(p1 == p2) && distance.x <= 1 && distance.y <= 1;
}

bool areDiagonal(Pixel & p1, Pixel & p2) {
    // two pixels are diagonal if they are neighbours and their x and y distances sum to 2
    sf::Vector2u distance = Pixel::getPosDist(p1, p2);
    return distance.x + distance.y == 2;
}

bool areDiagonallyBlocked(Map & map, Pixel & p1, Pixel & p2) {
    // diagonally blocked means only direct open path between two pixels is diagonal
    if (!areDiagonal(p1, p2)) return false;
    int blockCount = 0;
    for (auto & mapPixelRow : map.getPixels()) {
        for (auto & mapPixel : mapPixelRow) {
            if ((areNeighbours(p1, mapPixel) && areNeighbours(p2, mapPixel))
                && (mapPixel.getType() == Pixel::CLOSED)) {
                blockCount++;
            }
        }
    } return blockCount == 2;
}


void printPath(std::vector<Pixel> & pixels) {
    for (auto & pixel : pixels) {
        printf("[%d][%d] -> ", pixel.getPos().x, pixel.getPos().y);
    }
    printf("|\n");
}
