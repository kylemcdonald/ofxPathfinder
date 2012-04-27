# ofxPathfinder is an addon for [openFrameworks](http://openframeworks.cc/) 

ofxPathfinder implements the A* pathfinding algorithm very efficiently using STL.

## Usage

First, set up the world with an image using `setup()`. Black pixels are boundaries/walls, and darker pixels are slower than brighter pixels. If you don't have variable terrain cost, just use black and white.

After setting up the world you can make multiple calls to `find()` with starting and ending `x, y` coordinates. `find()` will return when it's done looking for a path, and the result will be stored in the `path` variable, an `ofPolyline` object.

## Technical Notes

This implementation is optimized for a single world with multiple pathfinding lookups, with some consideration for the case where a world is regularly changing. This code could easily be optimized for a very large, sparse world or one that is constantly changing, by moving the `setPosition()`/`setCost()` calls to immediately after the first `getTile()` inside `consider()`. This means the positions and costs of the tiles would be computed as necessary rather than assigned during the first `setup()`.

## License

This code is open source, under the MIT license.

## Reference

Based on ideas from this wonderful [A* tutorial](http://www.policyalmanac.org/games/aStarTutorial.htm). The efficiency of the implementation is heavily dependent on the fact that the [STL set](http://www.cplusplus.com/reference/stl/set/) is always sorted. For other openFrameworks addons dealing with pathfinding, see [ofxPathfinding](https://github.com/benMcChesney/ofxPathfinding) and [ofxAStar](https://github.com/paulobarcelos/ofxAStar).