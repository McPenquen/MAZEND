#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <string>
#include <vector>
#include "maths.h"

#define LS LevelSystem

using namespace std;
using namespace sf;

// Local position consists of the screen position and the sector id
struct WorldPosition {
    Vector2f position;
    Vector2i sectorId;
};

class LevelSystem {
public:
    enum TILE {EMPTY, TOPHORIZONTAL, TOPVERTICAL, TOPSTAIRUP, TOPSTAIRDOWN, TOPSTAIRRIGHT, TOPSTAIRLEFT, TOPCORNERLEFT, TOPCORNERRIGHT, TOPCORNERUP, TOPCORNERDOWN, TTUP, TTDOWN, TTLEFT, TTRIGHT, TXJUNCTION
       , MIDHORIZONTAL, MIDVERTICAL, MIDSTAIRUP, MIDSTAIRDOWN, MIDSTAIRRIGHT, MIDSTAIRLEFT, MIDCORNERLEFT, MIDCORNERRIGHT, MIDCORNERUP, MIDCORNERDOWN, MTUP, MTDOWN, MTLEFT, MTRIGHT, MXJUNCTION,
        BOTHORIZONTAL, BOTVERTICAL, BOTSTAIRUP, BOTSTAIRDOWN, BOTSTAIRRIGHT, BOTSTAIRLEFT, BOTCORNERLEFT, BOTCORNERRIGHT, BOTCORNERUP, BOTCORNERDOWN, BTUP, BTDOWN, BTLEFT, BTRIGHT, BXJUNCTION};


    static void loadLevelFile(const string &, float tileSize=100.f);
    static void Render(RenderWindow& window, int floor, Vector2i sectorId);
    static void UnLoad();

    static void SetOffset(const Vector2f& _offset); 

    static Vector2f getTexture(TILE t);

    static TILE getTile(Vector2ul, int); // get tile at grid coordinate of the floor
    static Vector2f getTilePosition(Vector2ul); // get screenspace coordinate of tile 
    static TILE getTileAt(Vector2f, Vector2i sectorId ,int floor); // get tile at screenspace pos

    static size_t getHeight();
    static size_t getWidth();

    static float getTileSize();

    static void buildSprites(int levelNum);

    static bool isStairs(TILE); //return if the tile is stairs
    static int getStairsFloorChnage(Vector2f, Vector2i, int); // return the layer number that the stairs lead to

    // Return the global position based on the sector and position
    static Vector2f getGlobalPos(Vector2f, Vector2i);
    // Return the sector and position based on the global position
    static WorldPosition getLocalPos(Vector2f);

protected:
    // Internal array of tiles, [0] - bot layer,[1] - mid layer, [2] - top layer
    static vector<unique_ptr<TILE[]>> _tiles;

    static size_t _width; // how many tiles the level is wide
    static size_t _height; // how many tiles the level is high
    static Vector2f _offset; // screenspace offset of level, when rendered
    static float _tileSize; // screenspace size of each tile, when rendered

    static map<TILE, Vector2f> _textures; // what sprite should be used;

    // Division into 3 layers vector[0]/[1]/[2], and each into sectoins with id of int version of the Vector2i id -> Vector2i(1,1)==int(11)
    static vector<map<int, vector<shared_ptr<RectangleShape>>>> _sprites; // array of sfml sprites of each tile
    
    // Tiles that are considered stairs
    static vector<TILE> _stairs;
    
private:
    LevelSystem() = delete;
    ~LevelSystem() = delete;
};
