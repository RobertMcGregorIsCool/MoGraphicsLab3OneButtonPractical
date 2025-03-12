// platform game..... 
// Author : Robert McGregor

// sf::RectangleShape cloudShape;
//cloudShape.setSize(sf::Vector2f(64, 32));
//cloudShape.setPosition(14.0f * 32.0f, 13.0f * 32.0f); // 14 * 64, 14 * 64);
//cloudShape.setFillColor(sf::Color(200, 200, 200, 255));
// level[row][col].move(cloudMoveSpeedX, 0.0f);

#ifdef _DEBUG  

#pragma comment(lib,"sfml-graphics-d.lib")  
#pragma comment(lib,"sfml-audio-d.lib")  
#pragma comment(lib,"sfml-system-d.lib")  
#pragma comment(lib,"sfml-window-d.lib")  
#pragma comment(lib,"sfml-network-d.lib")  

#else  

#pragma comment(lib,"sfml-graphics.lib")  
#pragma comment(lib,"sfml-audio.lib")  
#pragma comment(lib,"sfml-system.lib")  
#pragma comment(lib,"sfml-window.lib")  
#pragma comment(lib,"sfml-network.lib")  

#endif  

#include <SFML/Graphics.hpp> 
#include <iostream> 
#include <time.h>  

sf::Font fontSilkScreen;
sf::Text textLevelComplete; // text used for message on screen

const float SCREEN_WIDTH = 800;
const float SCREEN_HEIGHT = 600;

bool levelComplete = false;

class Game
{
public:
	//create Window 
	sf::RenderWindow window;
	sf::View view;
	float randomNum;

	float moveXSpeed = -4.0f;
	const float moveXSpeedDefault = -4.0f;
	float moveYSpeed = 0.05f;

	float cloudMoveSpeedX = -1.25f;

	float counterTurnMax = 20.0f;
	float counterTurnCur = 0.0f;

	sf::RectangleShape playerShape;

	sf::RectangleShape sunShape;

	float velocityX = 0, velocityY = 0, gravity = 0.3;
	float jumpVelocity = -8.0f;

	static const int numRows = 20;

	static const int numCols = 40;

	int levelData[numRows][numCols] =
	{
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,3,3,3,3,0 },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,6,6,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
	{ 4,1,1,1,5,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
	{ 1,1,1,1,1,1,1,0,0,1,1,1,1,1,0,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,5,0,0,1,1,1,4,1 },
	{ 1,1,1,1,1,1,1,0,0,1,1,1,1,1,0,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,0,0,1,1,1,1,4 },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
	 };

	sf::RectangleShape levelRects[numRows][numCols];

	Game()
	{
		window.create(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Endless Runner Game");
	}

	void init()
	{
		moveXSpeed = moveXSpeedDefault;

		playerShape.setSize(sf::Vector2f(20, 20));

		playerShape.setPosition(SCREEN_WIDTH * 0.5f, 475.0f);

		playerShape.setFillColor(sf::Color::Blue);

		sunShape.setSize(sf::Vector2f(64, 64));
		sunShape.setPosition(SCREEN_WIDTH * 0.75f, SCREEN_HEIGHT * 0.125f);
		sunShape.setFillColor(sf::Color::Yellow);

		for (int row = 0; row < numRows; row++)
		{
			for (int col = 0; col < numCols; col++)
			{
				if (levelData[row][col] == 1) // GREEN GROUND BLOCK
				{
					levelRects[row][col].setSize(sf::Vector2f(32, 32));
					levelRects[row][col].setPosition(col * 32, row * 32);
					levelRects[row][col].setFillColor(sf::Color::Green);
				}

				if (levelData[row][col] == 0) // SKY EMPTY BLOCK
				{
					levelRects[row][col].setSize(sf::Vector2f(32, 32));
					levelRects[row][col].setPosition(col * 32, row * 32);
					levelRects[row][col].setFillColor(sf::Color(67, 130, 232));
				}

				if (levelData[row][col] == 2) // RED DEADLY BLOCK
				{
					levelRects[row][col].setSize(sf::Vector2f(32, 32));
					levelRects[row][col].setPosition(col * 32, row * 32);
					levelRects[row][col].setFillColor(sf::Color::Red);
				}

				if (levelData[row][col] == 3) // GOLD GOAL BLOCK
				{
					levelRects[row][col].setSize(sf::Vector2f(32, 32));
					levelRects[row][col].setPosition(col * 32, row * 32);
					levelRects[row][col].setFillColor(sf::Color::Yellow);
				}

				if (levelData[row][col] == 4) // MAGENTA REVERSE BLOCK
				{
					levelRects[row][col].setSize(sf::Vector2f(32, 32));
					levelRects[row][col].setPosition(col * 32, row * 32);
					levelRects[row][col].setFillColor(sf::Color::Magenta);
				}

				if (levelData[row][col] == 5) // AQUA JUMP BLOCK
				{
					levelRects[row][col].setSize(sf::Vector2f(32, 32));
					levelRects[row][col].setPosition(col * 32, row * 32);
					levelRects[row][col].setFillColor(sf::Color::Cyan);
				}

				if (levelData[row][col] == 6) // GREY CLOUD/LAVA BLOCK
				{
					levelRects[row][col].setSize(sf::Vector2f(32, 32));
					levelRects[row][col].setPosition(col * 32, row * 32);
					levelRects[row][col].setFillColor(sf::Color(200, 200, 200, 255));
				}
			}
			std::cout << std::endl;
		}
	}

	void run()
	{
		sf::Time timePerFrame = sf::seconds(1.0f / 60.0f);

		sf::Time timeSinceLastUpdate = sf::Time::Zero;

		sf::Clock clock;

		clock.restart();

		while (window.isOpen())
		{
			sf::Event event;

			while (window.pollEvent(event))
			{
				if (event.type == sf::Event::Closed)

					window.close();

			}

			timeSinceLastUpdate += clock.restart();

			if (counterTurnCur > 0.0f)
			{
				counterTurnCur -= timeSinceLastUpdate.asSeconds();
				std::cout << "CounterTurnCur: " << counterTurnCur << "\n\n";
			}

			if (timeSinceLastUpdate > timePerFrame)
			{
				if (!levelComplete)
				{
					for (int row = 0; row < numRows; row++)
					{
						for (int col = 0; col < numCols; col++)
						{
							if (levelData[row][col] == 6)
							{
								levelRects[row][col].move(moveXSpeed + cloudMoveSpeedX, moveYSpeed);
							}
							else
							{
								levelRects[row][col].move(moveXSpeed, moveYSpeed);
							}
						}
					}
				}

				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && velocityY == 0)
				{
					if (levelComplete)
					{
						levelComplete = false;
						init();
					}
					else
					{
						velocityY = jumpVelocity;
					}
				}

				velocityY = velocityY + gravity;

				playerShape.move(0, velocityY);

				gravity = 0.6;

				for (int row = 0; row < numRows; row++)
				{
					for (int col = 0; col < numCols; col++)
					{

						// CLOUD-GROUND COLLISION -------------------------------
						if (levelData[row][col] == 1)
						{
							for (int row2 = 0; row2 < numRows; row2++)
							{
								for (int col2 = 0; col2 < numCols; col2++)
								{
									if (levelData[row2][col2] == 6)
									{
										if (levelRects[row][col].getGlobalBounds().intersects(levelRects[row2][col2].getGlobalBounds()))
										{
											cloudMoveSpeedX *= -1;
										}
									}
								}
							}
						}

						// GOLD BLOCK --------------------------------------------
						if (levelData[row][col] == 3)
						{// If player touches 3 block, mark level as complete.
							if (playerShape.getGlobalBounds().intersects(levelRects[row][col].getGlobalBounds()))
							{
								levelComplete = true;
							}
						}

						// MAGENTA BLOCK --------------------------------------------
						if (levelData[row][col] == 4)
						{// If player touches 4 block, reverse level movement
							if (playerShape.getGlobalBounds().intersects(levelRects[row][col].getGlobalBounds()) && counterTurnCur <= 0.0f)
							{
								counterTurnCur = counterTurnMax;
								moveXSpeed *= -1;
							}
						}

						// CYAN BLOCK --------------------------------------------
						if (levelData[row][col] == 5)
						{// If player touches 5 block, launched into air
							if (playerShape.getGlobalBounds().intersects(levelRects[row][col].getGlobalBounds()))
							{
								velocityY = jumpVelocity * 1.5f;
							}
						}


						// GREEN BLOCK AND OTHER FLOOR BLOCKS --------------------------------------------
						if (velocityY >= 0) // Falling or y stationary
						{
							if (levelData[row][col] == 1 || levelData[row][col] == 3 || levelData[row][col] == 4 || levelData[row][col] == 5)
							{
								if (playerShape.getGlobalBounds().intersects(levelRects[row][col].getGlobalBounds()))
								{// If intersecting a block marked 1
									if (playerShape.getPosition().y < levelRects[row][col].getPosition().y)
									{// If player Y position is above (<) the block
										gravity = 0;

										velocityY = 0;

										playerShape.setPosition(playerShape.getPosition().x, levelRects[row][col].getPosition().y); // Set player Y eq to block

										playerShape.move(0, -playerShape.getGlobalBounds().height); // Move player above block

										break;
									}
									else 
									{
										init();
									}
								}
							}

							if (levelData[row][col] == 6)
							{
								if (playerShape.getGlobalBounds().intersects(levelRects[row][col].getGlobalBounds()))
								{// If intersecting a cloud
									if (playerShape.getPosition().y < levelRects[row][col].getPosition().y)
									{// If player Y position is above (<) the block
										gravity = 0;

										velocityY = 0;

										playerShape.setPosition(playerShape.getPosition().x, levelRects[row][col].getPosition().y); // Set player Y eq to block

										playerShape.move(0, -playerShape.getGlobalBounds().height); // Move player above block

										break;
									}
								}
							}
						}

						// --------------------------------------------
						if (velocityY < 0) // RISING!
						{// If player is jumping up and touches red block (1), restart.
							/*if (levelData[row][col] == 1)
							{
								if (playerShape.getGlobalBounds().intersects(level[row][col].getGlobalBounds()))
								{
									init();
								}
							}*/
						}
						// BLUE BLOCK --------------------------------------------
						if (levelData[row][col] == 2)
						{// If player touches 2 block in any circumstances, restart
							if (playerShape.getGlobalBounds().intersects(levelRects[row][col].getGlobalBounds()))
							{
								init();
							}
						}

						
					}
				}

				// --------------------------------------------
				if (playerShape.getPosition().y > 600)
				{// If player below playable height, restart.
					init();
				}

				window.clear(sf::Color(67, 130, 232));

				for (int row = 0; row < numRows; row++)
				{
					for (int col = 0; col < numCols; col++)
					{
						window.draw(levelRects[row][col]);
					}
				}

				for (int row = 0; row < numRows; row++)
				{
					for (int col = 0; col < numCols; col++)
					{
						if (levelData[row][col] == 6) window.draw(levelRects[row][col]);
					}
				}

				window.draw(sunShape);

				window.draw(playerShape);

				if (levelComplete)
				{
					window.draw(textLevelComplete);
				}

				window.display();

				timeSinceLastUpdate = sf::Time::Zero;
			}
		}
	}
};

int main()
{
	Game game;

	if (!fontSilkScreen.loadFromFile("ASSETS\\FONTS\\slkscr.ttf"))
	{
		std::cout << "problem loading slkscr font" << std::endl;
	}
	textLevelComplete.setFont(fontSilkScreen);
	textLevelComplete.setString("LEVEL COMPLETE!");
	textLevelComplete.setStyle(sf::Text::Underlined | sf::Text::Italic | sf::Text::Bold);
	textLevelComplete.setPosition(0.0f, SCREEN_HEIGHT * 0.25f); //  (SCREEN_WIDTH * 0.5f) - (textLevelComplete.getScale().x), SCREEN_HEIGHT * 0.5);
	textLevelComplete.setCharacterSize(80U);
	textLevelComplete.setOutlineColor(sf::Color::Red);
	textLevelComplete.setFillColor(sf::Color::Black);
	textLevelComplete.setOutlineThickness(3.0f);

	game.init();

	game.run();

	return 0;
}