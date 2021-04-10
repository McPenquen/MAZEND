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
vector<unique_ptr<RectangleShape>> LevelSystem::_sprites;

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

map<LevelSystem::TILE, vector<Vector2ul>> LevelSystem::_tile_positions;

void LevelSystem::addTilePosition(TILE tile, Vector2ul pos) 
{
    if (_tile_positions.find(tile) == _tile_positions.end()) 
    {
        vector<Vector2ul> oneList;
        oneList.push_back(pos);
        _tile_positions.insert({ tile, oneList });
    }
    else 
    {
        _tile_positions[tile].push_back(pos);
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
    for (int i = 0; i < buffer.size(); ++i) 
    {
        const char c = buffer[i];
        Vector2ul ulPos = w == 0 ? Vector2ul(i, 0) : Vector2ul(i - ((w+1)*h), h);
        switch (c) 
        {
            case '1':
                addTilePosition(TILE::EMPTY, ulPos);
                temp_tiles.push_back(EMPTY);
                level = 1;
                break;
            case '2':
                addTilePosition(TILE::EMPTY, ulPos);
                temp_tiles.push_back(EMPTY);
                level = 2;
                break;
            case '3':
                addTilePosition(TILE::EMPTY, ulPos);
                temp_tiles.push_back(EMPTY);
                level = 3;
                break;

                if (level == 3)
                {
 
                case '-':
                    addTilePosition(TILE::TOPHORIZONTAL, ulPos);
                    temp_tiles.push_back(TOPHORIZONTAL);
                    break;
                case '|':
                    addTilePosition(TILE::TOPVERTICAL, ulPos);
                    temp_tiles.push_back(TOPVERTICAL);
                    break;
                case '/':
                    addTilePosition(TILE::TOPCORNERLEFT, ulPos); //left to right turn 
                    temp_tiles.push_back(TOPCORNERLEFT);
                    break;
                case '�':
                    addTilePosition(TILE::TOPCORNERRIGHT, ulPos);//left to right turn
                    temp_tiles.push_back(TOPCORNERRIGHT);
                    break;
                case ']':
                    addTilePosition(TILE::TOPCORNERUP, ulPos);//left to right turn
                    temp_tiles.push_back(TOPCORNERUP);
                    break;
                case 'L':
                    addTilePosition(TILE::TOPCORNERDOWN, ulPos);//left to right turn
                    temp_tiles.push_back(TOPCORNERDOWN);
                    break;
                case '^':
                    addTilePosition(TILE::TTUP, ulPos);//left to right turn
                    temp_tiles.push_back(TTUP);
                    break;
                case '<':
                    addTilePosition(TILE::TTLEFT, ulPos);//left to right turn
                    temp_tiles.push_back(TTLEFT);
                    break;
                case 'V':
                    addTilePosition(TILE::TTDOWN, ulPos);//left to right turn
                    temp_tiles.push_back(TTDOWN);
                    break;
                case '>':
                    addTilePosition(TILE::TTRIGHT, ulPos);//left to right turn
                    temp_tiles.push_back(TTRIGHT);
                    break;
                case '+':
                    addTilePosition(TILE::TXJUNCTION, ulPos); //cross section
                    temp_tiles.push_back(TXJUNCTION);
                    break;
                case 'D':
                    addTilePosition(TILE::TOPSTAIRDOWN, ulPos);
                    temp_tiles.push_back(TOPSTAIRDOWN);
                    break;
                case 'U':
                    addTilePosition(TILE::TOPSTAIRUP, ulPos);
                    temp_tiles.push_back(TOPSTAIRUP);
                    break;
                case 'R':
                    addTilePosition(TILE::TOPSTAIRRIGHT, ulPos);
                    temp_tiles.push_back(TOPSTAIRRIGHT);
                    break;
                case 'C':
                    addTilePosition(TILE::TOPSTAIRLEFT, ulPos);
                    temp_tiles.push_back(TOPSTAIRLEFT);
                    break;
                case ' ':
                    addTilePosition(TILE::EMPTY, ulPos);
                    temp_tiles.push_back(EMPTY);
                    break;
                case '\n':
                    if (w == 0) {
                        w = i;
                    }
                    h++;
                    break;
                default:
                    std::cout << i << " - Unknown tile: " << c << endl;
                }
        }
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
    buildSprites();
}

void LevelSystem::buildSprites() 
{
    if (!spriteSheet.loadFromFile("res/sprites/spriteSheet.png"))
    {
        cout << "ERROR" << endl;
    }
    _sprites.clear();
    for (size_t y = 0; y < LevelSystem::getHeight(); ++y) 
    {
        for (size_t x = 0; x < LevelSystem::getWidth(); ++x) 
        {
            auto s = make_unique<RectangleShape>();
            s->setPosition(getTilePosition({x, y}));
            s->setSize(Vector2f(_tileSize, _tileSize));
            s->setTexture(&spriteSheet);
            
            float g = getTexture(getTile({ x, y })).x;
            float p = getTexture(getTile({ x, y })).y;
            s->setTextureRect(IntRect(g, p ,64,64));
            //s->setFillColor(getColor(getTile({x, y})));
            _sprites.push_back(move(s));
        }
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

void LevelSystem::Render(RenderWindow &window) 
{
    for (size_t i = 0; i < _width * _height; ++i) 
    {
        window.draw(*_sprites[i]);
    }
}

vector<Vector2ul> LevelSystem::findTiles(TILE tile) 
{
    return _tile_positions[tile];
}
