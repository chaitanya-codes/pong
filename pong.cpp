#include <iostream>
#include <SFML/Graphics.hpp>
#include <windows.h>

using namespace sf;
using std::cout;

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 300;
const float BALL_SPEED_X = 6.0f;
const float BALL_SPEED_Y = 2.0f;
const int BAT_SPEED = 5;

const float batWidth = 1.0f;
const float batHeight = 3.0f;
const float FPS = 60;

int main() {

	RenderWindow window(VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Pong");

	Event event;
	Text score1,score2,gameover;
	Font font;
	font.loadFromFile("C:/Development/Fonts/Poppins/Poppins-Bold.ttf");

	score1.setFont(font); score2.setFont(font); gameover.setFont(font);
	score1.setCharacterSize(20); score2.setCharacterSize(20); gameover.setCharacterSize(20);
	score1.setString("Player 1: 0"); score2.setString("Player 2: 0"); gameover.setString("");
	score1.setPosition(10, 5); score2.setPosition(WINDOW_WIDTH - 120, 5); gameover.setPosition(WINDOW_WIDTH / 2 - 60, 140);
	score1.setFillColor(sf::Color::Green); score2.setFillColor(sf::Color::Blue); gameover.setFillColor(sf::Color::Cyan);

	Texture ball;
	Texture bat1, bat2;
	Texture background;
	Texture restart, restarted;
	ball.loadFromFile("images/ball.png");
	bat1.loadFromFile("images/bat.png");
	bat2.loadFromFile("images/bat.png");
	background.loadFromFile("images/background.png");
	restart.loadFromFile("images/restart.png");
	restarted.loadFromFile("images/restarted.png");
	Sprite sBall(ball);
	Sprite sBat1(bat1), sBat2(bat2);
	Sprite sBackground(background), sRestart(restart), sRestarted(restarted);

	bool goRightFirst = true;
	srand(time(0));
	if (rand() % 2) goRightFirst = true;
	else goRightFirst = false;

	sBall.setPosition(goRightFirst ? WINDOW_WIDTH/3 : WINDOW_WIDTH/1.5, WINDOW_HEIGHT/3);
	sBall.setScale(0.4,0.4);
	
	sBat1.setPosition(0, 80);
	sBat1.setScale(batWidth, batHeight);
	sBat2.setPosition(WINDOW_WIDTH-5, 80);
	sBat2.setScale(batWidth, batHeight);

	sRestart.setPosition(WINDOW_WIDTH / 2 - 60, 160);
	sRestarted.setPosition(WINDOW_WIDTH / 2 - 60, 170);

	cout << "Window size: " << window.getPosition().x << ' ' << window.getPosition().y << '\n';

	Vector2f velocity((goRightFirst ? BALL_SPEED_X : -BALL_SPEED_X), BALL_SPEED_Y);
	
	int player1Score = 0, player2Score = 0, seconds;
	bool gameOver = false;
	Clock clock;

	while (window.isOpen()) {
		while (window.pollEvent(event)) {
			if (event.type == Event::Closed) window.close();
			if (event.type == Event::KeyPressed) {
				if (Keyboard::isKeyPressed(Keyboard::W) && sBat1.getPosition().y > 0) {
					sBat1.move(0, -BAT_SPEED);
				}
				if (Keyboard::isKeyPressed(Keyboard::S) && sBat1.getPosition().y < WINDOW_HEIGHT - sBat1.getGlobalBounds().height) {
					sBat1.move(0, BAT_SPEED);
				}
				if (Keyboard::isKeyPressed(Keyboard::Up) && sBat2.getPosition().y > 0) {
					sBat2.move(0, -BAT_SPEED);
				}
				if (Keyboard::isKeyPressed(Keyboard::Down) && sBat2.getPosition().y < WINDOW_HEIGHT - sBat2.getGlobalBounds().height) {
					sBat2.move(0, BAT_SPEED);
				}
			} else if (gameOver && event.type == Event::MouseButtonPressed) {
				if (event.key.code == 0) {
					gameOver = false;
					if (rand() % 2) goRightFirst = true;
					else goRightFirst = false;
					sBall.setPosition(goRightFirst ? WINDOW_WIDTH / 3 : WINDOW_WIDTH / 1.5, WINDOW_HEIGHT/3);
					velocity.x = goRightFirst ? BALL_SPEED_X : -BALL_SPEED_X;
					velocity.y = BALL_SPEED_Y;
					gameover.setString("");
					player1Score = 0; player2Score = 0;
					score1.setString("Player 1: " + std::to_string(player1Score));
					score2.setString("Player 2: " + std::to_string(player2Score));
					window.draw(sRestarted);
				}
			}
		}

		if (!gameOver) {
			seconds = clock.getElapsedTime().asMilliseconds();
			if (seconds > FPS / 5) {
				seconds = 0;
				clock.restart();
				sBall.move(velocity);

				if (sBall.getGlobalBounds().intersects(sBat1.getGlobalBounds()) || sBall.getGlobalBounds().intersects(sBat2.getGlobalBounds())) std::cout << "Touched bat";
				if ((sBall.getPosition().x < (0+batWidth) || sBall.getPosition().x > (window.getSize().x - sBall.getGlobalBounds().width))) {
					velocity.x = -velocity.x;
					if ((sBall.getGlobalBounds().intersects(sBat1.getGlobalBounds()))) {
						player1Score++;
						score1.setString("Player 1: " + std::to_string(player1Score));
					} else if (sBall.getGlobalBounds().intersects(sBat2.getGlobalBounds())) {
						player2Score++;
						score2.setString("Player 2: " + std::to_string(player2Score));
					} else {
						gameOver = true;
					}
				}
				if (sBall.getPosition().y < 0 || sBall.getPosition().y > (window.getSize().y - sBall.getGlobalBounds().height)) {
					velocity.y = -velocity.y;
				}
			}
		}
		else {
			velocity.x = 0;
			velocity.y = 0;
			std::string won;
			if (player1Score > player2Score) won = "Player 1 won!\n";
			else if (player1Score < player2Score) won = "Player 2 won!\n";
			else won = "It's a draw!\n";
			gameover.setString("Game Over!\n" + won + "\nClick to restart!");
			window.draw(gameover);
			window.draw(sRestart);
		}

		window.clear(Color::White);
		window.draw(sBackground);
		window.draw(sBall);
		window.draw(sBat1); window.draw(sBat2);
		window.draw(score1); window.draw(score2);
		window.draw(gameover);
		window.display();
	}
	 
	return 0;
}