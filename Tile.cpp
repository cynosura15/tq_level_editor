#include "Tile.h"

Tile::Tile(int id, sf::Vector2f coords, std::vector<sf::Texture> &texts, bool special, int entityID) {
	_id = id;
	_sprite.setPosition(coords);
	_text = texts[id];

	if (special)
		_sprite.setColor(sf::Color(255, 255, 255, 100));

	_entityID = entityID;
	_boss = false;
}

Tile::~Tile() {

}

int Tile::getID() const {
	return _id;
}

int Tile::getEID() const {
	return _entityID;
}

bool Tile::isBoss() const {
	return _boss;
}
void Tile::setBoss(bool boss) {
	_boss = boss;
}

sf::FloatRect Tile::getHitbox() const {
	sf::Sprite s = _sprite;
	s.setTexture(_text);

	return s.getGlobalBounds();
}

sf::Vector2f Tile::getPosition() const {
	return sf::Vector2f(_sprite.getPosition().x, _sprite.getPosition().y);
}

void Tile::setPosition(sf::Vector2f newPos) {
	_sprite.setPosition(newPos);
}

void Tile::move(sf::Vector2f newPos) {
	_sprite.move(newPos);
}

void Tile::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	sf::Sprite s = _sprite;
	s.setTexture(_text);

	target.draw(s);
}