#pragma once

#include <SFML/Graphics.hpp>

class Hud : public sf::Drawable
{
public:
	Hud();
	~Hud();

	int update(int selectedMate = -1);	//Retourne la tile actuellement selectionn�e


	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:
	//Barre des tiles
	sf::RectangleShape _bodyTileSelector;
	std::vector<sf::Sprite> _tiles;
	std::vector<sf::Text> _texts;

	//Config du mate s�lectionn� (si selectionn�)
	sf::RectangleShape _bodyMateConfig;
	sf::RectangleShape _bodySetMateBoss;
	sf::RectangleShape _bodySetMateMessage;

	//Fenetre d'export / import
	sf::RectangleShape _window;
	sf::Font _font;

	sf::Text _txtSauvegarderUneMap;
	sf::Text _txtNomDeLaMap, _txtUsrNomDeLaMap;
	sf::Text _txtNomFichierQuete, _txtusrNomFichierQuete;
	sf::Text _txtNomMapSuivante, _txtUsrNomMapSuivante;

	sf::Text _txtOuvrirMap, _txtNomMap, _txtUsrNomMap;

	sf::Text _txtBtnValider, _txtBtnAnnuler;
};
