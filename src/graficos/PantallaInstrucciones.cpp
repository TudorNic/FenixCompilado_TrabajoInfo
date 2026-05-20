#include "PantallaInstrucciones.h"
#include <stdexcept>

static void centrarTextoInstrucciones(sf::Text& texto, float y)
{
    sf::FloatRect bounds = texto.getLocalBounds();
    texto.setOrigin(bounds.left + bounds.width / 2.f, bounds.top + bounds.height / 2.f);
    texto.setPosition(500.f, y);
}

PantallaInstrucciones::PantallaInstrucciones(sf::RenderWindow& v, const std::string& rutaFuente)
    : ventana(v),
    volverAlMenu(false),
    desplazamientoVertical(0.f),
    velocidadScroll(25.f),
    limiteInferiorScroll(-250.f)
{
    if (!fuente.loadFromFile(rutaFuente))
    {
        throw std::runtime_error("No se pudo cargar la fuente de instrucciones.");
    }

    titulo.setFont(fuente);
    textoInstrucciones.setFont(fuente);
    textoVolver.setFont(fuente);

    inicializarTextos();
}

void PantallaInstrucciones::procesarEventos()
{
    sf::Event evento;

    while (ventana.pollEvent(evento))
    {
        if (evento.type == sf::Event::Closed)
        {
            ventana.close();
        }

        if (evento.type == sf::Event::KeyPressed)
        {
            if (evento.key.code == sf::Keyboard::Escape)
            {
                volverAlMenu = true;
            }
            else if (evento.key.code == sf::Keyboard::Down || evento.key.code == sf::Keyboard::S)
            {
                desplazamientoVertical -= velocidadScroll;

                if (desplazamientoVertical < limiteInferiorScroll)
                {
                    desplazamientoVertical = limiteInferiorScroll;
                }
            }
            else if (evento.key.code == sf::Keyboard::Up || evento.key.code == sf::Keyboard::W)
            {
                desplazamientoVertical += velocidadScroll;

                if (desplazamientoVertical > 0.f)
                {
                    desplazamientoVertical = 0.f;
                }
            }
        }
    }
}

void PantallaInstrucciones::actualizar()
{
}

void PantallaInstrucciones::dibujar()
{
    ventana.clear(sf::Color(20, 20, 20));

    centrarTextoInstrucciones(titulo, 90.f + desplazamientoVertical);
    textoInstrucciones.setPosition(160.f, 170.f + desplazamientoVertical);
    centrarTextoInstrucciones(textoVolver, 930.f);

    ventana.draw(titulo);
    ventana.draw(textoInstrucciones);
    ventana.draw(textoVolver);

    ventana.display();
}

bool PantallaInstrucciones::debeVolverAlMenu() const
{
    return volverAlMenu;
}

void PantallaInstrucciones::reiniciarVolver()
{
    volverAlMenu = false;
}

void PantallaInstrucciones::inicializarTextos()
{
    titulo.setString("INSTRUCCIONES");
    titulo.setCharacterSize(38);
    titulo.setFillColor(sf::Color::White);
    centrarTextoInstrucciones(titulo, 90.f);
    textoInstrucciones.setString(
        
"En la opcion Jugar puedes elegir entre enfrentarte a la IA\n"
"o jugar contra otro jugador.\n\n"

"El juego se desarrolla sobre un tablero de 9x9. Cada equipo\n"
"empieza con una formacion fija de piezas:\n"
"Aficion, Delanteros, Centrocampistas, Laterales,\n"
"Defensas centrales y Entrenador.\n\n"

"Cada tipo de pieza tiene habilidades propias.\n"
"La mejor forma de descubrirlas es jugar y experimentar.\n\n"

"Para ganar la partida, debes cumplir uno de estos objetivos:\n"
"- Controlar los cinco puntos de poder del tablero.\n"
"- Eliminar todas las piezas del rival.\n"
"- Dejar al oponente con una sola pieza encarcelada.\n\n"

"Los puntos de poder estan situados en el centro del tablero\n"
"y en el centro de cada borde. Las piezas colocadas en ellos\n"
"se curan mas rapido y ademas son inmunes a los hechizos\n"
"del rival.\n\n"

"Cuando dos piezas coinciden en una misma casilla, se entra\n"
"en una arena de combate a pantalla completa. Alli controlas\n"
"directamente a tu pieza. Aunque las piezas mas fuertes suelen\n"
"tener ventaja, no te desesperes, tu habilidad y estrategia\n"
"es lo que marca la diferencia.\n\n"

"Prueba distintas estrategias y descubre por ti mismo las\n"
"habilidades de cada pieza.\n\n"

"Controles:\n"
"- Mover arriba: W o Flecha Arriba\n"
"- Mover abajo: S o Flecha Abajo\n"
"- Mover izquierda: A o Flecha Izquierda\n"
"- Mover derecha: D o Flecha Derecha\n"
"-Disparo/ataque: Barra espaciadora/ctrl\n"
"- Confirmar: Enter\n"
"- Cancelar: ESC\n"
    );
    textoInstrucciones.setCharacterSize(17);
    textoInstrucciones.setFillColor(sf::Color(220, 220, 220));
    textoInstrucciones.setPosition({ 160.f, 170.f });

    textoVolver.setString("Pulsa ESC para volver");
    textoVolver.setCharacterSize(18);
    textoVolver.setFillColor(sf::Color::Yellow);
    centrarTextoInstrucciones(textoVolver, 930.f);
}