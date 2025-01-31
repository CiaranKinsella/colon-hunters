// Ciaran Kinsella
// 28/01/2025

// Hello my name is Ciaran Kinsella. this is my Project im working on called Colon Hunters. so far ive only implemented ray casting here with online resources.
// lots of online resources used, Resources left in the READ ME txt File.
//as i said alot of this stuff was me reaserching and getting stuff online, trying to understand it fully.

// Include the files that we need
#include <vector>
#include "Game.h"
#include <iostream>
#include <cmath>

// Constructor for the Game class. This sets up the game window and other initial settings.
Game::Game() :
    m_window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Colon Hunters", sf::Style::Close),
    m_gameState(GameState::Menu), // Starts in the menu state
    m_selectedMenuIndex(0) // Starts with the first menu option selected which i set to zero
{
    m_window.setFramerateLimit(60); // Limits the frame rate to 60 frames per second
    initializeMap(); // Initializes the game world (the map)
    initializeMenu(); // Initializes the main menu (title and buttons)
}

// Destructor. Currently does nothing, but can be used to clean up resources later. 
// a good link to see this explained is https://www.geeksforgeeks.org/destructors-c/
Game::~Game() 
{

}

void Game::initializeMenu() {
    // Load font
    if (!m_font.loadFromFile("ASSETS\\FONTS\\ariblk.ttf")) 
    {
        std::cout << "Error loading font" << std::endl;
    }

    // Setup title text
    m_titleText.setFont(m_font); // Set the font for the title text
    m_titleText.setString("Colon Hunters"); // Set the text to be displayed
    m_titleText.setCharacterSize(100);  // Set the size of the text
    m_titleText.setFillColor(sf::Color::White);// Set the text color to white

    // Center the title
    sf::FloatRect titleBounds = m_titleText.getLocalBounds();
    m_titleText.setPosition
    (
        (SCREEN_WIDTH - titleBounds.width) / 2, // Center horizontally
        SCREEN_HEIGHT * 0.25f  // Set a vertical position (25% of the screen height)
    );

    // Setup Play button
    m_playText.setFont(m_font); // Set the font for the Play button, same as all fonts
    m_playText.setString("Play"); // Play the game
    m_playText.setCharacterSize(70);  // size
    sf::FloatRect playBounds = m_playText.getLocalBounds();
    m_playText.setPosition
    (
        (SCREEN_WIDTH - playBounds.width) / 2, // Centers horizontally?
        SCREEN_HEIGHT * 0.5f
    );

    m_cardGameText.setFont(m_font);
    m_cardGameText.setString("Card Game");
    m_cardGameText.setCharacterSize(70);
    sf::FloatRect cardBounds = m_cardGameText.getLocalBounds();
    m_cardGameText.setPosition(
        (SCREEN_WIDTH - cardBounds.width) / 2,
        SCREEN_HEIGHT * 0.58f  // Positioned between Play and Quit
    );

    // Setup Quit button
    m_quitText.setFont(m_font); 
    m_quitText.setString("Quit");
    m_quitText.setCharacterSize(70);  // Increased from 40
    sf::FloatRect quitBounds = m_quitText.getLocalBounds();
    m_quitText.setPosition
    (
        (SCREEN_WIDTH - quitBounds.width) / 2,
        SCREEN_HEIGHT * 0.65f  // Adjusted from 0.6f
    );
}

// This function initializes the game world (the map), which consists of walls and empty spaces.
void Game::initializeMap() {
    // Create a simple map (1 = wall, 0 = empty)
    const int MAP[MAP_HEIGHT][MAP_WIDTH] = {
        {1,1,1,1,1,1,1,1},
        {1,0,0,0,0,0,0,1},
        {1,0,0,0,0,1,0,1},
        {1,0,0,0,0,0,0,1},
        {1,0,0,1,0,0,0,1},
        {1,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,1},
        {1,1,1,1,1,1,1,1}
    };

    // Copy the map into the game's world map (m_worldMap)
    for (int y = 0; y < MAP_HEIGHT; y++) 
    {
        for (int x = 0; x < MAP_WIDTH; x++) 
        {
            m_worldMap[x][y] = MAP[y][x];
        }
    }
}

void Game::run() 
{
    while (m_window.isOpen()) 
    {
        if (m_gameState == GameState::Menu) 
        { //When the game runs we start at gamestate menu here, so we are in the main menu
            processMenuEvents(); //process what happens at the main menu
            renderMenu();
        }
        else if (m_gameState == GameState::Playing) 
        { //now were in the game
            processEvents();
            mouseMovement();
            update();
            render();
            m_window.setMouseCursorVisible(false);
        }
        else if (m_gameState == GameState::CardGame) {
            runCardGame();
        }
    }
}

void Game::mouseMovement()
{
    // ok so heres the handle movement code here.
    float mouseDistance = 0;

    if (sf::Mouse::getPosition().x < (SCREEN_WIDTH / 2))
    {
        mouseDistance = m_mousePosX - sf::Mouse::getPosition().x;
        mouseDistance = mouseDistance / 1000;

        m_playerAngle -= mouseDistance; //turn player left
    }
    if (sf::Mouse::getPosition().x > (SCREEN_WIDTH / 2))
    {
        mouseDistance = sf::Mouse::getPosition().x - m_mousePosX;
        mouseDistance = mouseDistance / 1000;

        m_playerAngle += mouseDistance; //now turn him right
    }

    sf::Mouse::setPosition(mousePosition);


}

void Game::processEvents() 
{
    sf::Event event;
    while (m_window.pollEvent(event)) 
    {
        if (event.type == sf::Event::Closed || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)) 
        {
            m_window.close(); // basically all the things and keypresses that will close the window here
        }
    }

    // ok so heres the handle movement code here.

    float moveSpeed = 0.05f;// put thsi in header file later dummy
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
    {
        float newX = m_playerX + cos(m_playerAngle) * moveSpeed; // new player position x and y
        float newY = m_playerY + sin(m_playerAngle) * moveSpeed;

        if (m_worldMap[int(newX)][int(m_playerY)] == 0)
        {
            m_playerX = newX;  //its checking our 2 map arrays. if its equal to zero we can mive there. if its equal
            //to 1 (wall) we cant
        }
        if (m_worldMap[int(m_playerX)][int(newY)] == 0)
        {
            m_playerY = newY;
        }
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) 
    {
        float newX = m_playerX - cos(m_playerAngle) * moveSpeed;
        float newY = m_playerY - sin(m_playerAngle) * moveSpeed;
        if (m_worldMap[int(newX)][int(m_playerY)] == 0)
        {
            m_playerX = newX;
        }

        if (m_worldMap[int(m_playerX)][int(newY)] == 0)
        {
            m_playerY = newY;
        }

    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        float newX = m_playerX - cos(m_playerAngle + m_playerStrafeRadians) * moveSpeed; // new player position x and y
        float newY = m_playerY - sin(m_playerAngle + m_playerStrafeRadians) * moveSpeed;

        if (m_worldMap[int(newX)][int(m_playerY)] == 0)
        {
            m_playerX = newX;  //its checking our 2 map arrays. if its equal to zero we can mive there. if its equal
            //to 1 (wall) we cant
        }
        if (m_worldMap[int(m_playerX)][int(newY)] == 0)
        {
            m_playerY = newY;
        }
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        float newX = m_playerX + cos(m_playerAngle + m_playerStrafeRadians) * moveSpeed; // new player position x and y
        float newY = m_playerY + sin(m_playerAngle + m_playerStrafeRadians) * moveSpeed;

        if (m_worldMap[int(newX)][int(m_playerY)] == 0)
        {
            m_playerX = newX;  //its checking our 2 map arrays. if its equal to zero we can mive there. if its equal
            //to 1 (wall) we cant
        }
        if (m_worldMap[int(m_playerX)][int(newY)] == 0)
        {
            m_playerY = newY;
        }
    }
}

void Game::update() 
{
   // im gonna be honest im too tired right now to add the movement into update man.
}

void Game::render() 
{
    m_window.clear(sf::Color(100, 100, 100)); // Sky color

    // Draw floor
    sf::RectangleShape floor(sf::Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT / 2)); //formula for drawing floors aparently
    floor.setPosition(0, SCREEN_HEIGHT / 2);
    floor.setFillColor(sf::Color(80, 80, 80));
    m_window.draw(floor);

    // Cast rays
    float rayAngle = m_playerAngle - FOV / 2; //inital left most ray is what i draw here
    float rayStep = FOV / float(SCREEN_WIDTH); //steps the rays out until it reaches the FOV 

    for (int x = 0; x < SCREEN_WIDTH; x++) 
    {
        // Ray direction
        float dirX = cos(rayAngle);
        float dirY = sin(rayAngle);

        // Current position
        float rayX = m_playerX;
        float rayY = m_playerY;

        // Step size
        float stepSize = 0.01f;
        float distance = 0.0f;
        bool hitWall = false;

        // Cast ray until we hit a wall
        while (!hitWall && distance < 20.0f) 
        {
            distance += stepSize;
            rayX = m_playerX + dirX * distance;
            rayY = m_playerY + dirY * distance;

            // Check if ray hit a wall
            if (m_worldMap[int(rayX)][int(rayY)] == 1) 
            {
                hitWall = true;
            }
        }

        // Fix fisheye effect
        distance = distance * cos(rayAngle - m_playerAngle);

        // Calculate wall height
        int wallHeight = int(SCREEN_HEIGHT / distance);

        if (wallHeight > SCREEN_HEIGHT)
        {
            wallHeight = SCREEN_HEIGHT;
        }

        // Draw wall slice
        int wallTop = (SCREEN_HEIGHT - wallHeight) / 2;
        sf::RectangleShape wall(sf::Vector2f(1, wallHeight));
        wall.setPosition(x, wallTop);

        // Shade walls based on distance
        int shade = 255 - int(distance * 20);
        if (shade < 0) shade = 0;
        wall.setFillColor(sf::Color(shade, shade, shade));

        m_window.draw(wall);
        rayAngle += rayStep;
    }

    // Draw 2D minimap so we can see our player (put this in function to toggle on later??
    float minimapScale = 0.2f;
    int minimapSize = int(SCREEN_WIDTH * minimapScale);
    int cellSize = minimapSize / MAP_WIDTH;

    // Draw map cells
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            if (m_worldMap[x][y] == 1) {
                sf::RectangleShape cell(sf::Vector2f(cellSize - 1, cellSize - 1));
                cell.setPosition(x * cellSize, y * cellSize);
                cell.setFillColor(sf::Color::White);
                m_window.draw(cell);
            }
        }
    }

    // Draw player on minimap
    sf::CircleShape player(3);
    player.setFillColor(sf::Color::Red);
    player.setPosition(m_playerX * cellSize - 3, m_playerY * cellSize - 3);
    m_window.draw(player);

    m_window.display();
}

//all stuff for main menu stuff
void Game::processMenuEvents() 
{
    sf::Event event;
    while (m_window.pollEvent(event)) 
    {
        if (event.type == sf::Event::Closed || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)) 
        {
            m_window.close();
        }

        // Handle menu navigation
        if (event.type == sf::Event::KeyPressed) 
        {
            if (event.key.code == sf::Keyboard::Up) 
            {
                m_selectedMenuIndex--;
                if (m_selectedMenuIndex < 0) m_selectedMenuIndex = 1; // Wrap around
            }
            if (event.key.code == sf::Keyboard::Down) 
            {
                m_selectedMenuIndex++;
                if (m_selectedMenuIndex > 1) m_selectedMenuIndex = 0; // Wrap around
            }

            // Handle selection
            if (event.key.code == sf::Keyboard::Enter) 
            {
                if (m_selectedMenuIndex == 0) 
                {
                    m_gameState = GameState::Playing; // Start the game
                }
                else if (m_selectedMenuIndex == 1) {
                    m_gameState = GameState::CardGame; // play card game
                }
                else if (m_selectedMenuIndex == 2) {
                    m_window.close(); // Quit game
                }
            }
        }
    }
}

void Game::renderMenu() 
{
    m_window.clear(sf::Color(100, 100, 100)); // Clear screen with a background color

    // Draw the title
    m_window.draw(m_titleText);

    // Draw the "Play" button
    if (m_selectedMenuIndex == 0) 
    {
        m_playText.setFillColor(sf::Color::Red);  // Highlight selected option
    }
    else 
    {
        m_playText.setFillColor(sf::Color::White);
    }
    m_window.draw(m_playText);

    if (m_selectedMenuIndex == 1)
    {
        m_cardGameText.setFillColor(sf::Color::Red);  // Highlight selected option
    }
    else
    {
        m_cardGameText.setFillColor(sf::Color::White);
    }

    // Draw the "Quit" button
    if (m_selectedMenuIndex == 2) 
    {
        m_quitText.setFillColor(sf::Color::Red);  // Highlight selected option
    }
    else
    {
        m_quitText.setFillColor(sf::Color::White);
    }
    m_window.draw(m_quitText);
    m_window.draw(m_cardGameText);
    m_window.display();
}

void Game::runCardGame()
{

}
