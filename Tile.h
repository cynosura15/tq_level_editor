#pragma once
#include <SFML/Graphics.hpp>

#define TSIZE 30

#define ENNEMY 9
#define SPAWN 10
#define MATE 11
#define WEAPON 12
#define MEDKIT 13

class Tile : public sf::Drawable
{
public:
	Tile(int id, sf::Vector2f coords, std::vector<sf::Texture>& texts, bool special = false, int entityID = -1);
	~Tile();

	int getID() const;
	int getEID() const;
	bool isBoss() const;
	void setBoss(bool boss);
	sf::FloatRect getHitbox() const;
	sf::Vector2f getPosition() const;

	void setPosition(sf::Vector2f newPos);
	void move(sf::Vector2f newPos);

	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:
	int _id;
	int _entityID;
	bool _boss;
	sf::Texture _text;
	sf::Sprite _sprite;
};

