#include "Game.h"
#include "Box2DHelper.h"
#include <iostream>

// Constructor de la clase Game
Game::Game(int ancho, int alto, std::string titulo)
{
    // Inicialización de la ventana y configuración de propiedades
    wnd = new RenderWindow(VideoMode(ancho, alto), titulo);
    wnd->setVisible(true);
    fps = 60;
    wnd->setFramerateLimit(fps);
    frameTime = 1.0f / fps;
    SetZoom(); // Configuración de la vista del juego
    InitPhysics(); // Inicialización del motor de física

    // Crear una caja en la posición inicial
    b2BodyDef boxDef;
    boxDef.type = b2_dynamicBody;
    boxDef.position.Set(50.0f, 50.0f);
    controlBody = phyWorld->CreateBody(&boxDef);

    b2PolygonShape boxShape;
    boxShape.SetAsBox(5.0f, 5.0f); // Tamaño de la caja

    b2FixtureDef boxFixtureDef;
    boxFixtureDef.shape = &boxShape;
    boxFixtureDef.density = 90.0f; // Densidad
    boxFixtureDef.friction = 50.0f; // Coeficiente de rozamiento
    boxFixtureDef.restitution = 0.0f; // Coeficiente de restitución

    controlBody->CreateFixture(&boxFixtureDef);
}

// Destructor de la clase
Game::~Game(void)
{ }

// Bucle principal del juego
void Game::Loop()
{
    while (wnd->isOpen())
    {
        wnd->clear(clearColor); // Limpiar la ventana
        DoEvents(); // Procesar eventos de entrada
        UpdatePhysics(); // Actualizar la simulación física
        DrawGame(); // Dibujar el juego
        wnd->display(); // Mostrar la ventana
    }
}

// Actualización de la simulación física
void Game::UpdatePhysics()
{
    phyWorld->Step(frameTime, 8, 8); // Simular el mundo físico
    phyWorld->ClearForces(); // Limpiar las fuerzas aplicadas a los cuerpos
    phyWorld->DebugDraw(); // Dibujar el mundo físico para depuración

    // Aplicar fuerza a la caja según las teclas izquierda/derecha
    if (Keyboard::isKeyPressed(Keyboard::Left))
    {
        controlBody->ApplyForceToCenter(b2Vec2(-0.9f, 0.0f), true);
    }
    else if (Keyboard::isKeyPressed(Keyboard::Right))
    {
        controlBody->ApplyForceToCenter(b2Vec2(0.1f, 0.0f), true);
    }
}

// Dibujo de los elementos del juego
void Game::DrawGame()
{

}

// Procesamiento de eventos de entrada
void Game::DoEvents()
{
    Event evt;
    while (wnd->pollEvent(evt))
    {
        switch (evt.type)
        {
        case Event::Closed:
            wnd->close(); // Cerrar la ventana si se presiona el botón de cerrar
            break;
        }
    }

    // Controlar el movimiento del cuerpo de control con el teclado
    // Segun la numeracion usada, cuando mas cerca de cero mas 
    // lento es el desplazamiento sobre ese eje
    controlBody->SetAwake(true);
    if (Keyboard::isKeyPressed(Keyboard::Left))
        controlBody->SetLinearVelocity(b2Vec2(-9.0f, 0.0f));
    if (Keyboard::isKeyPressed(Keyboard::Right))
        controlBody->SetLinearVelocity(b2Vec2(9.0f, 0.0f));
    if (Keyboard::isKeyPressed(Keyboard::Down))
        controlBody->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
    if (Keyboard::isKeyPressed(Keyboard::Up))
        controlBody->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
}

// Comprobación de colisiones (a implementar más adelante)
void Game::CheckCollitions()
{
    // Implementación de la comprobación de colisiones
}

// Configuración de la vista del juego
void Game::SetZoom()
{
    View camara;
    // Posicionamiento y tamaño de la vista
    camara.setSize(100.0f, 100.0f);
    camara.setCenter(50.0f, 50.0f);
    wnd->setView(camara); // Asignar la vista a la ventana
}

// Inicialización del motor de física y los cuerpos del mundo físico
void Game::InitPhysics()
{
    // Inicializar el mundo físico con la gravedad por defecto
    phyWorld = new b2World(b2Vec2(0.0f, 9.8f));

    // Crear un renderer de debug para visualizar el mundo físico
    debugRender = new SFMLRenderer(wnd);
    debugRender->SetFlags(UINT_MAX);
    phyWorld->SetDebugDraw(debugRender);

    // Crear el suelo y el techo
    b2Body* groundBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 100, 9);
    groundBody->SetTransform(b2Vec2(50.0f, 95.0f), 0.0f); // Bajar el suelo un poco para evitar la colisión con el techo

    b2Body* roofBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 100, 10);
    roofBody->SetTransform(b2Vec2(50.0f, 3.0f), 0.0f); // Posicionar el techo más arriba

    // Crear las paredes
    b2Body* leftWallBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 10, 100);
    leftWallBody->SetTransform(b2Vec2(1.0f, 50.0f), 0.0f); // Ajustar la posición X del muro izquierdo

    b2Body* rightWallBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 10, 100);
    rightWallBody->SetTransform(b2Vec2(95.0f, 50.0f), 0.0f); // Ajustar la posición X del muro derecho
}