#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <string>
#include <vector>
#include "maths.h"

#define LS LevelSystem

using namespace std;
using namespace sf;

class LevelSystem {
public:
    enum TILE {EMPTY, TOPHORIZONTAL, TOPVERTICAL, TOPSTAIRUP, TOPSTAIRDOWN, TOPSTAIRRIGHT, TOPSTAIRLEFT, TOPCORNERLEFT, TOPCORNERRIGHT, TOPCORNERUP, TOPCORNERDOWN, TTUP, TTDOWN, TTLEFT, TTRIGHT, TXJUNCTION
       , MIDHORIZONTAL, MIDVERTICAL, MIDSTAIRUP, MIDSTAIRDOWN, MIDSTAIRRIGHT, MIDSTAIRLEFT, MIDCORNERLEFT, MIDCORNERRIGHT, MIDCORNERUP, MIDCORNERDOWN, MTUP, MTDOWN, MTLEFT, MTRIGHT, MXJUNCTION,
        BOTHORIZONTAL, BOTVERTICAL, BOTSTAIRUP, BOTSTAIRDOWN, BOTSTAIRRIGHT, BOTSTAIRLEFT, BOTCORNERLEFT, BOTCORNERRIGHT, BOTCORNERUP, BOTCORNERDOWN, BTUP, BTDOWN, BTLEFT, BTRIGHT, BXJUNCTION};

    static void loadLevelFile(const string &, float tileSize=100.f);
    static void Render(RenderWindow &window);
    static void UnLoad();

    static Color getColor(TILE t);
    static void setColor(TILE t, Color c);

    static Vector2f getTexture(TILE t);
   // static void setTexture(TILE t, Texture x);

    static TILE getTile(Vector2ul); // get tile at grid coordinate
    static Vector2f getTilePosition(Vector2ul); // get screenspace coordinate of tile 
    static Vector2f getTileOrigin(Vector2ul);
    static TILE getTileAt(Vector2f); // get tile at screenspace pos

    static size_t getHeight();
    static size_t getWidth();

    static vector<Vector2ul> findTiles(TILE);

    static float getTileSize();

    static void buildSprites();

protected:
    static unique_ptr<TILE[]> _tiles; // internal array of tiles
    static size_t _width; // how many tiles the level is wide
    static size_t _height; // how many tiles the level is high
    static Vector2f _offset; // screenspace offset of level, when rendered
    static float _tileSize; // screenspace size of each tile, when rendered
    static map<TILE, Color> _colours; // color to render each tile type
    static map<TILE, Vector2f> _textures; // what sprite should be used
    static vector<unique_ptr<RectangleShape>> _sprites; // array of sfml sprites of each tile
    
    static void addTilePosition(TILE, Vector2ul);
    static map<TILE, vector<Vector2ul>> _tile_positions;
private:
    LevelSystem() = delete;
    ~LevelSystem() = delete;
};
