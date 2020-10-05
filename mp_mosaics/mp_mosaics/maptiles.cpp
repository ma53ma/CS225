/**
 * @file maptiles.cpp
 * Code for the maptiles function.
 */

#include <iostream>
#include <map>
#include "maptiles.h"
//#include "cs225/RGB_HSL.h"

using namespace std;


Point<3> convertToXYZ(LUVAPixel pixel) {
    return Point<3>( pixel.l, pixel.u, pixel.v );
}

MosaicCanvas* mapTiles(SourceImage const& theSource,
                       vector<TileImage>& theTiles)
{
     if (theTiles.size() == 0) {
       return NULL;
     }
     MosaicCanvas * mosaic = new MosaicCanvas(theSource.getRows(), theSource.getColumns());
     vector<Point<3>> tileVector;
     map<Point<3>, TileImage *> tileMap;

     for (TileImage& tile : theTiles) { //making a vector to make the tree from and making the map to map avg distance to tile
       tileMap.insert({convertToXYZ(tile.getAverageColor()), &tile});
       tileVector.push_back(convertToXYZ(tile.getAverageColor()));
     }

     KDTree<3> tileTree(tileVector); // making a tree from the tiles
     tileTree.printTree();


     for (int x = 0; x < mosaic->getRows(); x++) {
       for (int y = 0; y < mosaic->getColumns(); y++) {
         LUVAPixel sourcePixel = theSource.getRegionColor(x,y); //getting the pixel at the source
         Point<3> sourcePoint = convertToXYZ(sourcePixel); //converting source pixel to point
         Point<3> closestPoint = tileTree.findNearestNeighbor(sourcePoint); //finding the nearest point in tree
         mosaic->setTile(x, y, tileMap.at(closestPoint)); // adding tile to mosaic
       }
     }

     return mosaic;
}
