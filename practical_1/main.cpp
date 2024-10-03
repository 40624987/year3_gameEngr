#include <SFML/Graphics.hpp>
#include <iostream>

using namespace sf;
using namespace std;

// Global variables
Vector2f ballVelocity;
const float initialVelocityX = 100.f;
const float initialVelocityY = 60.f;

const Keyboard::Key controls[4] = {
    Keyboard::A,   // Player1 UP
    Keyboard::Z,   // Player1 Down
    Keyboard::Up,  // Player2 UP
    Keyboard::Down // Player2 Down
};
const Vector2f paddleSize(25.f, 100.f);
const float ballRadius = 10.f;
const int gameWidth = 800;
const int gameHeight = 600;
const float paddleSpeed = 400.f;
const float paddleOffsetWall = 10.f;

CircleShape ball;
RectangleShape paddles[2];
Font font; // Declare font
Text text; // Declare text for score

// Score variables
int player1Score = 0;
int player2Score = 0;

// Function declarations
void Load();
void Reset();
void Update(RenderWindow& window);
void Render(RenderWindow& window);
void UpdateScoreText();

void Load() {
   
   
    // Set initial paddle colors
    paddles[0].setFillColor(Color::White);
    paddles[1].setFillColor(Color::White);
    // Initialize the ball
    ball.setRadius(ballRadius);
    ball.setFillColor(Color::White); // Set ball color
    ball.setOrigin(ballRadius, ballRadius); // Set origin to center
    ball.setPosition(Vector2f(gameWidth / 2.f, gameHeight / 2.f)); // Center the ball
    
    

    // Initialize paddle sizes and positions
    paddles[0].setSize(paddleSize);
    paddles[1].setSize(paddleSize);

    //// Load font-face from res dir
    if (!font.loadFromFile("res/fonts/OpenSans-Light.ttf")) {
        cerr << "Error loading font!" << endl; // Error message if font fails to load
        return;
    }
    //// Set text element to use font
   text.setFont(font);
    //// Set the character size to 24 pixels
    text.setCharacterSize(24);

    // Set initial scores to zero
    player1Score = 0;
    player2Score = 0;

    // Update Score Text
    UpdateScoreText();

    // Initial positions
    //Reset(true);
}

void UpdateScoreText() {
    text.setString("Score: " + to_string(player1Score) + " - " + to_string(player2Score));
    text.setPosition((gameWidth * 0.5f) - (text.getLocalBounds().width * 0.5f), 0);
}


void Reset() {
    // Reset paddle positions
    paddles[0].setPosition(Vector2f(paddleOffsetWall, gameHeight / 2.f - paddleSize.y / 2.f));
    paddles[1].setPosition(Vector2f(gameWidth - paddleOffsetWall - paddleSize.x / 2.f, gameHeight / 2.f));

    // Reset Ball Position and Velocity
    ball.setPosition(Vector2f(gameWidth / 2.f, gameHeight / 2.f));
    // Set initial ball velocity based on who served
   

   
}

void Update(RenderWindow& window) {
    // Reset clock, recalculate deltatime
    static Clock clock;
    float dt = clock.restart().asSeconds();

    // Check and consume events
    Event event;
    while (window.pollEvent(event)) {
        if (event.type == Event::Closed) {
            window.close();
            return;
        }
    }

    //Move the ball
    ball.move(ballVelocity * dt);

    // Quit via ESC Key
    if (Keyboard::isKeyPressed(Keyboard::Escape)) {
        window.close();
    }

    // Handle paddle movement
    float direction1 = 0.0f;
    float direction2 = 0.0f;

    // Player 1 controls
    if (Keyboard::isKeyPressed(controls[0])) {
        direction1 -= 1.0f; // Player 1 UP
    }
    if (Keyboard::isKeyPressed(controls[1])) {
        direction1 += 1.0f; // Player 1 Down
    }

    // Player 2 controls
    if (Keyboard::isKeyPressed(controls[2])) {
        direction2 -= 1.0f; // Player 2 UP
    }
    if (Keyboard::isKeyPressed(controls[3])) {
        direction2 += 1.0f; // Player 2 Down
    }

    // Move paddles
    paddles[0].move(Vector2f(0.f, direction1 * paddleSpeed * dt));
    paddles[1].move(Vector2f(0.f, direction2 * paddleSpeed * dt));

    // Keep paddles within bounds
    for (int i = 0; i < 2; ++i) {
        Vector2f pos = paddles[i].getPosition();
        if (pos.y < paddleSize.y / 2.f) {
            paddles[i].setPosition(pos.x, paddleSize.y / 2.f);
        }
        else if (pos.y > gameHeight - paddleSize.y / 2.f ) {
            paddles[i].setPosition(pos.x, gameHeight - paddleSize.y / 2.f);
        }
    }

    // Check ball collision with walls
    const float bx = ball.getPosition().x;
    const float by = ball.getPosition().y;

    if (by >= gameHeight - ballRadius) { // Bottom wall
        ballVelocity.y *= -1; // Reverse the Y direction
        ball.setPosition(bx, gameHeight - ballRadius); // Prevent sticking
    }
    else if (by <= ballRadius) { // Top wall
        ballVelocity.y *= -1; // Reverse the Y direction
        ball.setPosition(bx, ballRadius); // Prevent sticking
    }

    // Check ball collision with score walls
    if (bx > gameWidth) { // Right wall (Player 1 scores)
        player2Score++; // Increment Player 2's score
        UpdateScoreText(); // Update the score display
        Reset(); // Pass true to indicate Player 1 lost the point
        ballVelocity = { initialVelocityX, initialVelocityY }; // Player 1 serves next
    }
    else if (bx < 0) { // Left wall (Player 2 scores)
        player1Score++; // Increment Player 1's score
        UpdateScoreText(); // Update the score display
        Reset(); // Pass false to indicate Player 2 lost the point
        ballVelocity = { -initialVelocityX, initialVelocityY }; // Player 2 serves next
    }

    // Check ball collision with paddles
    if (bx < paddleSize.x + paddleOffsetWall &&
        by > paddles[0].getPosition().y &&
        by < paddles[0].getPosition().y + paddleSize.y) {
        // Bounce off left paddle
        ballVelocity.x *= -1; // Reverse X direction
        ball.setPosition(paddleSize.x + paddleOffsetWall + ballRadius, by); // Position the ball outside the paddle
    }
    else if (bx > gameWidth - paddleSize.x - paddleOffsetWall &&
        by > paddles[1].getPosition().y &&
        by < paddles[1].getPosition().y + paddleSize.y) {
        // Bounce off right paddle
        ballVelocity.x *= -1; // Reverse X direction
        ball.setPosition(gameWidth - paddleSize.x - paddleOffsetWall - ballRadius, by); // Position the ball outside the paddle
    }
}

void Render(RenderWindow& window) {
    // Draw Everything
    window.draw(paddles[0]);
    window.draw(paddles[1]);
    window.draw(ball);
    window.draw(text); // Draw the score text
}

int main() {
    RenderWindow window(VideoMode(gameWidth, gameHeight), "PONG");
    Load();
    Reset(); // Start the game without any player awarded a point
    ballVelocity = { initialVelocityX, initialVelocityY }; // Player 1 serves first

    while (window.isOpen()) {
        window.clear();
        Update(window);
        Render(window);
        window.display();
    }
    return 0;
}
