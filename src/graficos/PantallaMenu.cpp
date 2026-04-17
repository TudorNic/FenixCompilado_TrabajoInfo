#include "PantallaMenu.h"
#include <stdexcept>
#include <iostream>

PantallaMenu::PantallaMenu(sf::RenderWindow& v, const std::string& rutaFuente, const std::string& rutaCampo, const std::string& rutaPelota)
    : ventana(v),
    fuente(),
    titulo(fuente),
    opcionesTexto{ sf::Text(fuente), sf::Text(fuente), sf::Text(fuente), sf::Text(fuente) },
    texturaFondo(),
	fondoCampo(texturaFondo),
    pelota1(rutaPelota, { 60.f, 120.f }, { 1.6f, 1.2f }, 0.12f),
    pelota2(rutaPelota, { 620.f, 420.f }, { -1.3f, -1.0f }, 0.12f),
    opcionConfirmada(false)
{
    if (!fuente.openFromFile(rutaFuente))
    {
        throw std::runtime_error("No se pudo cargar la fuente del menu.");
    }
    if (!texturaFondo.loadFromFile(rutaCampo))
    {
        throw std::runtime_error("No se pudo cargar la imagen del campo.");
    }
	std::cout << "Campo cargado correctamente. Tamano: " << texturaFondo.getSize().x << " x " << texturaFondo.getSize().y << std::endl;//prueba para verificar que la imagen se carga correctamente

    fondoCampo.setTexture(texturaFondo, true);

    sf::Vector2u sizeVentana = ventana.getSize();
    sf::Vector2u sizeImagen = texturaFondo.getSize();

    fondoCampo.setScale({static_cast<float>(sizeVentana.x) / static_cast<float>(sizeImagen.x),static_cast<float>(sizeVentana.y) / static_cast<float>(sizeImagen.y)});
   

    inicializarTextos();
    actualizarAspectoOpciones();
}

void PantallaMenu::procesarEventos()
{
    while (const auto evento = ventana.pollEvent())
    {
        if (evento->is<sf::Event::Closed>())
        {
            ventana.close();
        }

        manejarTeclado(*evento);
        manejarRaton(*evento);
    }
}

void PantallaMenu::actualizar()
{
    pelota1.actualizar(ventana);
	pelota2.actualizar(ventana);

    comprobarChoquePelotas();

    actualizarAspectoOpciones();
}

void PantallaMenu::dibujar()
{
    ventana.clear(sf::Color::Red);
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
    titulo.setCharacterSize(42);
    titulo.setFillColor(sf::Color::White);
    titulo.setPosition({ 220.f, 80.f });

    for (int i = 0; i < menu.obtenerNumeroOpciones(); i++)
    {
        opcionesTexto[i].setString(obtenerTextoOpcion(i));
        opcionesTexto[i].setCharacterSize(30);
        opcionesTexto[i].setPosition({ 280.f, 200.f + i * 70.f });
        opcionesTexto[i].setFillColor(sf::Color(255, 255, 255));
    }
}

void PantallaMenu::actualizarAspectoOpciones()
{
    for (int i = 0; i < menu.obtenerNumeroOpciones(); i++)
    {
        if (i == menu.obtenerIndiceSeleccionado())
        {
            opcionesTexto[i].setFillColor(sf::Color::Yellow);
            opcionesTexto[i].setScale({ 1.15f, 1.15f });
        }
        else
        {
            opcionesTexto[i].setFillColor(sf::Color(255, 255, 255));
            opcionesTexto[i].setScale({ 1.f, 1.f });
        }
    }
}

void PantallaMenu::manejarTeclado(const sf::Event& evento)
{
    if (const auto* tecla = evento.getIf<sf::Event::KeyPressed>())
    {
        if (tecla->code == sf::Keyboard::Key::Up)
        {
            menu.moverArriba();
        }
        else if (tecla->code == sf::Keyboard::Key::Down)
        {
            menu.moverAbajo();
        }
        else if (tecla->code == sf::Keyboard::Key::Enter)
        {
            opcionConfirmada = true;
        }
        else if (tecla->code == sf::Keyboard::Key::Escape)
        {
            ventana.close();
        }
    }
}

void PantallaMenu::manejarRaton(const sf::Event& evento)
{
    if (const auto* mouseMove = evento.getIf<sf::Event::MouseMoved>())
    {
        seleccionarOpcionConRaton(mouseMove->position);
    }

    if (const auto* mouseClick = evento.getIf<sf::Event::MouseButtonPressed>())
    {
        if (mouseClick->button == sf::Mouse::Button::Left)
        {
            seleccionarOpcionConRaton(mouseClick->position);
            opcionConfirmada = true;
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