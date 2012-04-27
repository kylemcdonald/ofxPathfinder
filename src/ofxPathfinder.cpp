#include "ofxPathfinder.h"

// this implementation is optimized for a single map with multiple pathfinding
// lookups, with some consideration for the case where a map is regularly
// changing. this could be optimized for a very large, sparse map or one that
// is changing constantly, by moving the setPosition()/setCost() to right after
// the first getTile() inside consider().

// based on:
// http://www.policyalmanac.org/games/aStarTutorial.htm
// http://stackoverflow.com/questions/1517854/priority-queue-comparison-for-pointers
// http://www.cplusplus.com/reference/stl/priority_queue/
// http://www.cplusplus.com/reference/stl/set/

class Tile {
public:
	Tile() : total(0), known(0), estimate(0), cost(1) {
	}
	
	float getTotal() const {
		return total;
	}
	float getKnown() const {
		return known;
	}
	float getCost() const {
		return cost;
	}
	ofVec2f getPosition() const {
		return ofVec2f(x, y);
	}
	void getNeighbor(int& x, int& y) const {
		x += this->x, y += this->y;
	}
	bool walkable() const {
		return cost >= 1;
	}

	void setKnown(float known) {
		this->known = known;
		total = known + estimate;
	}
	void setCost(float cost) { // not walkable < 1 <= walkable
		this->cost = cost;
	}
	void setPosition(int x, int y) {
		this->x = x, this->y = y;
	}
	void setEstimate(const TilePtr& target) {
		int dx = abs(target->x - x), dy = abs(target->y - y);
		this->estimate = abs(dx - dy) + sqrt(2) * MIN(dx, dy);
	}

	bool operator<(const Tile& tile) {
		if(getTotal() < tile.getTotal()) {
			return true;
		} else if(getTotal() > tile.getTotal()) {
			return false;
		}
		if(x < tile.x) {
			return true;
		} else if(x > tile.x) {
			return false;
		} else {
			return (y < tile.y);
		}
	}

	TilePtr parent;
protected:
	int x, y;
	float cost, estimate, known, total;
};

inline bool operator<(const TilePtr& a, const TilePtr& b) {
	return (*a) < (*b);
}

void ofxPathfinder::setup(ofImage& img) {
	setupTiles(img.getWidth(), img.getHeight());
	for(int y = 0; y < h; y++) {
		for(int x = 0; x < w; x++) {
			tiles.push_back(TilePtr(new Tile()));
			getTile(x, y)->setPosition(x, y);
			float brightness = img.getColor(x, y).getBrightness() / 255.;
			if(brightness == 0.) { // black tiles are not walkable
				getTile(x, y)->setCost(0);
			} else if(brightness < 1.) { // slow tiles (darker) have higher cost
				getTile(x, y)->setCost(1. / brightness); 
			}
		}
	}
}
void ofxPathfinder::find(int sourceX, int sourceY, int targetX, int targetY) {
	found = false;
	openSet.clear();
	closedSet.clear();
	
	setSource(sourceX, sourceY);
	setTarget(targetX, targetY);
	
	while(!found && !openSet.empty()) {
		TilePtr cur = *(openSet.begin());
		openSet.erase(cur);
		closedSet.insert(cur);
		if(cur == target) {
			finish();
		}
		consider(cur, -1, -1);
		consider(cur, +0, -1);
		consider(cur, +1, -1);
		consider(cur, -1, +0);
		consider(cur, +1, +0);
		consider(cur, -1, +1);
		consider(cur, +0, +1);
		consider(cur, +1, +1);
	}
}
void ofxPathfinder::setSource(int x, int y) {
	source = getTile(ofClamp(x, 0, w - 1), ofClamp(y, 0, h - 1));
	openSet.insert(source);
}
void ofxPathfinder::setTarget(int x, int y) {
	target = getTile(ofClamp(x, 0, w - 1), ofClamp(y, 0, h - 1));
}
void ofxPathfinder::setupTiles(int w, int h) {
	if(w != this->w || h != this->h) {
		this->w = w, this->h = h;		
		tiles.clear();
		for(int y = 0; y < h; y++) {
			for(int x = 0; x < w; x++) {
				tiles.push_back(TilePtr(new Tile()));
			}
		}
	}
}

TilePtr& ofxPathfinder::getTile(int x, int y) {
	return tiles[y * w + x];
}
bool ofxPathfinder::safe(int x, int y) const {
	return x >= 0 && y >= 0 && x < w && y < h;
}
void ofxPathfinder::finish() {
	found = true;
	TilePtr cur = target;
	path.clear();
	while(cur != source) {
		path.addVertex(cur->getPosition());
		cur = cur->parent;
	}
}
void ofxPathfinder::consider(const TilePtr& parent, int x, int y) {
	parent->getNeighbor(x, y);
	if(safe(x, y)) {
		TilePtr cur = getTile(x, y);
		if(!closedSet.count(cur) && cur->walkable()) {
			float cost = parent->getKnown() + parent->getCost();
			if(openSet.count(cur)) {
				if(cost < cur->getKnown()) {
					openSet.erase(cur);
					cur->parent = parent;
					cur->setKnown(cost);
					openSet.insert(cur);
				}
			} else {
				cur->setEstimate(target);
				cur->parent = parent;
				cur->setKnown(cost);
				openSet.insert(cur);
			}
		}
	}
}