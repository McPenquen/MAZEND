#include "LevelSystem.h"
#include <fstream>
#include <iostream>
#include <memory>

using namespace std;
using namespace sf;

unique_ptr<LevelSystem::TILE[]> LevelSystem::_tiles;
size_t LevelSystem::_width;
size_t LevelSystem::_height;
Vector2f LevelSystem::_offset(0.0f, 0.0f);

float LevelSystem::_tileSize(100.f);
vector<map<int,vector<shared_ptr<RectangleShape>>>> LevelSystem::_sprites;

map<LevelSystem::TILE, Color> LevelSystem::_colours{ {TOPHORIZONTAL, Color::Blue },{TOPVERTICAL, Color::Blue} };
Texture spriteSheet;

map<LevelSystem::TILE, Vector2f> LevelSystem::_textures{ 
    {TOPHORIZONTAL, Vector2f(192, 192)}, {TOPVERTICAL,Vector2f(128, 192)},
    {TOPSTAIRUP,Vector2f(0, 64)},{TOPSTAIRDOWN,Vector2f(64, 0)},
    {TOPSTAIRLEFT,Vector2f(128, 0)},{TOPSTAIRRIGHT,Vector2f(192, 0)},
    {TOPCORNERLEFT,Vector2f(0, 128)},{TOPCORNERRIGHT,Vector2f(64, 128)},
    {TOPCORNERUP,Vector2f(64, 192)},{TOPCORNERDOWN,Vector2f(0, 192)},
    {TTUP,Vector2f(192, 128)},{TTDOWN,Vector2f(128, 64)},
    {TTLEFT,Vector2f(128, 64)},{TTRIGHT,Vector2f(128, 128)},
    {TXJUNCTION,Vector2f(256, 64)} 
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

void LevelSystem::addTilePosition(TILE tile, Vector2ul pos, int levelNum, Vector2i sectorId) 
{
    if (_tile_positions[levelNum][getIntSectorId(sectorId)].find(tile) == _tile_positions[levelNum][getIntSectorId(sectorId)].end())
    {
        vector<Vector2ul> oneList;
        oneList.push_back(pos);
        _tile_positions[levelNum][getIntSectorId(sectorId)].insert({ tile, oneList });
    }
    else 
    {
        _tile_positions[levelNum][getIntSectorId(sectorId)][tile].push_back(pos);
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
        //map<int, vector<unique_ptr<RectangleShape>>> m;
        //_sprites.push_back({});
        //_sprites.resize(3);

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
        throw string("Couldn't open lvel file:") + path;
    }

    vector<TILE> temp_tiles;
    // Vector2i sector id generator
    Vector2i sectorId = { 1, 1 };
    int sectorXswitch = 1;
    int sectorYswitch = 1;

    for (int i = 0; i < buffer.size(); ++i) 
    {
        const char c = buffer[i];
        Vector2ul ulPos = w == 0 ? Vector2ul(i, 0) : Vector2ul(i - ((w+1)*h), h);
        switch (c) 
        {
            case '1':
                addTilePosition(TILE::EMPTY, ulPos, 0, sectorId);
                temp_tiles.push_back(EMPTY);
                level = 1;
                break;
            case '2':
                addTilePosition(TILE::EMPTY, ulPos, 1, sectorId);
                temp_tiles.push_back(EMPTY);
                level = 2;
                break;
            case '3':
                addTilePosition(TILE::EMPTY, ulPos, 2, sectorId);
                temp_tiles.push_back(EMPTY);
                level = 3;
                break;

                if (level == 3)
                {
 
                case '-':
                    addTilePosition(TILE::TOPHORIZONTAL, ulPos, level - 1, sectorId);
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
                case '¬':
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
                    if (w == 0) {
                        w = i;
                    }
                    h++;
                    // Update the sector Id generating Y value
                    if (sectorYswitch == 3) {
                        sectorYswitch = 0;
                        sectorId.y++;
                    }
                    // Reset X value
                    sectorXswitch = 0;
                    sectorId.x = 1;
                    sectorYswitch++;
                    break;
                default:
                    std::cout << i << " - Unknown tile: " << c << endl;
                }
        }
        // Update sector Id generating X value
        if (sectorXswitch == 3) {
            sectorXswitch = 0;
            sectorId.x++;
        }
        sectorXswitch++;
        
    }
    if (temp_tiles.size() != (w*h)) 
    {
        throw string("Can't parse level file");
    }
    _tiles = std::make_unique<TILE[]>(w*h);
    _width = w;
    _height = h;
    std::copy(temp_tiles.begin(), temp_tiles.end(), &_tiles[0]);
    std::cout << "Level " << path << " loaded. " << w << "x" << h << endl;
    buildSprites(level - 1);
}

void LevelSystem::buildSprites(int levelNum) 
{
    if (!spriteSheet.loadFromFile("res/sprites/spriteSheet.png"))
    {
        cout << "ERROR" << endl;
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
            auto s = make_shared<RectangleShape>();
            s->setPosition(getTilePosition({x, y}));
            s->setSize(Vector2f(_tileSize, _tileSize));
            s->setTexture(&spriteSheet);
            
            float g = getTexture(getTile({ x, y })).x;
            float p = getTexture(getTile({ x, y })).y;
            s->setTextureRect(IntRect(g, p ,64,64));
            //s->setFillColor(getColor(getTile({x, y})));
            _sprites[levelNum][getIntSectorId(sectorId)].push_back(move(s));
            
            // Update sector Id X counter
            if (sectorXswitch==3) {
                sectorXswitch = 0;
                sectorId.x++;
            }
            sectorXswitch++;
        }
        // Update sector Id Y counter
        if (sectorYswitch == 3) {
            sectorYswitch = 0;
            sectorId.y++;
        }
        sectorYswitch++;
        sectorId.x = 1;
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

LevelSystem::TILE LevelSystem::getTile(Vector2ul p) 
{
    if (p.x > _width || p.y > _height) 
    {
         throw string("Tile out of range ") + to_string(p.x) + ", " + to_string(p.y) + ")";
    }
    return _tiles[(p.y * _width) + p.x];
}

LevelSystem::TILE LevelSystem::getTileAt(Vector2f v) 
{
    auto a = v - _offset;
    if (a.x < 0 || a.y < 0) 
    {
        throw string("tile out of range");
    }
    return getTile(Vector2ul((v - _offset) / (_tileSize)));
}

void LevelSystem::Render(RenderWindow& window)
{/*
    if (_sprites.size() > 0) {
        if (_sprites[0].size() > 0) {
            for (size_t i = 0; i < _width * _height; ++i)
            {
                window.draw(*_sprites[0][i]);
            }
        }
        if (_sprites[1].size() > 0) {
            for (size_t i = 0; i < _width * _height; ++i)
            {
                window.draw(*_sprites[1][i]);
            }
        }
        if (_sprites[2].size() > 0) {
            for (size_t i = 0; i < _width * _height; ++i)
            {
                window.draw(*_sprites[2][i]);
            }
        }
    }*/
}

void LevelSystem::Render(RenderWindow& window, int floor, Vector2i sectorId) {
    if (_sprites.size() > 0) {
        if (_sprites[floor].size() > 0) {
            for (size_t i = 0; i < _width * _height; ++i) {
                window.draw(*_sprites[floor][getIntSectorId(sectorId)][i]);
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
    _tiles.reset();
    _width = 0;
    _height = 0;
    _offset = { 0, 0 };
}

void LevelSystem::SetOffset(const Vector2f& new_offset) {
    _offset = new_offset;
}