#include <utility>

class Turtle
{
	std::pair<int,int> location = {0, 0};

public:
	void moveLeft(int distance) { location.first -= distance; }
	void moveRight(int distance) { location.first += distance; }
	void moveDown(int distance) { location.first -= distance; }
	void moveUp(int distance) { location.first += distance; }

	int getX() { return location.first; }
	int getY() { return location.second; }
	std::pair<int,int> getLocation() { return location; }
};
