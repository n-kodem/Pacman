#include <iostream>
#include <cmath>
#include <list>
#include <vector>
#include <algorithm>
// Storing pos
class Position
{
    int x, y;
public:
    Position(int _x, int _y) : x(_x), y(_y) {}
    Position() = default;
    Position operator +(const Position& other) {
        Position temp;
        temp.x = this->x + other.x;
        temp.y = this->y + other.y;
        return temp;
    }
    int getX() const { return x; }
    int getY() const { return y; }

    friend class Map;
};
// Node Class
struct Node
{
    Position position;
    // G - Optimal Way H - Distance F - Predicted distance
    int G, H, F;
    Node* parent = nullptr;
    Node() = default;
    Node(const Node& other) = default;
    Node(Position pos):position(pos) {};


    // Calculating Position
    void calcPos(const Position& endPos) {
        H = std::abs(position.getX() - endPos.getX()) + std::abs(position.getY() - endPos.getY());
        G = parent ? parent->G + 1 : 1;
        F = G + H;
    }
    // Operators
    bool operator==(const Node& other) const {
        return (position.getX() == other.position.getX() && position.getY() == other.position.getY());
    }
    bool operator!=(const Node& other) const {
        return !(*this == other);
    }
    bool operator<(const Node& other)  const {
        return(F < other.F);
    }
};
// Class MAP
// - Map and everything related to it
class Map
{
    std::vector<std::string> data;
    int size;

public:
    Map() = default;
    // Creates clear map
    Map(int _size) : size(_size) {
        std::size_t area = size * size;
        data.resize(area);
        for (std::size_t i = 0; i < area; ++i) {
            data[i] = "WOL";
        }
    }
    //Map display
    void display() const{
        for (std::size_t i = 0, area = size * size; i < area; ++i) {
            for (std::size_t endOfRow = i + size - 1; i < endOfRow; ++i) {
                 std::cout << data[i] << " ";
            }
            std::cout << data[i] << "\n";
        }
    }
    std::size_t getIndexFrom(Position p) const {
        return p.getX() + p.getY() * size;
    }
    // Prevention of errors in array sizes
    bool getIfInDanger(Position position) const {
        if (position.y < 0) {
            position.y = 0;
        }
        if (position.x < 0) {
            position.x = 0;
        }
        if (position.y >= size) {
            position.y = size - 1;
        }
        if (position.x >= size) {
            position.x = size - 1;
        }
        return data[getIndexFrom(position)] == "MUR";
    }
    // Deleting elements
    void setElement(std::string&& asda, Position position) {data[position.getX() + (position.getY() * size)] = asda;}
};

// Class solver
class Solver
{
    Position startPos, endPos;
    std::vector<Position> directions;
    Map map;
public:
    Solver(Position _startPos, Position _endPos, int size, string** WallMapData ) : startPos(_startPos), endPos(_endPos){
        Map temp(size);
        map = temp;
        // Downloading wall positions
        for (int i = 0;i<20;i+=1){
            for(int j = 0;j<20;j+=1){
                if(WallMapData[i][j]=="MUR"){map.setElement("MUR",Position(i,j));}
            }
        }
        // Bot directions
        directions.resize(4);
        directions[0] = Position(-1, 0);
        directions[1] = Position(0, 1);
        directions[2] = Position(0, -1);
        directions[3] = Position(1, 0);
    }
    // A(star) algorithm
    std::array<int,2> aStar(int map_size) {
        std::array<int,2> i_posToGo;
        Node startNode(startPos);
        Node goalNode(Position(endPos.getX(), endPos.getY()));

        if ((map.getIfInDanger(startNode.position) || map.getIfInDanger(goalNode.position))&& (startNode.position.getX()!=goalNode.position.getX() && startNode.position.getY()!=goalNode.position.getY())) {
            std::cout << "Poczatek drogi to sciana.";
        }
        // Unvisited positions
        std::list<Node> unvisited;
        // Visited positions
        std::list<Node> visited;

        startNode.calcPos(endPos);
        // Startnode becomes first unvisited node
        unvisited.push_back(startNode);

        // As long as we do not check all unvisited positions
        while (!unvisited.empty()) {
            auto current = Node(*std::min_element(unvisited.begin(), unvisited.end()));
            current.calcPos(endPos);

            // If visited position is a goalNode
            visited.push_back(current);
            if (current == goalNode) {
                break;
            }
            unvisited.remove(current);
            // Finding the way
            for (auto& direction : directions) {
                Node nextPos(direction + current.position);

                if (map.getIfInDanger(nextPos.position) || nextPos.position.getX() > map_size - 1 ||
                    nextPos.position.getY() > map_size - 1 || nextPos.position.getX() < 0 ||
                    nextPos.position.getY() < 0 ||
                    std::find(visited.begin(), visited.end(), nextPos) != visited.end()) {
                    continue;
                }

                nextPos.calcPos(endPos);

                auto inOpen = std::find(unvisited.begin(), unvisited.end(), nextPos);
                if (inOpen == unvisited.end()) {
                    nextPos.parent = &visited.back();
                    nextPos.calcPos(endPos);
                    unvisited.push_back(nextPos);
                }
                else
                    if (nextPos.G < inOpen->G) nextPos.parent = &visited.back();
            }
        }
        // If unable to find the way
        if (unvisited.empty()) {
            std::cout << "Brak drogi\n";
        }
        // Road coordinates one by one
        std::_List_iterator<Node> inClosed = std::find(visited.begin(), visited.end(), goalNode);
        // The movement counter has the value 0 because zero movement is a ghost position
        int i_moveCounter = 0;
        // Swaps elements from the map to the road in case it needs to be displayed
        if (inClosed != visited.end() ) {
            while (*inClosed != startNode) {
                // Takes 1 movement to the next ghost position
                if(i_moveCounter==1){
                    map.setElement("WAY",inClosed->position);
                    // Downloads way position
                    i_posToGo[0]=inClosed->position.getX();
                    i_posToGo[1]=inClosed->position.getY();
                }

                *inClosed = *inClosed->parent;
                i_moveCounter+=1;
            }
        }

        //map.display();
        return i_posToGo;
    }
};
