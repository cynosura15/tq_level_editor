#include "Editor.h"

Editor::Editor() {
	_lines = sf::VertexArray(sf::Lines, 2);
	for (unsigned int i = 0; i < 100; i++) {
		sf::Vertex v;
		if (i == 0)
			v.color = sf::Color::Red;

		v.position = sf::Vector2f(0, (float)i * TSIZE);
		_lines.append(v);
		v.position = sf::Vector2f(TSIZE * 100, (float)i * TSIZE);
		_lines.append(v);

		v.position = sf::Vector2f((float)i * TSIZE, 0);
		_lines.append(v);
		v.position = sf::Vector2f((float)i * TSIZE, TSIZE * 100);
		_lines.append(v);
	}

	//chargement des textures
	sf::Texture text;
	_tilesheet_path = "tilesheet_war.png";
	for (int i = 0; i < 14; i++) {
		text.loadFromFile("assets/" + _tilesheet_path, sf::IntRect(i * TSIZE, 0, TSIZE, TSIZE));
		_texts.push_back(text);
	}

	_justPressedKey = false;
	_justPlacedSpecial = false;
	_playerSpawn = sf::Vector2f(0, 0);

	_penSize = 1;

	_oldMousePosTile = sf::Vector2f(0, 0);
	_curMousePosTile = _oldMousePosTile;
	_curTileID = 0;
	_oldCurTileID = 0;
	_entityIDCount = 0;

	_curTilesMouse.push_back(Tile(_curTileID, sf::Vector2f(0, 0), _texts, true));

	_selectedMateID = -1;
}

Editor::~Editor() {

}

void Editor::update(sf::View& view, sf::Vector2f mousePos) {
	_curMousePosTile.x = (float)((int)mousePos.x / (int)TSIZE) * TSIZE;
	_curMousePosTile.y = (float)((int)mousePos.y / (int)TSIZE) * TSIZE;

	//conversion
	mousePos.x = (float)((int)mousePos.x / (int)TSIZE);
	mousePos.y = (float)((int)mousePos.y / (int)TSIZE);
	mousePos.x *= TSIZE;
	mousePos.y *= TSIZE;

	_hud.update();

	float speed = 1;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
		view.move(sf::Vector2f(0, -speed));
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
		view.move(sf::Vector2f(-speed, 0));
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		view.move(sf::Vector2f(0, speed));
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		view.move(sf::Vector2f(speed, 0));

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Add) || sf::Keyboard::isKeyPressed(sf::Keyboard::Subtract))
	{
		if (!_justPressedChangeView) {
			_justPressedChangeView = true;

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Subtract))
				view.setSize(sf::Vector2f(view.getSize().x * 1.1f, view.getSize().y * 1.1f));
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Add))
				view.setSize(sf::Vector2f(view.getSize().x * 0.9f, view.getSize().y * 0.9f));
		}
	}
	else
		_justPressedChangeView = false;

	//Changer de tiles
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1))
		_curTileID = 0;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2))
		_curTileID = 1;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3))
		_curTileID = 2;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num4))
		_curTileID = 3;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num5))
		_curTileID = 4;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num6))
		_curTileID = 5;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num7))
		_curTileID = 6;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num8))
		_curTileID = 7;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num9))
		_curTileID = 8;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad0))
		_curTileID = ENNEMY;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad1))
		_curTileID = SPAWN;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad2))
		_curTileID = MATE;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad3))
		_curTileID = WEAPON;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad4))
		_curTileID = MEDKIT;
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left)){
		if (!_justPlacedSpecial && _curTileID >= 9) {
			_justPlacedSpecial = true;

			if (_curTileID == SPAWN) {
				_playerSpawn = sf::Vector2f(mousePos.x, mousePos.y);
				for (unsigned int i = 0; i < _tiles.size(); i++)
					if (_tiles[i].getID() == SPAWN)
						_tiles.erase(_tiles.begin() + i);
				_tiles.push_back(Tile(SPAWN, mousePos, _texts));
			}
			else {
				_tiles.push_back(Tile(_curTileID, mousePos, _texts, false, _entityIDCount));
				_entityIDCount++;
			}
		}
	}
	else
		_justPlacedSpecial = false;

	//sauvegarde / chargement
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::O) || sf::Keyboard::isKeyPressed(sf::Keyboard::P)
		|| sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::O)) {
			if (!_justPressedKey) {
				_justPressedKey = true;
				mapImport();
			}
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::P)) {
			if (!_justPressedKey) {
				_justPressedKey = true;
				mapExport();
			}
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
			if (!_justPressedKey) {
				_justPressedKey = true;
				
				if (_penSize > 0) {
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
						_penSize++;

					else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
						_penSize--;
						if (_penSize < 1)
							_penSize = 1;
					}

					_curTilesMouse.clear();

					for (int i = 0; i < _penSize; i++) {
						for (int j = 0; j < _penSize; j++) {
							sf::Vector2f pos(mousePos.x + i * TSIZE, mousePos.y + j * TSIZE);
							_curTilesMouse.push_back(Tile(_curTileID, pos, _texts, true));
						}
					}
				}
			}
		}
	}
	else
		_justPressedKey = false;

	//Tile transparentes là
	if (_oldMousePosTile != _curMousePosTile || _oldCurTileID != _curTileID) {
		float diffX = _curMousePosTile.x - _oldMousePosTile.x;
		float diffY = _curMousePosTile.y - _oldMousePosTile.y;
		sf::Vector2f moveVect(diffX, diffY);

		if(_oldCurTileID != _curTileID)
			for (int i = 0; i < _penSize; i++) {
				for (int j = 0; j < _penSize; j++) {
					sf::Vector2f pos(mousePos.x + i * TSIZE, mousePos.y + j * TSIZE);
					_curTilesMouse.push_back(Tile(_curTileID, pos, _texts, true));
				}
			}

		for (int i = 0; i < _curTilesMouse.size(); i++)
			_curTilesMouse[i].move(moveVect);
	}

	if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
		bool canAdd = true;
		if (canAdd) {
			for (int i = 0; i < _curTilesMouse.size(); i++) {
				canAdd = true;
				for (unsigned int j = 0; j < _tiles.size() && canAdd; j++)
					if (_tiles[j].getHitbox().contains(_curTilesMouse[i].getPosition()))
						canAdd = false;
				if (canAdd) {
					_tiles.push_back(Tile(_curTileID, _curTilesMouse[i].getPosition(), _texts));
				}
			}
		}
	}
	else if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
		for (int i = 0; i < _curTilesMouse.size(); i++)
			for (unsigned int j = 0; j < _tiles.size(); j++)
				if (_tiles[j].getHitbox().contains(_curTilesMouse[i].getPosition())) {
					_tiles.erase(_tiles.begin() + j);
					break;
				}
	}

	_oldMousePosTile = _curMousePosTile;
	_oldCurTileID = _curTileID;
}

void Editor::mapImport() {
	std::string path = "";

	print("- importation d'une map -\nMerci de specifier le chemin ! (vide = ./map.txt) :", INFO);
	std::getline(std::cin, path);
	if (path == "")
		path = "map.txt";

	std::ifstream mapFile(path.c_str());
	if (mapFile) {
		_tiles.clear();
		std::string currentOpe = "";
		while (mapFile >> currentOpe) {
			if (currentOpe == "#tilesheet")
				mapFile >> _tilesheet_path;
			else if (currentOpe == "#mapsize") {
				mapFile >> _mapSize.x;
				mapFile >> _mapSize.y;
			}
			else if (currentOpe == "#playerspawn") {
				mapFile >> _playerSpawn.x;
				mapFile >> _playerSpawn.y;
				_tilesEntities.push_back(Tile(SPAWN, _playerSpawn, _texts, false));
			}
			else if (currentOpe == "#mate") {
				std::string trash;
				sf::Vector2f matePos;
				mapFile >> trash;	//2 fois car la 1ere valeur ici (la vie) ne nous interesse pas
				mapFile >> matePos.x;
				mapFile >> matePos.y;
				_tilesEntities.push_back(Tile(MATE, matePos, _texts, false));
			}
			else if (currentOpe == "#ennemy") {
				std::string trash;
				sf::Vector2f ennemyPos;
				mapFile >> trash;	//2 fois car la 1ere valeur ici (la vie) ne nous interesse pas
				mapFile >> ennemyPos.x;
				mapFile >> ennemyPos.y;
				_tilesEntities.push_back(Tile(ENNEMY, ennemyPos, _texts, false));
			}
			else if (currentOpe == "#gun") {
				std::string trash = "";
				sf::Vector2f gunPos;
				mapFile >> trash;
				mapFile >> gunPos.x;
				mapFile >> gunPos.y;
				_tilesEntities.push_back(Tile(WEAPON, gunPos, _texts, false));
			}
			else if (currentOpe == "#medkit") {
				sf::Vector2f medpos;
				mapFile >> medpos.x;
				mapFile >> medpos.y;
				_tilesEntities.push_back(Tile(MEDKIT, medpos, _texts, false));
			}
			else if (currentOpe == "#tiles") {
				std::cout << "lecture des tiles...";
				int i = 0, j = 0;
				while (mapFile >> currentOpe) {
					_tiles.push_back(Tile(std::stoi(currentOpe), sf::Vector2f(i * TSIZE, j * TSIZE), _texts, false));
					i++;
					if (i >= _mapSize.x) {
						j++;
						i = 0;
						if (j > _mapSize.y)
							print("Mauvaise lecture du niveau...", ERROR);
					}
				}
			}
		}
		for (int i = 0; i < _tilesEntities.size(); i++) {
			_tiles.push_back(_tilesEntities[i]);
		}

		std::cout << "Il y a " << _tiles.size() << " tiles\n";
	}
	else
		print("impossible d'acceder a " + path, ERROR);
}

void Editor::mapExport() {
	std::string path = "";

	print("- exportation de la map -\nMerci de specifier le chemin ! (vide = ./map.txt) :", INFO);
	std::getline(std::cin, path);
	if (path == "")
		path = "map.txt";

	std::ofstream mapFile(path.c_str());
	if (mapFile) {
		mapFile << "-----Textures-----\n\n";

		mapFile << "#tilesheet\n";
		mapFile << _tilesheet_path << "\n\n";
		mapFile << "#playertexture\n";
		mapFile << "soldatFrancais40.png\n\n";
		mapFile << "#matetexture\n";
		mapFile << "soldatFrancais40.png\n\n";
		mapFile << "#ennemytexture\n";
		mapFile << "soldatAllemand40.png\n\n";

		mapFile << "-----Map infos-----\n\n";

		mapFile << "#mapsize\n";
		print("Obtention de la taille de la map", INFO);
		int max_x = 0, max_y = 0;
		for (unsigned int i = 0; i < _tiles.size(); i++) {
			if (_tiles[i].getPosition().x > max_x)
				max_x = _tiles[i].getPosition().x;
			if (_tiles[i].getPosition().y > max_y)
				max_y = _tiles[i].getPosition().y;
		}
		max_x /= TSIZE;
		max_x++;
		max_y /= TSIZE;
		max_y++;
		
		if (max_x == 0 && max_y == 0)
			print("la map est vide !", ERROR);
		else {
			std::stringstream msg;
			msg << "taille de la map : " << max_x << "x" << max_y << " et il y a " << _tiles.size() << " tiles";
			print(msg.str(), OK);

			mapFile << max_x << " " << max_y << "\n\n";
			mapFile << "#playerspawn\n" << _playerSpawn.x << " " << _playerSpawn.y << "\n\n";

			mapFile << "#bossID\n";
			bool findBoss = false;
			for (int i = 0; i < _tiles.size() && !findBoss; i++) {
				if (_tiles[i].isBoss()) {
					mapFile << _tiles[i].getEID() << "\n\n";
					findBoss = true;
				}
			}
			if(!findBoss)
				mapFile << "-1\n\n";

			mapFile << "#nextmap\n";
			print("Veuillez renseigner la map suivante (ou laisser vide)");
			std::string nextPath = "none";
			std::cin >> nextPath;
			mapFile << nextPath << "\n\n";
			std::cin.ignore();

			mapFile << "#quest\n";
			print("Veuillez renseigner la quete correspondante (ou laisser vide)");
			std::string nextquest = "none";
			std::cin >> nextquest;
			mapFile << nextquest << "\n\n";
			std::cin.ignore();

			mapFile << "-----Entities-----\n\n";

			for (unsigned int i = 0; i < _tiles.size(); i++) {
				if (_tiles[i].getID() == ENNEMY)
					mapFile << "#ennemy\n" << "100 " << _tiles[i].getPosition().x << " " << _tiles[i].getPosition().y << " " << _tiles[i].getEID() << "\n";
				else if (_tiles[i].getID() == MATE) {
					mapFile << "#mate\n" << "100 " << _tiles[i].getPosition().x << " " << _tiles[i].getPosition().y << " " << _tiles[i].getEID() << "\n";
					mapFile << "Salut, je suis un mate\n";
				}
			}

			mapFile << "\n-----Objects-----\n\n";

			for (unsigned int i = 0; i < _tiles.size(); i++) {
				if (_tiles[i].getID() == WEAPON)
					mapFile << "#gun\nmp40 " << _tiles[i].getPosition().x << " " << _tiles[i].getPosition().y << "\n";
				else if (_tiles[i].getID() == MEDKIT)
					mapFile << "#medkit\n" << _tiles[i].getPosition().x << " " << _tiles[i].getPosition().y << "\n";
			}

			mapFile << "\n-----Tiles disposition-----\n\n";

			mapFile << "#tiles\n";

			std::vector<int> orderedMap;

			//on réordonne les tiles
			for (int i = 0; i < max_y; i++) {
				for (int j = 0; j < max_x; j++) {
					bool find = false;
					for (unsigned int k = 0; k < _tiles.size(); k++) {
						if (_tiles[k].getPosition().x / TSIZE == j && _tiles[k].getPosition().y / TSIZE == i && _tiles[k].getID() < 9) {
							//std::cout << i << ";" << j << " - tile : " << _tiles[k].getPosition().x << ";" << _tiles[k].getPosition().y / 30 << std::endl;
							find = true;
							orderedMap.push_back(_tiles[k].getID());
							std::cout << ".";
						}
					}

					if (!find) {
						orderedMap.push_back(0);
						std::cout << ".";
						//std::cout << "nouvelle tile en " << i << ";" << j << "\n";
					}
				}
			}

			std::stringstream s;
			s << "taille finale de la map : " << orderedMap.size() << "\n";
			print(s.str(), DEBUG);
			for (unsigned int i = 0; i < orderedMap.size(); i++) {
				std::cout << orderedMap[i] << " ";
				mapFile << orderedMap[i] << " ";
			}
			mapFile << "\n";
			print("export reussi !", OK);
		}
	}
	else
		print("impossible de creer le fichier a l'adresse " + path, ERROR);
}

void Editor::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	//Dessin de la grille
	target.draw(_lines);

	float tx = target.getView().getCenter().x;
	float ty = target.getView().getCenter().y;
	float sizex = target.getView().getSize().x / 2;
	float sizey = target.getView().getSize().y / 2;

	for (unsigned int i = 0; i < _tiles.size(); i++) {
		float ax = _tiles[i].getPosition().x;
		float ay = _tiles[i].getPosition().y;
		
		if (ax >= tx - sizex && ax < tx + sizex && ay >= ty - sizey && ay < ty + sizey)
			target.draw(_tiles[i]);
	}

	//Pre dessin des / de la tile actuelle
	for (unsigned int i = 0; i < _curTilesMouse.size(); i++)
		target.draw(_curTilesMouse[i]);
}
