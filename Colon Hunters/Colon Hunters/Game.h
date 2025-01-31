/// <summary>
/// author Ciaran Kinsella
/// </summary>
//#ifndef GAME_HPP
#define GAME_HPP
#define GAME_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Event.hpp>
#include <vector>
#include <cmath>


class Game {
public:
    Game();
    ~Game();
    void run();

private:
    // Game states
    enum class GameState 
    {
        Menu, //either starting in menu
        Playing, // or playing the game.
        CardGame
    };

    GameState m_gameState;

    // Window settings
    static const int SCREEN_WIDTH = 1280;  // self exlanitory here
    static const int SCREEN_HEIGHT = 960;  
    sf::RenderWindow m_window;

    // Menu items
    sf::Font m_font;
    sf::Text m_titleText;
    sf::Text m_playText;
    sf::Text m_cardGameText;
    sf::Text m_quitText;
    int m_selectedMenuIndex;

    // Map settings
    static const int MAP_WIDTH = 8;
    static const int MAP_HEIGHT = 8;
    int m_worldMap[MAP_WIDTH][MAP_HEIGHT]; //2 arrays

    // Player variables
    float m_playerX = 4.0f; //players initial position x
    float m_playerY = 4.0f; //players initial position y
    float m_playerAngle = 0.0f; //the players angle
    const float m_playerStrafeRadians = 1.57079633f;
    const float FOV = 3.14159f / 3.0f; // 60 degrees in radians, because some joker loves making me use radians 

    // Mouse Variables
    const int m_mousePosX = 640;
    const int m_mousePosY = 480;
    sf::Vector2i mousePosition{m_mousePosX, m_mousePosY};

    // Game functions
    void processEvents();
    void update();
    void render();
    void initializeMap();
    void mouseMovement();

    // Menu functions
    void initializeMenu();
    void processMenuEvents();
    void renderMenu();
    void runCardGame();
};