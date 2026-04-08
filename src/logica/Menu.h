#pragma once
class Menu
{
public:
    enum Opcion
    {
        JUGAR = 0,
        RANKING = 1,
        INSTRUCCIONES = 2,
        SALIR = 3
    };

private:
    Opcion opcionSeleccionada;
    int numeroOpciones;

public:
    Menu();

    void moverArriba();
    void moverAbajo();
    void seleccionarOpcion(int indice);

    Opcion obtenerOpcionSeleccionada() const;
    int obtenerIndiceSeleccionado() const;
    int obtenerNumeroOpciones() const;
};