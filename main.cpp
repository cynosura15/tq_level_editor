#include "Editor.h"

int main()
{
	sf::RenderWindow window(sf::VideoMode(1500, 900), "TIME QUEST EDITEUR");
	sf::View view(sf::Vector2f(0, 0), sf::Vector2f(1500 / 2, 900 / 2));
	window.setView(view);

	sf::err().rdbuf(NULL);

	print("Editeur de niveau pour Time-Quest", INFO);
	print("Commandes : \n* changer de tile avec les touches 1 à 9 (pas le numpad !)");
	print("IMPORTER : O, EXPORTER : P");
	print("Souvent se referer a la console ! L'UI arrive bientot");

	Editor editor;

	bool isWindowActive = true;

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::LostFocus)
				isWindowActive = false;
			if (event.type == sf::Event::GainedFocus)
				isWindowActive = true;
			if (event.type == sf::Event::Closed)
				window.close();
		}

		// get the current mouse position in the window
		sf::Vector2i pixelPos = sf::Mouse::getPosition(window);

		// convert it to world coordinates
		sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);

		if(window.hasFocus())
			editor.update(view, worldPos);

		window.clear();
		window.draw(editor);
		window.setView(view);
		window.display();
	}

	return 0;
}