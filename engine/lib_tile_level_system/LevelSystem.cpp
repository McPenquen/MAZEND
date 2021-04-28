#include "LevelSystem.h"
#include <fstream>
#include <iostream>
#include <memory>
#include "../system_renderer.h"
//Size of one sector
#define sectorTilesNumber 10

using namespace std;
using namespace sf;

vector<unique_ptr<LevelSystem::TILE[]>> LevelSystem::_tiles;
size_t LevelSystem::_width;
size_t LevelSystem::_height;
Vector2f LevelSystem::_offset(0.0f, 0.0f);

float LevelSystem::_tileSize(100.f);
vector<map<int,vector<shared_ptr<RectangleShape>>>> LevelSystem::_sprites;
vector<LevelSystem::TILE> LevelSystem::_stairs = { TOPSTAIRUP, TOPSTAIRDOWN, TOPSTAIRRIGHT, TOPSTAIRLEFT,
        MIDSTAIRUP, MIDSTAIRDOWN, MIDSTAIRRIGHT, MIDSTAIRLEFT,
        BOTSTAIRUP, BOTSTAIRDOWN, BOTSTAIRRIGHT, BOTSTAIRLEFT };

map<LevelSystem::TILE, Color> LevelSystem::_colours{ {TOPHORIZONTAL, Color::Blue },{TOPVERTICAL, Color::Blue} };
Texture topspriteSheet;
Texture midspriteSheet;
Texture botspriteSheet;
map<LevelSystem::TILE, Vector2f> LevelSystem::_textures{ 
    {TOPHORIZONTAL, Vector2f(192, 192)}, {TOPVERTICAL,Vector2f(128, 192)},
    {TOPSTAIRUP,Vector2f(0, 64)},{TOPSTAIRDOWN,Vector2f(64, 0)},
    {TOPSTAIRLEFT,Vector2f(128, 0)},{TOPSTAIRRIGHT,Vector2f(192, 0)},
    {TOPCORNERLEFT,Vector2f(0, 128)},{TOPCORNERRIGHT,Vector2f(64, 128)},
    {TOPCORNERUP,Vector2f(64, 192)},{TOPCORNERDOWN,Vector2f(0, 192)},
    {TTUP,Vector2f(192, 128)},{TTDOWN,Vector2f(128, 64)},
    {TTLEFT,Vector2f(192, 64)},{TTRIGHT,Vector2f(128, 128)},
    {TXJUNCTION,Vector2f(64, 64)} , {MIDHORIZONTAL, Vector2f(192, 192)}, {MIDVERTICAL,Vector2f(128, 192)},
    {MIDSTAIRUP,Vector2f(0, 64)},{MIDSTAIRDOWN,Vector2f(64, 0)},
    {MIDSTAIRLEFT,Vector2f(128, 0)},{MIDSTAIRRIGHT,Vector2f(192, 0)},
    {MIDCORNERLEFT,Vector2f(0, 128)},{MIDCORNERRIGHT,Vector2f(64, 128)},
    {MIDCORNERUP,Vector2f(64, 192)},{MIDCORNERDOWN,Vector2f(0, 192)},
    {MTUP,Vector2f(192, 128)},{MTDOWN,Vector2f(128, 64)},
    {MTLEFT,Vector2f(192, 64)},{MTRIGHT,Vector2f(128, 128)},
    {MXJUNCTION,Vector2f(64, 64)}, {BOTHORIZONTAL, Vector2f(192, 192)}, {BOTVERTICAL,Vector2f(128, 192)},
    {BOTSTAIRUP,Vector2f(0, 0)},{BOTSTAIRDOWN,Vector2f(0, 0)},
    {BOTSTAIRLEFT,Vector2f(0, 0)},{BOTSTAIRRIGHT,Vector2f(0, 0)},
    {BOTCORNERLEFT,Vector2f(0, 128)},{BOTCORNERRIGHT,Vector2f(64, 128)},
    {BOTCORNERUP,Vector2f(64, 192)},{BOTCORNERDOWN,Vector2f(0, 192)},
    {BTUP,Vector2f(192, 128)},{BTDOWN,Vector2f(128, 64)},
    {BTLEFT,Vector2f(128, 64)},{BTRIGHT,Vector2f(128, 128)},
    {BXJUNCTION,Vector2f(64, 64)}
};


int level = 0;


size_t LevelSystem::getHeight() {return _height;}
size_t LevelSystem::getWidth() {return _width;}

vector<map<int,map<LevelSystem::TILE, vector<Vector2ul>>>> LevelSystem::_tile_positions;

// Helper function to get int sector id from Vector2i sector id
int getIntSectorId(Vector2i vecID) {
    int answer = vecID.x * 10 + vecID.y;
    return answer;
}

// Helper function to get the uniform location of the sprites (starting at {0,0})
Vector2ul getNormalisedSectorPositions(Vector2ul pos, Vector2i sectorID, float posPadding) {
    Vector2ul answer = pos;
    if (sectorID.x == 2) {
        answer.x -= float(sectorTilesNumber) * posPadding;
    }
    if (sectorID.x == 3) {
        answer.x -= 2 * float(sectorTilesNumber) * posPadding;
    }
    if (sectorID.y == 2) {
        answer.y -= float(sectorTilesNumber) * posPadding;
    }
    if (sectorID.y == 3) {
        answer.y -= 2 * float(sectorTilesNumber) * posPadding;
    }
    return answer;
}

void LevelSystem::addTilePosition(TILE tile, Vector2ul pos, int levelNum, Vector2i sectorId) 
{
    Vector2ul screenPos = getNormalisedSectorPositions(pos, sectorId, getTileSize());

    if (_tile_positions[levelNum][getIntSectorId(sectorId)].find(tile) == _tile_positions[levelNum][getIntSectorId(sectorId)].end())
    {
        vector<Vector2ul> oneList;
        oneList.push_back(screenPos);
        _tile_positions[levelNum][getIntSectorId(sectorId)].insert({ tile, oneList });
    }
    else 
    {
        _tile_positions[levelNum][getIntSectorId(sectorId)][tile].push_back(screenPos);
    }

}

Color LevelSystem::getColor(LevelSystem::TILE t) 
{
    auto it = _colours.find(t);
    if (it == _colours.end()) {
        _colours[t] = Color::Transparent;
    }
    return _colours[t];
}

void LevelSystem::setColor(LevelSystem::TILE t, Color c) 
{
    _colours.insert({t, c});
}

Vector2f LevelSystem::getTexture(LevelSystem::TILE t)
{
    _textures.find(t);

    return _textures[t];
}

float LevelSystem::getTileSize() 
{
    return _tileSize;
}

void LevelSystem::loadLevelFile(const string &path, float tileSize) 
{
    // If sprites are empty initialise first 3 empty vectors and di the same for _tile_positions
    if (_sprites.size() == 0) {
        for (int i = 0; i < 3; i++) {
            map<int, vector<shared_ptr<RectangleShape>>> m;
            auto s = make_shared<RectangleShape>();
            m[44].push_back(move(s));
            _sprites.push_back(m);

            _tile_positions.push_back(map<int, map<TILE, vector<Vector2ul>>>());
        }
    }

    _tileSize = tileSize;
    size_t w = 0, h = 0;
    string buffer;

    //Load in file
    ifstream f(path);
    if (f.good()) {
        f.seekg(0, ios::end);
        buffer.resize(f.tellg());
        f.seekg(0);
        f.read(&buffer[0], buffer.size());
        f.close();
    }
    else 
    {
        throw string("Couldn't open level file:") + path;
    }

    vector<TILE> temp_tiles;
    // Vector2i sector id generator
    Vector2i sectorId = { 1, 1 };
    int sectorXswitch = 1;
    int sectorYswitch = 1;
    bool unknownTile = false;
    int realI = 0;

    for (int i = 0; i < buffer.size(); ++i) 
    {
        unknownTile = false;
        const char c = buffer[i];
        Vector2ul ulPos = w == 0 ? Vector2ul(realI, 0) : Vector2ul(realI - ((w+1)*h), h);
        if (i == 0)

        {
            switch (c)
            {
            case'1':
                level = 1;
                break;
            case'2':
                level = 2;
                break;
            case'3':
                level = 3;
                break;
            }
        }
        if (level == 1)
        {
            switch (c)
            {
            case '1':
                addTilePosition(TILE::EMPTY, ulPos, 0, sectorId);
                temp_tiles.push_back(EMPTY);
                break;
            case '-':
                addTilePosition(TILE::BOTHORIZONTAL, ulPos, level - 1, sectorId);
                temp_tiles.push_back(BOTHORIZONTAL);
                break;
            case '|':
                addTilePosition(TILE::BOTVERTICAL, ulPos, level - 1, sectorId);
                temp_tiles.push_back(BOTVERTICAL);
                break;
            case '/':
                addTilePosition(TILE::BOTCORNERLEFT, ulPos, level - 1, sectorId); //left to right turn 
                temp_tiles.push_back(BOTCORNERLEFT);
                break;
            case '~':
                addTilePosition(TILE::BOTCORNERRIGHT, ulPos, level - 1, sectorId);//left to right turn
                temp_tiles.push_back(BOTCORNERRIGHT);
                break;
            case ']':
                addTilePosition(TILE::BOTCORNERUP, ulPos, level - 1, sectorId);//left to right turn
                temp_tiles.push_back(BOTCORNERUP);
                break;
            case 'L':
                addTilePosition(TILE::BOTCORNERDOWN, ulPos, level - 1, sectorId);//left to right turn
                temp_tiles.push_back(BOTCORNERDOWN);
                break;
            case '^':
                addTilePosition(TILE::BTUP, ulPos, level - 1, sectorId);//left to right turn
                temp_tiles.push_back(TTUP);
                break;
            case '<':
                addTilePosition(TILE::BTLEFT, ulPos, level - 1, sectorId);//left to right turn
                temp_tiles.push_back(TTLEFT);
                break;
            case 'V':
                addTilePosition(TILE::BTDOWN, ulPos, level - 1, sectorId);//left to right turn
                temp_tiles.push_back(TTDOWN);
                break;
            case '>':
                addTilePosition(TILE::BTRIGHT, ulPos, level - 1, sectorId);//left to right turn
                temp_tiles.push_back(TTRIGHT);
                break;
            case '+':
                addTilePosition(TILE::BXJUNCTION, ulPos, level - 1, sectorId); //cross section
                temp_tiles.push_back(TXJUNCTION);
                break;
            case 'D':
                addTilePosition(TILE::BOTSTAIRDOWN, ulPos, level - 1, sectorId);
                temp_tiles.push_back(BOTSTAIRDOWN);
                break;
            case 'U':
                addTilePosition(TILE::BOTSTAIRUP, ulPos, level - 1, sectorId);
                temp_tiles.push_back(BOTSTAIRUP);
                break;
            case 'R':
                addTilePosition(TILE::BOTSTAIRRIGHT, ulPos, level - 1, sectorId);
                temp_tiles.push_back(BOTSTAIRRIGHT);
                break;
            case 'C':
                addTilePosition(TILE::BOTSTAIRLEFT, ulPos, level - 1, sectorId);
                temp_tiles.push_back(BOTSTAIRLEFT);
                break;
            case ' ':
                addTilePosition(TILE::EMPTY, ulPos, level - 1, sectorId);
                temp_tiles.push_back(EMPTY);
                break;
            case '\n':
                if (w == 0)
                {
                    w = i;
                }
                h++;
                // Update the sector Id generating Y value
                if (sectorYswitch == sectorTilesNumber)
                {
                    sectorYswitch = 0;
                    sectorId.y++;
                }
                // Reset X value
                sectorXswitch = 0;
                sectorId.x = 1;
                sectorYswitch++;
                break;
            default:
                unknownTile = true;
                realI--;
                std::cout << i << " - Unknown tile: " << c << endl;

            }
            if (!unknownTile)
            {
                // Update sector Id generating X value
                if (sectorXswitch == sectorTilesNumber)
                {
                    sectorXswitch = 0;
                    sectorId.x++;
                }
                sectorXswitch++;
                realI++;
            }
        }
        if (level == 2)
        {
            switch (c)
            {
            case '2':
                addTilePosition(TILE::EMPTY, ulPos, 1, sectorId);
                temp_tiles.push_back(EMPTY);
                break;
            case '-':
                addTilePosition(TILE::MIDHORIZONTAL, ulPos, level - 1, sectorId);
                temp_tiles.push_back(MIDHORIZONTAL);
                break;
            case '|':
                addTilePosition(TILE::MIDVERTICAL, ulPos, level - 1, sectorId);
                temp_tiles.push_back(MIDVERTICAL);
                break;
            case '/':
                addTilePosition(TILE::MIDCORNERLEFT, ulPos, level - 1, sectorId); //left to right turn 
                temp_tiles.push_back(MIDCORNERLEFT);
                break;
            case '~':
                addTilePosition(TILE::MIDCORNERRIGHT, ulPos, level - 1, sectorId);//left to right turn
                temp_tiles.push_back(MIDCORNERRIGHT);
                break;
            case ']':
                addTilePosition(TILE::MIDCORNERUP, ulPos, level - 1, sectorId);//left to right turn
                temp_tiles.push_back(MIDCORNERUP);
                break;
            case 'L':
                addTilePosition(TILE::MIDCORNERDOWN, ulPos, level - 1, sectorId);//left to right turn
                temp_tiles.push_back(MIDCORNERDOWN);
                break;
            case '^':
                addTilePosition(TILE::MTUP, ulPos, level - 1, sectorId);//left to right turn
                temp_tiles.push_back(MTUP);
                break;
            case '<':
                addTilePosition(TILE::MTLEFT, ulPos, level - 1, sectorId);//left to right turn
                temp_tiles.push_back(MTLEFT);
                break;
            case 'V':
                addTilePosition(TILE::MTDOWN, ulPos, level - 1, sectorId);//left to right turn
                temp_tiles.push_back(MTDOWN);
                break;
            case '>':
                addTilePosition(TILE::MTRIGHT, ulPos, level - 1, sectorId);//left to right turn
                temp_tiles.push_back(MTRIGHT);
                break;
            case '+':
                addTilePosition(TILE::MXJUNCTION, ulPos, level - 1, sectorId); //cross section
                temp_tiles.push_back(MXJUNCTION);
                break;
            case 'D':
                addTilePosition(TILE::MIDSTAIRDOWN, ulPos, level - 1, sectorId);
                temp_tiles.push_back(MIDSTAIRDOWN);
                break;
            case 'U':
                addTilePosition(TILE::MIDSTAIRUP, ulPos, level - 1, sectorId);
                temp_tiles.push_back(MIDSTAIRUP);
                break;
            case 'R':
                addTilePosition(TILE::MIDSTAIRRIGHT, ulPos, level - 1, sectorId);
                temp_tiles.push_back(MIDSTAIRRIGHT);
                break;
            case 'C':
                addTilePosition(TILE::MIDSTAIRLEFT, ulPos, level - 1, sectorId);
                temp_tiles.push_back(MIDSTAIRLEFT);
                break;
            case ' ':
                addTilePosition(TILE::EMPTY, ulPos, level - 1, sectorId);

                temp_tiles.push_back(EMPTY);
                break;
            case '\n':
                if (w == 0)
                {
                    w = i;
                }
                h++;
                // Update the sector Id generating Y value
                if (sectorYswitch == sectorTilesNumber)
                {
                    sectorYswitch = 0;
                    sectorId.y++;
                }
                // Reset X value
                sectorXswitch = 0;
                sectorId.x = 1;
                sectorYswitch++;
                break;
            default:
                unknownTile = true;
                realI--;
                std::cout << i << " - Unknown tile: " << c << endl;

            }
            if (!unknownTile)
            {
                // Update sector Id generating X value
                if (sectorXswitch == sectorTilesNumber)
                {
                    sectorXswitch = 0;
                    sectorId.x++;
                }
                sectorXswitch++;
                realI++;
            }
        }

        if (level == 3)
        {
            switch (c)
            {
            case '3':
                addTilePosition(TILE::EMPTY, ulPos, 2, sectorId);
                temp_tiles.push_back(EMPTY);
                break;
            case '-':
                addTilePosition(TILE::MIDHORIZONTAL, ulPos, level - 1, sectorId);
                temp_tiles.push_back(TOPHORIZONTAL);
                break;
            case '|':
                addTilePosition(TILE::TOPVERTICAL, ulPos, level - 1, sectorId);
                temp_tiles.push_back(TOPVERTICAL);
                break;
            case '/':
                addTilePosition(TILE::TOPCORNERLEFT, ulPos, level - 1, sectorId); //left to right turn 
                temp_tiles.push_back(TOPCORNERLEFT);
                break;
            case '~':
                addTilePosition(TILE::TOPCORNERRIGHT, ulPos, level - 1, sectorId);//left to right turn
                temp_tiles.push_back(TOPCORNERRIGHT);
                break;
            case ']':
                addTilePosition(TILE::TOPCORNERUP, ulPos, level - 1, sectorId);//left to right turn
                temp_tiles.push_back(TOPCORNERUP);
                break;
            case 'L':
                addTilePosition(TILE::TOPCORNERDOWN, ulPos, level - 1, sectorId);//left to right turn
                temp_tiles.push_back(TOPCORNERDOWN);
                break;
            case '^':
                addTilePosition(TILE::TTUP, ulPos, level - 1, sectorId);//left to right turn
                temp_tiles.push_back(TTUP);
                break;
            case '<':
                addTilePosition(TILE::TTLEFT, ulPos, level - 1, sectorId);//left to right turn
                temp_tiles.push_back(TTLEFT);
                break;
            case 'V':
                addTilePosition(TILE::TTDOWN, ulPos, level - 1, sectorId);//left to right turn
                temp_tiles.push_back(TTDOWN);
                break;
            case '>':
                addTilePosition(TILE::TTRIGHT, ulPos, level - 1, sectorId);//left to right turn
                temp_tiles.push_back(TTRIGHT);
                break;
            case '+':
                addTilePosition(TILE::TXJUNCTION, ulPos, level - 1, sectorId); //cross section
                temp_tiles.push_back(TXJUNCTION);
                break;
            case 'D':
                addTilePosition(TILE::TOPSTAIRDOWN, ulPos, level - 1, sectorId);
                temp_tiles.push_back(TOPSTAIRDOWN);
                break;
            case 'U':
                addTilePosition(TILE::TOPSTAIRUP, ulPos, level - 1, sectorId);
                temp_tiles.push_back(TOPSTAIRUP);
                break;
            case 'R':
                addTilePosition(TILE::TOPSTAIRRIGHT, ulPos, level - 1, sectorId);
                temp_tiles.push_back(TOPSTAIRRIGHT);
                break;
            case 'C':
                addTilePosition(TILE::TOPSTAIRLEFT, ulPos, level - 1, sectorId);
                temp_tiles.push_back(TOPSTAIRLEFT);
                break;
            case ' ':
                addTilePosition(TILE::EMPTY, ulPos, level - 1, sectorId);
                temp_tiles.push_back(EMPTY);
                break;
            case '\n':
                if (w == 0)
                {
                    w = i;
                }
                h++;
                // Update the sector Id generating Y value
                if (sectorYswitch == sectorTilesNumber)
                {
                    sectorYswitch = 0;
                    sectorId.y++;
                }
                // Reset X value
                sectorXswitch = 0;
                sectorId.x = 1;
                sectorYswitch++;
                break;
            default:
                unknownTile = true;
                realI--;
                std::cout << i << " - Unknown tile: " << c << endl;

            }
            if (!unknownTile)
            {
                // Update sector Id generating X value
                if (sectorXswitch == sectorTilesNumber)
                {
                    sectorXswitch = 0;
                    sectorId.x++;
                }
                sectorXswitch++;
                realI++;
            }
        }
    }
    if (temp_tiles.size() != (w*h)) 
    {
        throw string("Can't parse level file");
    }
    // Save tiles in the correct floors - bottom floor first, mid floor second, top floor 3rd
    _tiles.push_back(std::make_unique<TILE[]>(w*h));
    _width = w;
    _height = h;
    std::copy(temp_tiles.begin(), temp_tiles.end(), &_tiles[level - 1][0]);
    std::cout << "Level " << path << " loaded. " << w << "x" << h << endl;
    buildSprites(level - 1);
}

void LevelSystem::buildSprites(int levelNum) 
{
    if (level == 1)
    {
        botspriteSheet.loadFromFile("res/sprites/botspriteSheet.png");
    }
    if (level == 2)
    {
        midspriteSheet.loadFromFile("res/sprites/midspriteSheet.png");
    }
    if (level == 3)
    {
        topspriteSheet.loadFromFile("res/sprites/spriteSheet.png");
    }

    _sprites[levelNum].clear();

    // Vector2i sector id generator
    Vector2i sectorId = Vector2i(1, 1);
    int sectorXswitch = 1;
    int sectorYswitch = 1;

    for (size_t y = 0; y < LevelSystem::getHeight(); ++y) 
    {
        for (size_t x = 0; x < LevelSystem::getWidth(); ++x) 
        {
            Vector2ul normalisedPos = getNormalisedSectorPositions(Vector2ul(x, y), sectorId, 1.0f);
            Vector2f tilePos = getTilePosition(normalisedPos);
            // Centralise the position
            tilePos.x += _offset.x;
            tilePos.y += _offset.y;

            auto s = make_shared<RectangleShape>();
            s->setPosition(tilePos);
            s->setSize(Vector2f(_tileSize, _tileSize));
            if (level == 1)
            {
                s->setTexture(&botspriteSheet);
            }
            if (level == 2)
            {
                s->setTexture(&midspriteSheet);
            }
            if (level == 3)
            {
                s->setTexture(&topspriteSheet);
            }
            
            float g = getTexture(getTile({ x, y }, level)).x;
            float p = getTexture(getTile({ x, y }, level)).y;
            s->setTextureRect(IntRect(g, p ,64,64));
            //s->setFillColor(getColor(getTile({x, y})));
            _sprites[levelNum][getIntSectorId(sectorId)].push_back(move(s));
            
            // Update sector Id X counter
            if (sectorXswitch == sectorTilesNumber) {
                sectorXswitch = 0;
                sectorId.x++;
            }
            sectorXswitch++;
        }
        // Update sector Id Y counter
        if (sectorYswitch == sectorTilesNumber) {
            sectorYswitch = 0;
            sectorId.y++;
        }
        // Reset X value
        sectorXswitch = 1;
        sectorId.x = 1;
        sectorYswitch++;
    }
}

Vector2f LevelSystem::getTilePosition(Vector2ul p) 
{
    return (Vector2f(p.x, p.y)) * _tileSize;
}

Vector2f LevelSystem::getTileOrigin(Vector2ul p) 
{
    return (Vector2f(p.x, p.y)) * _tileSize + Vector2f(_tileSize, _tileSize) * 0.5f;
}

LevelSystem::TILE LevelSystem::getTile(Vector2ul p, int floor) 
{
    return _tiles[floor - 1][(p.y * _width) + p.x];
}

LevelSystem::TILE LevelSystem::getTileAt(Vector2f v, Vector2i sectorId, int floor) {
    auto a = v - _offset;
    if (sectorId.x == 2) {
        a.x += float(sectorTilesNumber) * _tileSize;
    }
    if (sectorId.x == 3) {
        a.x += 2 * float(sectorTilesNumber) * _tileSize;
    }
    if (sectorId.y == 2) {
        a.y += float(sectorTilesNumber) * _tileSize;
    }
    if (sectorId.y == 3) {
        a.y += 2 * float(sectorTilesNumber) * _tileSize;
    }

    return getTile(Vector2ul(a / (_tileSize)), floor);
}

void LevelSystem::Render(RenderWindow& window, int floor, Vector2i sectorId) {
    auto floorIndex = floor-1;
    if (_sprites.size() > 0) {
        if (_sprites[floorIndex].size() > 0) {
            for (size_t i = 0; i < sectorTilesNumber * sectorTilesNumber; ++i) {
                Drawable *d = _sprites[floorIndex][getIntSectorId(sectorId)][i].get();
                Renderer::Queue(d);
            }
        }
        else {
            throw string("Couldn't render sector: " + to_string(sectorId.x) + ", " + to_string(sectorId.y)
                + " of floor number " + to_string(floor));
        }
    }
}

vector<Vector2ul> LevelSystem::findTiles(TILE tile, int levelNum, Vector2i sectorId) 
{
    return _tile_positions[levelNum][getIntSectorId(sectorId)][tile];
}

void LevelSystem::UnLoad() {
    cout << "LevelSystem unloading" << endl;
    _sprites.clear();
    _tiles.clear();
    _width = 0;
    _height = 0;
    _offset = { 0, 0 };
}

void LevelSystem::SetOffset(const Vector2f& new_offset) {
    _offset = new_offset;
}

bool LevelSystem::isStairs(LS::TILE t) {
    for (int i = 0; i < _stairs.size(); ++i) {
        if (t == _stairs[i]) {
            return true;
        }
    }
    return false;
}

// When the player stands on stairs tile it returns the other floor's number or 0 if invalid
int LevelSystem::getStairsFloorChnage(Vector2f pos, Vector2i sectorId, int floor) {
    if (floor == 1) {
        return getTileAt(pos, sectorId, 2) == TILE::EMPTY ? 3 : 2;
    }
    if (floor == 2) {
        return getTileAt(pos, sectorId, 1) == TILE::EMPTY ? 3 : 1;
    }
    if (floor == 3) {
        return getTileAt(pos, sectorId, 1) == TILE::EMPTY ? 2 : 1;
    }
    return 0;
}

// Return the world's position
Vector2f LevelSystem::getGlobalPos(Vector2f screenPos, Vector2i sector) {
    Vector2f globalPos = screenPos - _offset;

    if (sector.x == 2) {
        globalPos.x += float(sectorTilesNumber) * _tileSize;
    }
    if (sector.x == 3) {
        globalPos.x += 2 * float(sectorTilesNumber) * _tileSize;
    }
    if (sector.y == 2) {
        globalPos.y += float(sectorTilesNumber) * _tileSize;
    }
    if (sector.y == 3) {
        globalPos.y += 2 * float(sectorTilesNumber) * _tileSize;
    }

    return globalPos;
}

WorldPosition LevelSystem::getLocalPos(Vector2f globalPos) {
    WorldPosition answr;
    answr.position = globalPos;
    answr.sectorId = Vector2i(1, 1);
    float oneSectorSize = float(sectorTilesNumber) * _tileSize;

    if (globalPos.x >= oneSectorSize) {
        answr.position.x -= oneSectorSize;
        answr.sectorId.x = 2;
        if (globalPos.x >= 2 * oneSectorSize) {
            answr.position.x -= oneSectorSize;
            answr.sectorId.x = 3;
        }
    }
    if (globalPos.y >= oneSectorSize) {
        answr.position.y -= oneSectorSize;
        answr.sectorId.y = 2;
        if (globalPos.y >= 2 * oneSectorSize) {
            answr.position.y -= oneSectorSize;
            answr.sectorId.y = 3;
        }
    }

    answr.position += _offset;
    return answr;
}