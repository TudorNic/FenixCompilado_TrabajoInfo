#include <SFML/Graphics.hpp>

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Prueba SFML");

    sf::Texture tileTexture;
    if (!tileTexture.loadFromFile("assets/board/tile_grass.png"))
    {
        return -1;
    }

    sf::Sprite tileSprite(tileTexture);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(tileSprite);
        window.display();
    }

    return 0;
}