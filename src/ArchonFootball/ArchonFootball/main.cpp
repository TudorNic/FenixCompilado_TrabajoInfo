#include <SFML/Graphics.hpp>

int main()
{
    const int tileSize = 64;
    const int rows = 9;
    const int cols = 9;

    sf::RenderWindow window(sf::VideoMode(cols * tileSize, rows * tileSize), "Prueba SFML");

    sf::Texture tileTexture;
    if (!tileTexture.loadFromFile("D:/Users/fjesu/Documents/GitHub/FenixCompilado_TrabajoInfo/src/ArchonFootball/x64/Debug/assets/board/tile_grass.png"))
    {
        return -1;
    }

    sf::Texture selectedTexture;
    if (!selectedTexture.loadFromFile("D:/Users/fjesu/Documents/GitHub/FenixCompilado_TrabajoInfo/src/ArchonFootball/x64/Debug/assets/board/tile_selected.png"))
    {
        return -1;
    }

    sf::Texture highlightTexture;
    if (!highlightTexture.loadFromFile("D:/Users/fjesu/Documents/GitHub/FenixCompilado_TrabajoInfo/src/ArchonFootball/x64/Debug/assets/board/tile_highlight.png"))
    {
        return -1;
    }

    sf::Texture powerTexture;
    if (!powerTexture.loadFromFile("D:/Users/fjesu/Documents/GitHub/FenixCompilado_TrabajoInfo/src/ArchonFootball/x64/Debug/assets/board/tile_power.png"))
    {
        return -1;
    }

    sf::Sprite tileSprite(tileTexture);
    sf::Sprite selectedSprite(selectedTexture);
    sf::Sprite highlightSprite(highlightTexture);
    sf::Sprite powerSprite(powerTexture);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();

        for (int row = 0; row < rows; row++)
        {
            for (int col = 0; col < cols; col++)
            {
                tileSprite.setPosition(col * tileSize, row * tileSize);
                window.draw(tileSprite);
            }
        }

        selectedSprite.setPosition(2 * tileSize, 2 * tileSize);
        window.draw(selectedSprite);

        highlightSprite.setPosition(3 * tileSize, 2 * tileSize);
        window.draw(highlightSprite);

        highlightSprite.setPosition(4 * tileSize, 2 * tileSize);
        window.draw(highlightSprite);

        powerSprite.setPosition(6 * tileSize, 4 * tileSize);
        window.draw(powerSprite);

        window.display();
    }

    return 0;
}