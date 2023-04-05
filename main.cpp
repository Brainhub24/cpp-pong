#include <SFML/Graphics.hpp>
#include <cmath>
#include <string>

void moveOpponentPaddle( sf::RectangleShape& paddle, sf::CircleShape& ball ) {
    float opponentPaddleSpeed = 6.2f;
    float paddle_middle = paddle.getPosition().y + ( paddle.getSize().y / 2.0f );

    if( ball.getPosition().y < ( paddle_middle - 55 ) ) {
        paddle.move(sf::Vector2f(0.0f, -opponentPaddleSpeed));
    } else if( ball.getPosition().y > ( paddle_middle + 55 ) ) {
        paddle.move(sf::Vector2f(0.0f, opponentPaddleSpeed));
    }
}

void handleInput(sf::RectangleShape& paddle, float paddleSpeed)
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
    {
        paddle.move(sf::Vector2f(0.0f, -paddleSpeed));
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
    {
        paddle.move(sf::Vector2f(0.0f, paddleSpeed));
    }
}

void checkCollision(sf::CircleShape& ball, sf::Vector2f& ballVelocity, sf::RectangleShape& leftPaddle, sf::RectangleShape& rightPaddle, sf::RenderWindow& window, int& player_points, int& opponent_points)
{
    if (ball.getPosition().y < 0.0f)
    {
        ball.setPosition(sf::Vector2f(ball.getPosition().x, 0.0f));
        ballVelocity.y = -ballVelocity.y;
    }
    if (ball.getPosition().y > 590.0f)
    {
        ball.setPosition(sf::Vector2f(ball.getPosition().x, 590.0f));
        ballVelocity.y = -ballVelocity.y;
    }
    if (ball.getPosition().x < 0.0f)
    {
        ball.setPosition(sf::Vector2f(400.0f, 300.0f));
        ballVelocity.x = -ballVelocity.x;
        opponent_points += 1;
    }
    if (ball.getPosition().x > 790.0f)
    {
        ball.setPosition(sf::Vector2f(400.0f, 300.0f));
        ballVelocity.x = -ballVelocity.x;
        player_points += 1;
    }
    if (ball.getGlobalBounds().intersects(leftPaddle.getGlobalBounds()))
    {
        float relativeIntersectY = leftPaddle.getPosition().y + (leftPaddle.getSize().y / 2.0f) - ball.getPosition().y;
        float normalizedRelativeIntersectionY = relativeIntersectY / (leftPaddle.getSize().y / 2.0f);
        float bounceAngle = normalizedRelativeIntersectionY * (3.14159265359f / 3.0f);
        ballVelocity.x = std::abs(ballVelocity.x) * std::cos(bounceAngle);
        ballVelocity.y = -std::abs(ballVelocity.x) * std::sin(bounceAngle);
    }
    if (ball.getGlobalBounds().intersects(rightPaddle.getGlobalBounds()))
    {
        float relativeIntersectY = rightPaddle.getPosition().y + (rightPaddle.getSize().y / 2.0f) - ball.getPosition().y;
        float normalizedRelativeIntersectionY = relativeIntersectY / (rightPaddle.getSize().y / 2.0f);
        float bounceAngle = normalizedRelativeIntersectionY * (3.14159265359f / 3.0f);
        ballVelocity.x = -std::abs(ballVelocity.x) * std::cos(bounceAngle);
        ballVelocity.y = -std::abs(ballVelocity.x) * std::sin(bounceAngle);
    }
}

int main()
{
    int player_points = 0;
    int opponent_points = 0;

    sf::RenderWindow window(sf::VideoMode(800, 600), "Pong");

    // Create a font object and load a font file
    sf::Font font;
    if (!font.loadFromFile("Ubuntu-Regular.ttf"))
    {
        return 1;
    }

    // Create rectangle shapes for the paddles
    sf::RectangleShape leftPaddle(sf::Vector2f(20.0f, 100.0f));
    leftPaddle.setFillColor(sf::Color::White);
    leftPaddle.setPosition(sf::Vector2f(50.0f, 250.0f));

    sf::RectangleShape rightPaddle(sf::Vector2f(20.0f, 100.0f));
    rightPaddle.setFillColor(sf::Color::White);
    rightPaddle.setPosition(sf::Vector2f(730.0f, 250.0f));

    // Create a circle shape for the ball
    sf::CircleShape ball(10.0f);
    ball.setFillColor(sf::Color::White);
    ball.setPosition(sf::Vector2f(400.0f, 300.0f));
    sf::Vector2f ballVelocity(-2.0f, 0.0f);

    // Define the paddle speed and ball speed
    float paddleSpeed = 6.2f;
    float ballSpeed = 2.5f;

    // Define the game update rate
    const float gameUpdateRate = 1.0f / 60.0f;

    sf::Clock clock;
    float timeSinceLastUpdate = 0.0f;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        bool game_over = (opponent_points - player_points) > 6;

        // Update the game if enough time has elapsed since the last update
        timeSinceLastUpdate += clock.restart().asSeconds();
        while ( ! game_over && timeSinceLastUpdate >= gameUpdateRate)
        {
            handleInput(leftPaddle, paddleSpeed);
            checkCollision(ball, ballVelocity, leftPaddle, rightPaddle, window, player_points, opponent_points);
            moveOpponentPaddle( rightPaddle, ball );

            // Get the current speed of the ball
            float currentSpeed = std::sqrt(std::pow(ballVelocity.x, 2) + std::pow(ballVelocity.y, 2));

            // Normalize the ball's velocity to have a magnitude of ballSpeed
            ballVelocity.x *= ballSpeed / currentSpeed;
            ballVelocity.y *= ballSpeed / currentSpeed;

            // Move the ball
            ball.move(ballVelocity * ballSpeed);

            timeSinceLastUpdate -= gameUpdateRate;
        }

        // Create a text object for the player score
        sf::Text playerScoreText;
        playerScoreText.setFont(font);
        playerScoreText.setCharacterSize(64);
        playerScoreText.setFillColor(sf::Color::White);
        playerScoreText.setString(std::to_string( player_points ));
        playerScoreText.setPosition(300.0f, 20.0f);

        // Create a text object for the opponent score
        sf::Text opponentScoreText;
        opponentScoreText.setFont(font);
        opponentScoreText.setCharacterSize(64);
        opponentScoreText.setFillColor(sf::Color::White);
        opponentScoreText.setString(std::to_string(opponent_points));
        opponentScoreText.setPosition(480.0f, 20.0f);

        // Set background to black
        window.clear(sf::Color::Black);

        // Draw the player score
        window.draw(playerScoreText);

        // Draw the opponent score
        window.draw(opponentScoreText);

        // Draw the paddles
        window.draw(leftPaddle);
        window.draw(rightPaddle);

        // Draw the ball
        window.draw(ball);

        if( game_over ) {
            // Create a text object for the opponent score
            sf::Text gameOverText;
            gameOverText.setFont(font);
            gameOverText.setCharacterSize(74);
            gameOverText.setFillColor(sf::Color::White);
            gameOverText.setString("GAME OVER");
            gameOverText.setPosition(190.0f, 250.0f);
            window.draw(gameOverText);
        }

        window.display();
    }

    return 0;
}
