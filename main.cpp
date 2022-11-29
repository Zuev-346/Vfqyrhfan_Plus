#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>

int main()
{
	sf::Color const BACKGROUND = sf::Color(180, 180, 180, 0);
	int const WIDTH = 1400;
	int const HEIGHT = 750;
	int const FPS = 60;


	sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Vfqyrhfan");
	window.setFramerateLimit(FPS);

	sf::CircleShape gunsight;
	gunsight.setRadius(5);
	gunsight.setOutlineColor(sf::Color::Red);
	gunsight.setFillColor(sf::Color::Transparent);
	gunsight.setOutlineThickness(1.5f);
	gunsight.setPosition(static_cast<int>(WIDTH / 2.0f) - gunsight.getRadius(), static_cast<int>(HEIGHT / 2.0f) - gunsight.getRadius());


	sf::Font font;
	font.loadFromFile("fonts/arial.ttf");

	float precision = 0;
	float recLevel = 0;
	sf::Text zoomText, precText;
	zoomText.setFont(font);
	precText.setFont(font);
	zoomText.setFillColor(sf::Color::Black);
	precText.setFillColor(sf::Color::Black);
	zoomText.setCharacterSize(24);
	precText.setCharacterSize(24);

	while (window.isOpen())
	{
		sf::Event evnt;
		while (window.pollEvent(evnt))
		{
			switch (evnt.type)
			{
			case sf::Event::Closed:
				window.close();
				break;
			case sf::Event::KeyReleased:
				if (evnt.key.code == sf::Keyboard::Key::O){}
				break;
			case sf::Event::MouseWheelScrolled:
				if (evnt.mouseWheelScroll.delta <= 0) {}

			}
		}

		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
		}

		zoomText.setString("Zoom: " + std::to_string(pow(8, recLevel - 1)));
		precText.setString("Max. Iterations: " + std::to_string(precision));
		precText.setPosition(sf::Vector2f(0, 32));

		window.clear(BACKGROUND);
		window.draw(zoomText);
		window.draw(precText);
		window.draw(gunsight);
		window.display();
	}

	return 0;
}