#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFPhysics.h>
#include <SFML/Audio.hpp>

const int windowWidth = 800;
const int windowHeight = 600;
const float paddleWidth = 10.0f;
const float paddleHeight = 100.0f;
const float ballRadius = 10.0f;
const float paddleSpeed = 5.0f;
const float ballSpeed = 5.0f;

int leftScore = 0;
int rightScore = 0;

int main() {

    SoundBuffer paddleHitBuffer;
    if (!paddleHitBuffer.loadFromFile("paddle.ogg")) {
        std::cerr << "Sound file not found." << std::endl;
        return 1;
    }
    Sound paddleHitSound;
    paddleHitSound.setBuffer(paddleHitBuffer);

    SoundBuffer pointBuffer;
    if (!pointBuffer.loadFromFile("point.ogg")) {
        std::cerr << "Sound file not found." << std::endl;
        return 1;
    }
    Sound pointSound;
    pointSound.setBuffer(pointBuffer);


    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Pong Game");
    window.setFramerateLimit(60);

    // Create paddles and ball
    sf::RectangleShape leftPaddle(sf::Vector2f(paddleWidth, paddleHeight));
    sf::RectangleShape rightPaddle(sf::Vector2f(paddleWidth, paddleHeight));
    sf::CircleShape ball(ballRadius);

    // Create score display text
    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        std::cerr << "Font file not found." << std::endl;
        return 1;
    }

    sf::Text scoreText;
    scoreText.setFont(font);
    scoreText.setCharacterSize(30);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(windowWidth / 2 - 30, 10);

    // Set initial positions
    leftPaddle.setPosition(10, windowHeight / 2 - paddleHeight / 2);
    rightPaddle.setPosition(windowWidth - 10 - paddleWidth, windowHeight / 2 - paddleHeight / 2);
    ball.setPosition(windowWidth / 2 - ballRadius, windowHeight / 2 - ballRadius);

    // Set initial directions
    sf::Vector2f ballDirection(1.0f, 1.0f);
    sf::Vector2f leftPaddleDirection(0.0f, 0.0f);
    sf::Vector2f rightPaddleDirection(0.0f, 0.0f);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // Move left paddle
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
            leftPaddleDirection.y = -paddleSpeed;
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            leftPaddleDirection.y = paddleSpeed;
        else
            leftPaddleDirection.y = 0.0f;

        // Move right paddle
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            rightPaddleDirection.y = -paddleSpeed;
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            rightPaddleDirection.y = paddleSpeed;
        else
            rightPaddleDirection.y = 0.0f;

        // Update paddles and ball positions
        leftPaddle.move(leftPaddleDirection);
        rightPaddle.move(rightPaddleDirection);
        ball.move(ballDirection * ballSpeed);

        // Handle ball collisions with top and bottom boundaries
        if (ball.getPosition().y <= 0 || ball.getPosition().y >= windowHeight - 2 * ballRadius) {
            ballDirection.y = -ballDirection.y;
            //paddleHitSound.play();
        }

        // Ball collision with paddles
        if (ball.getGlobalBounds().intersects(leftPaddle.getGlobalBounds()) ||
            ball.getGlobalBounds().intersects(rightPaddle.getGlobalBounds())) {
            ballDirection.x = -ballDirection.x;
            paddleHitSound.play();
        }


        // Ball out of bounds (scoring)
        if (ball.getPosition().x <= 0) {
            // Right player scores a point
            rightScore++;
            ball.setPosition(windowWidth / 2 - ballRadius, windowHeight / 2 - ballRadius);
            ballDirection.x = 1.0f;
            pointSound.play();
        }
        else if (ball.getPosition().x >= windowWidth - 2 * ballRadius) {
            // Left player scores a point
            leftScore++;
            ball.setPosition(windowWidth / 2 - ballRadius, windowHeight / 2 - ballRadius);
            ballDirection.x = -1.0f;
            pointSound.play();
        }

        // Check if a player has reached a score of 10
        if (leftScore >= 10 || rightScore >= 10) {
            std::string winner = (leftScore >= 10) ? "Left Player" : "Right Player";
            std::cout << winner << " wins!" << std::endl;
            window.close();
        }

        // Update the score display text
        scoreText.setString("Left: " + std::to_string(leftScore) + " - Right: " + std::to_string(rightScore));

        window.clear();
        window.draw(leftPaddle);
        window.draw(rightPaddle);
        window.draw(ball);
        window.draw(scoreText);
        window.display();
    }

    return 0;
}