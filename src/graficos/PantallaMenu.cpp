#include "PantallaMenu.h"
#include <stdexcept>

void centrarTexto(sf::Text& texto, float y)
{
    sf::FloatRect bounds = texto.getLocalBounds();
    texto.setOrigin(bounds.left + bounds.width / 2.f, bounds.top + bounds.height / 2.f);
    texto.setPosition(500.f, y);
}

PantallaMenu::PantallaMenu(sf::RenderWindow& v, const std::string& rutaFuente, const std::string& rutaCampo, 
    const std::string& rutaPelota,const std::string& rutaMover, const std::string& rutaConfirmar,
    const std::string& rutaSalir, const std::string& rutaMusica)
    : ventana(v),
    pelota1(rutaPelota, { 60.f, 120.f }, { 1.6f, 1.2f }, 0.12f),
    pelota2(rutaPelota, { 620.f, 420.f }, { -1.3f, -1.0f }, 0.12f),
	sonidoMenu(rutaMover, rutaConfirmar, rutaSalir, rutaMusica),
    opcionConfirmada(false)
{
    if (!fuente.loadFromFile(rutaFuente))
    {
        throw std::runtime_error("No se pudo cargar la fuente del menu.");
    }
    if (!texturaFondo.loadFromFile(rutaCampo))
    {
        throw std::runtime_error("No se pudo cargar la imagen del campo.");
    }

    titulo.setFont(fuente);
    for (int i = 0; i < 4; i++)
    {
        opcionesTexto[i].setFont(fuente);
    }

    fondoCampo.setTexture(texturaFondo, true);

    sf::Vector2u sizeVentana = ventana.getSize();
    sf::Vector2u sizeImagen = texturaFondo.getSize();

    fondoCampo.setScale(static_cast<float>(sizeVentana.x) / static_cast<float>(sizeImagen.x),static_cast<float>(sizeVentana.y) / static_cast<float>(sizeImagen.y));
   

    inicializarTextos();
    actualizarAspectoOpciones();

    sonidoMenu.reproducirMusica();
}

void PantallaMenu::procesarEventos()
{
    sf::Event evento;

    while (ventana.pollEvent(evento))
    {
        if (evento.type == sf::Event::Closed)
        {
            ventana.close();
        }

        if (evento.type == sf::Event::Resized)
        {
            sf::View vista(sf::FloatRect(
                0.f,
                0.f,
                static_cast<float>(evento.size.width),
                static_cast<float>(evento.size.height)
            ));
            ventana.setView(vista);

            sf::Vector2u sizeImagen = texturaFondo.getSize();
            fondoCampo.setScale(
                static_cast<float>(evento.size.width) / static_cast<float>(sizeImagen.x),
                static_cast<float>(evento.size.height) / static_cast<float>(sizeImagen.y)
            );
        }

        manejarTeclado(evento);
        manejarRaton(evento);
    }
}

void PantallaMenu::actualizar()
{
    pelota1.actualizar(ventana);
	pelota2.actualizar(ventana);

    comprobarChoquePelotas();

    actualizarAspectoOpciones();
}

void PantallaMenu::detenerMusicaMenu()
{
    sonidoMenu.detenerMusica();
}

void PantallaMenu::dibujar()
{
    ventana.clear();
    //campo de fondo
	ventana.draw(fondoCampo);
    //dibujamos fondo animado
	pelota1.dibujar(ventana);
    pelota2.dibujar(ventana);
    
    //Dibujamos luego el menu por encima
    ventana.draw(titulo);

    for (int i = 0; i < menu.obtenerNumeroOpciones(); i++)
    {
        ventana.draw(opcionesTexto[i]);
    }

    ventana.display();
}

Menu& PantallaMenu::obtenerMenu()
{
    return menu;
}

bool PantallaMenu::estaOpcionConfirmada() const
{
    return opcionConfirmada;
}

Menu::Opcion PantallaMenu::obtenerOpcionConfirmada() const
{
    return menu.obtenerOpcionSeleccionada();
}

void PantallaMenu::reiniciarConfirmacion()
{
    opcionConfirmada = false;
}

void PantallaMenu::inicializarTextos()
{
    titulo.setString("ARCHON FOOTBALL");
    titulo.setCharacterSize(80);
    titulo.setFillColor(sf::Color::White);
    centrarTexto(titulo, 145.f);

    for (int i = 0; i < menu.obtenerNumeroOpciones(); i++)
    {
        opcionesTexto[i].setString(obtenerTextoOpcion(i));
        opcionesTexto[i].setCharacterSize(50);
        opcionesTexto[i].setFillColor(sf::Color::Black);
        centrarTexto(opcionesTexto[i], 300.f + i * 75.f);
    }
}

void PantallaMenu::actualizarAspectoOpciones()
{
    for (int i = 0; i < menu.obtenerNumeroOpciones(); i++)
    {
        if (i == menu.obtenerIndiceSeleccionado())
        {
            opcionesTexto[i].setFillColor(sf::Color::Yellow);
            opcionesTexto[i].setScale({ 1.18f, 1.18f });
        }
        else
        {
            opcionesTexto[i].setFillColor(sf::Color::Black);
            opcionesTexto[i].setScale({ 1.f, 1.f });
        }
        centrarTexto(opcionesTexto[i], 300.f + i *115.f);
    }
}

void PantallaMenu::manejarTeclado(const sf::Event& evento)
{
    if (evento.type == sf::Event::KeyPressed)
    {
        if (evento.key.code == sf::Keyboard::Up)
        {
            menu.moverArriba();
            sonidoMenu.reproducirMover();
        }
        else if (evento.key.code == sf::Keyboard::Down)
        {
            menu.moverAbajo();
            sonidoMenu.reproducirMover();
        }
        else if (evento.key.code == sf::Keyboard::Return)
        {
            opcionConfirmada = true;
            sonidoMenu.reproducirConfirmar();
        }
        else if (evento.key.code == sf::Keyboard::Escape)
        {
            sonidoMenu.reproducirSalir();
            ventana.close();
        }
    }
}

void PantallaMenu::manejarRaton(const sf::Event& evento)
{
    if (evento.type == sf::Event::MouseMoved)
    {
        seleccionarOpcionConRaton(sf::Vector2i(evento.mouseMove.x, evento.mouseMove.y));
    }

    if (evento.type == sf::Event::MouseButtonPressed)
    {
        if (evento.mouseButton.button == sf::Mouse::Left)
        {
            seleccionarOpcionConRaton(sf::Vector2i(evento.mouseButton.x, evento.mouseButton.y));
            opcionConfirmada = true;
            sonidoMenu.reproducirConfirmar();
        }
    }
}

void PantallaMenu::seleccionarOpcionConRaton(sf::Vector2i posicionRaton)
{
    sf::Vector2f posicion(
        static_cast<float>(posicionRaton.x),
        static_cast<float>(posicionRaton.y)
    );

    for (int i = 0; i < menu.obtenerNumeroOpciones(); i++)
    {
        if (opcionesTexto[i].getGlobalBounds().contains(posicion))
        {
            menu.seleccionarOpcion(i);
            sonidoMenu.reproducirMover();
            break;
        }
        
    }
}

std::string PantallaMenu::obtenerTextoOpcion(int indice) const
{
    switch (indice)
    {
    case 0: return "Jugar";
    case 1: return "Ranking";
    case 2: return "Instrucciones";
    case 3: return "Salir";
    default: return "";
    }
}

void PantallaMenu::comprobarChoquePelotas()
{
    sf::Vector2f pos1 = pelota1.obtenerPosicion();
    sf::Vector2f pos2 = pelota2.obtenerPosicion();

    float r1 = pelota1.obtenerRadioAproximado();
    float r2 = pelota2.obtenerRadioAproximado();

    float dx = pos2.x - pos1.x;
    float dy = pos2.y - pos1.y;

    float distanciaCuadrada = dx * dx + dy * dy;
    float sumaRadios = r1 + r2;

    if (distanciaCuadrada <= sumaRadios * sumaRadios)
    {
        sf::Vector2f vel1 = pelota1.obtenerVelocidad();
        sf::Vector2f vel2 = pelota2.obtenerVelocidad();

        // Rebote simple: intercambiar velocidades
        pelota1.setVelocidad(vel2);
        pelota2.setVelocidad(vel1);
    }
}