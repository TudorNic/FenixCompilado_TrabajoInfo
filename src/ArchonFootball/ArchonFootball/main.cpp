#include <SFML/Graphics.hpp>
#include <cmath>

int main()
{
    const int tileSize = 64;
    const int rows = 9;
    const int cols = 9;

    sf::RenderWindow window(sf::VideoMode(cols * tileSize, rows * tileSize), "Archon Football - Prueba");
    window.setFramerateLimit(60);

    // -------------------------
    // TEXTURAS TABLERO
    // -------------------------
    sf::Texture tileTexture;
    if (!tileTexture.loadFromFile("D:/Users/fjesu/Documents/GitHub/FenixCompilado_TrabajoInfo/src/ArchonFootball/x64/Debug/assets/board/tile_grass.png"))
        return -1;

    sf::Texture selectedTexture;
    if (!selectedTexture.loadFromFile("D:/Users/fjesu/Documents/GitHub/FenixCompilado_TrabajoInfo/src/ArchonFootball/x64/Debug/assets/board/tile_selected.png"))
        return -1;

    sf::Texture highlightTexture;
    if (!highlightTexture.loadFromFile("D:/Users/fjesu/Documents/GitHub/FenixCompilado_TrabajoInfo/src/ArchonFootball/x64/Debug/assets/board/tile_highlight.png"))
        return -1;

    sf::Texture powerTexture;
    if (!powerTexture.loadFromFile("D:/Users/fjesu/Documents/GitHub/FenixCompilado_TrabajoInfo/src/ArchonFootball/x64/Debug/assets/board/tile_power.png"))
        return -1;

    // -------------------------
    // TEXTURAS DELANTERO ROJO
    // -------------------------
    sf::Texture delantero1;
    if (!delantero1.loadFromFile("D:/Users/fjesu/Documents/GitHub/FenixCompilado_TrabajoInfo/src/ArchonFootball/x64/Debug/assets/players/red/sprite_delantero_red_idle.png"))
        return -1;

    sf::Texture delantero2;
    if (!delantero2.loadFromFile("D:/Users/fjesu/Documents/GitHub/FenixCompilado_TrabajoInfo/src/ArchonFootball/x64/Debug/assets/players/red/sprite_delantero_red_idle-2.png"))
        return -1;

    sf::Texture delantero3;
    if (!delantero3.loadFromFile("D:/Users/fjesu/Documents/GitHub/FenixCompilado_TrabajoInfo/src/ArchonFootball/x64/Debug/assets/players/red/sprite_delantero_red_idle-3.png"))
        return -1;

    // -------------------------
    // SPRITES
    // -------------------------
    sf::Sprite tile(tileTexture);
    sf::Sprite selected(selectedTexture);
    sf::Sprite highlight(highlightTexture);
    sf::Sprite power(powerTexture);

    sf::Sprite delantero(delantero1);
    delantero.setScale(3.f, 3.f);

    // -------------------------
    // POSICIÓN DE CASILLA SELECCIONADA
    // -------------------------
    int selectedRow = 2;
    int selectedCol = 2;

    // -------------------------
    // POSICIÓN VISUAL DEL JUGADOR
    // -------------------------
    float spriteWidth = delantero.getGlobalBounds().width;
    float spriteHeight = delantero.getGlobalBounds().height;

    float playerX = selectedCol * tileSize + (tileSize - spriteWidth) / 2.f;
    float playerY = selectedRow * tileSize + (tileSize - spriteHeight) / 2.f;

    float targetX = playerX;
    float targetY = playerY;

    // Velocidad de movimiento visual
    float moveSpeed = 250.f; // píxeles por segundo

    sf::Clock clock;
    sf::Clock deltaClock;

    while (window.isOpen())
    {
        float deltaTime = deltaClock.restart().asSeconds();

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    int mouseX = event.mouseButton.x;
                    int mouseY = event.mouseButton.y;

                    selectedCol = mouseX / tileSize;
                    selectedRow = mouseY / tileSize;

                    if (selectedCol < 0) selectedCol = 0;
                    if (selectedCol >= cols) selectedCol = cols - 1;
                    if (selectedRow < 0) selectedRow = 0;
                    if (selectedRow >= rows) selectedRow = rows - 1;

                    // Nueva posición objetivo del jugador
                    targetX = selectedCol * tileSize + (tileSize - spriteWidth) / 2.f;
                    targetY = selectedRow * tileSize + (tileSize - spriteHeight) / 2.f;
                }
            }
        }

        // -------------------------
        // ANIMACIÓN DEL DELANTERO
        // -------------------------
        float time = clock.getElapsedTime().asSeconds();
        int frame = static_cast<int>(time / 0.2f) % 3;

        if (frame == 0)
            delantero.setTexture(delantero1);
        else if (frame == 1)
            delantero.setTexture(delantero2);
        else
            delantero.setTexture(delantero3);

        // -------------------------
        // MOVIMIENTO SUAVE HACIA LA CASILLA OBJETIVO
        // -------------------------
        float dx = targetX - playerX;
        float dy = targetY - playerY;
        float distance = std::sqrt(dx * dx + dy * dy);

        if (distance > 1.f)
        {
            float dirX = dx / distance;
            float dirY = dy / distance;

            playerX += dirX * moveSpeed * deltaTime;
            playerY += dirY * moveSpeed * deltaTime;

            // Evitar pasarse del objetivo
            float newDx = targetX - playerX;
            float newDy = targetY - playerY;

            if ((dx > 0 && newDx < 0) || (dx < 0 && newDx > 0))
                playerX = targetX;

            if ((dy > 0 && newDy < 0) || (dy < 0 && newDy > 0))
                playerY = targetY;
        }
        else
        {
            playerX = targetX;
            playerY = targetY;
        }

        delantero.setPosition(playerX, playerY);

        window.clear();

        // -------------------------
        // DIBUJAR TABLERO 9x9
        // -------------------------
        for (int row = 0; row < rows; row++)
        {
            for (int col = 0; col < cols; col++)
            {
                tile.setPosition(static_cast<float>(col * tileSize), static_cast<float>(row * tileSize));
                window.draw(tile);
            }
        }

        // -------------------------
        // DIBUJAR CASILLAS ESPECIALES
        // -------------------------
        selected.setPosition(static_cast<float>(selectedCol * tileSize), static_cast<float>(selectedRow * tileSize));
        window.draw(selected);

        highlight.setPosition(3.f * tileSize, 2.f * tileSize);
        window.draw(highlight);

        highlight.setPosition(4.f * tileSize, 2.f * tileSize);
        window.draw(highlight);

        power.setPosition(6.f * tileSize, 4.f * tileSize);
        window.draw(power);

        // -------------------------
        // DIBUJAR JUGADOR
        // -------------------------
        window.draw(delantero);

        window.display();
    }

    return 0;
}