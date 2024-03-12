#include <iostream>
#include <SFML/Graphics.hpp>
#include <windows.h>

using namespace sf;
using std::cout;
using std::string;

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 300;
const float BALL_SPEED_X = 6.5f;
const float BALL_SPEED_Y = 3.0f;
const int BAT_SPEED = 5;

const float batWidth = 1.0f;
const float batHeight = 3.0f;
const float FPS = 60;

Font font;

void resetBallPosition(bool &goRightFirst, Sprite &sBall, Vector2f &velocity) {
	if (rand() % 2) goRightFirst = true;
	else goRightFirst = false;
	sBall.setPosition(goRightFirst ? WINDOW_WIDTH / 3 : WINDOW_WIDTH / 1.5, WINDOW_HEIGHT / 3);
	velocity.x = goRightFirst ? BALL_SPEED_X : -BALL_SPEED_X;
	velocity.y = BALL_SPEED_Y;
}

class Button {
	public:
		Text text;
		Texture button, buttonPressed;
		Sprite sButton;
		
		Button(Vector2f position, string text_, bool scaleWide = false) {
			text.setString(text_);
			Vector2f textLocation(position.x + 2, position.y + 2);
			text.setPosition(textLocation);
			text.setCharacterSize(30);
			text.setFillColor(sf::Color::Blue);
			text.setFont(font);
			sButton.setPosition(position);
			sButton.setScale(0.7, 0.5);
			if (scaleWide) sButton.scale(1.2,1);
			if (!button.loadFromFile("images/button.png")) std::cerr << "Error loading button.png\n";
			if (!buttonPressed.loadFromFile("images/buttonPressed.png")) std::cerr << "Error loading buttonPressed.png\n";
		}

		void display(RenderWindow &window) {
			sButton.setTexture(button);
			window.draw(sButton);
			window.draw(text);
		}

		void press(RenderWindow &window) {
			sButton.setTexture(buttonPressed);
			sButton.move(2, 2);
			text.move(2, 2);
			window.draw(sButton);
			window.draw(text);
		}
		FloatRect globalBounds() {
			return this->sButton.getGlobalBounds();
		}
};

int main() {

	RenderWindow window(VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Pong");

	Event event;
	Text score1, score2, gameover;

	font.loadFromFile("C:/Development/Fonts/Poppins/Poppins-Bold.ttf");
	score1.setFont(font); score2.setFont(font); gameover.setFont(font);
	score1.setCharacterSize(20); score2.setCharacterSize(20); gameover.setCharacterSize(20);
	score1.setString("Player 1: 0"); score2.setString("Player 2: 0"); gameover.setString("");
	score1.setPosition(10, 5); score2.setPosition(WINDOW_WIDTH - 120, 5); gameover.setPosition(WINDOW_WIDTH / 2 - 60, 140);
	score1.setFillColor(sf::Color::Green); score2.setFillColor(sf::Color::Blue); gameover.setFillColor(sf::Color::Cyan);

	Texture ball;
	Texture bat1, bat2;
	Texture background;
	ball.loadFromFile("images/ball.png");
	bat1.loadFromFile("images/bat.png");
	bat2.loadFromFile("images/bat.png");
	background.loadFromFile("images/background.png");

	Vector2f restartPos(WINDOW_WIDTH/2 - 70, WINDOW_HEIGHT-50);
	Button restart(restartPos, "Restart");
	Button pvpButton({ WINDOW_WIDTH / 3 - 70, WINDOW_HEIGHT / 2 }, "   Player");
	Button computerButton({ WINDOW_WIDTH / 3 + 140, WINDOW_HEIGHT / 2 }, "Computer", true);

	Sprite sBall(ball);
	Sprite sBat1(bat1), sBat2(bat2);
	Sprite sBackground(background);

	bool goRightFirst = true;
	Vector2f velocity((goRightFirst ? BALL_SPEED_X : -BALL_SPEED_X), BALL_SPEED_Y);
	resetBallPosition(goRightFirst, sBall, velocity);

	sBall.setScale(0.4,0.4);

	sBat1.setPosition(0, 80);
	sBat1.setScale(batWidth, batHeight);
	sBat2.setPosition(WINDOW_WIDTH-5, 80);
	sBat2.setScale(batWidth, batHeight);

	cout << "Window size: " << window.getPosition().x << ' ' << window.getPosition().y << '\n';

	int player1Score = 0, player2Score = 0, seconds;
	bool gameOver = false, gamemodeChosen = false, pvp = true;
	Clock clock;

	while (window.isOpen()) {
		while (window.pollEvent(event)) {
			if (event.type == Event::Closed) window.close();
			if (event.type == Event::KeyPressed && gamemodeChosen) {
				if (Keyboard::isKeyPressed(Keyboard::W) && sBat1.getPosition().y > 0) {
					sBat1.move(0, -BAT_SPEED);
				}
				if (Keyboard::isKeyPressed(Keyboard::S) && sBat1.getPosition().y < WINDOW_HEIGHT - sBat1.getGlobalBounds().height) {
					sBat1.move(0, BAT_SPEED);
				}
				if (Keyboard::isKeyPressed(Keyboard::Up) && sBat2.getPosition().y > 0 && pvp) {
					sBat2.move(0, -BAT_SPEED);
				}
				if (Keyboard::isKeyPressed(Keyboard::Down) && sBat2.getPosition().y < WINDOW_HEIGHT - sBat2.getGlobalBounds().height && pvp) {
					sBat2.move(0, BAT_SPEED);
				}
			}
			else if (event.type == Event::MouseButtonPressed && event.key.code == 0) {
				Vector2f mousePos = Vector2f(Mouse::getPosition(window));
				if (!gamemodeChosen) {
					if (pvpButton.globalBounds().contains(mousePos)) {
						pvp = true;
						pvpButton.press(window);
						gamemodeChosen = true;
					} else if (computerButton.globalBounds().contains(mousePos)) {
						pvp = false;
						computerButton.press(window);
						gamemodeChosen = true;
					}
				} else if (gameOver && restart.globalBounds().contains(mousePos)) {
					gameOver = false;
					resetBallPosition(goRightFirst, sBall, velocity);
					gameover.setString("");
					player1Score = 0; player2Score = 0;
					score1.setString("Player 1: " + std::to_string(player1Score));
					score2.setString("Player 2: " + std::to_string(player2Score));
					restart.press(window);
				}
			}
		}
		
		window.clear(Color::White);
		window.draw(sBackground);

		if (!gamemodeChosen) {
			pvpButton.display(window);
			computerButton.display(window);
		} else if (!gameOver) {
			seconds = clock.getElapsedTime().asMilliseconds();
			if (seconds > FPS / 5) {
				seconds = 0;
				clock.restart();
				sBall.move(velocity);

				// Computer
				if (!pvp) {
					if (velocity.y > 0 && sBat2.getPosition().y < WINDOW_HEIGHT - sBat2.getGlobalBounds().height) sBat2.move(0, BAT_SPEED / 2);
					else if (velocity.y < 0 && sBat2.getPosition().y > 0) sBat2.move(0, -BAT_SPEED / 2);
				}

				if (sBall.getGlobalBounds().intersects(sBat1.getGlobalBounds()) || sBall.getGlobalBounds().intersects(sBat2.getGlobalBounds())) std::cout << "Touched bat";
				if (sBall.getPosition().x < (0 + batWidth)) {
					if ((sBall.getGlobalBounds().intersects(sBat1.getGlobalBounds()))) velocity.x = -velocity.x;
					else if (sBall.getPosition().x < 0) {
						player2Score++;
						score2.setString("Player 2: " + std::to_string(player2Score));
						resetBallPosition(goRightFirst, sBall, velocity);
						Sleep(300);
					}
				} else if (sBall.getPosition().x > (window.getSize().x - sBall.getGlobalBounds().width)) {
					if (sBall.getGlobalBounds().intersects(sBat2.getGlobalBounds())) velocity.x = -velocity.x;
					else if (sBall.getPosition().x > window.getSize().x) {
						player1Score++;
						score1.setString("Player 1: " + std::to_string(player1Score));
						resetBallPosition(goRightFirst, sBall, velocity);
						Sleep(300);
					}
				} else if (player1Score + player2Score >= 5) gameOver = true;
				if (sBall.getPosition().y < 0 || sBall.getPosition().y > (window.getSize().y - sBall.getGlobalBounds().height)) {
					velocity.y = -velocity.y;
				}
			}
		} else {
			velocity.x = 0;
			velocity.y = 0;
			std::string won;
			if (player1Score > player2Score) won = "Player 1 won!\n";
			else if (player1Score < player2Score) won = "Player 2 won!\n";
			else won = "It's a draw!\n";
			gameover.setString("Game Over!\n" + won + '\n');
			window.draw(gameover);
			restart.display(window);
		}

		window.draw(sBall);
		window.draw(sBat1); window.draw(sBat2);
		window.draw(score1); window.draw(score2);
		window.draw(gameover);
	
		window.display();
	}
	 
	return 0;
}