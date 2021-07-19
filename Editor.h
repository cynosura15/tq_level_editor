#pragma once
#include <SFML/Graphics.hpp>
#include <fstream>
#include <sstream>
#include <string>
#include "print.h"
#include "Tile.h"
#include "Hud.h"

class Editor : public sf::Drawable
{
public:
	Editor();
	~Editor();

	void update(sf::View& view, sf::Vector2f mousePos);
	void mapImport();
	void mapExport();

	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:
	Hud _hud;

	sf::VertexArray _lines;
	std::vector<Tile> _tiles, _tilesEntities;
	std::vector<Tile> _curTilesMouse;
	std::string _tilesheet_path;
	sf::Vector2f _mapSize;	//used to read map file
	sf::Vector2f _curMousePosTile, _oldMousePosTile;

	int _selectedMateID;

	int _curTileID, _oldCurTileID;
	int _entityIDCount;
	sf::Vector2f _playerSpawn;
	bool _justPressedKey, _justPlacedSpecial, _justPressedChangeView;
	int _penSize;

	std::vector<sf::Texture> _texts;
};

