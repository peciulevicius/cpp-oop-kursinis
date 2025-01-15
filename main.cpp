#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <cstdlib> // For rand() and srand()
#include <ctime>   // For time()

// Constants for window size and grid size
const int WINDOW_WIDTH = 600;
const int WINDOW_HEIGHT = 600;
const int CELL_SIZE = 20;
const int NUM_CELLS = WINDOW_WIDTH / CELL_SIZE;

// Direction enums for snake movement
enum Direction { UP, DOWN, LEFT, RIGHT };

// Class representing the Snake
class Snake {
private:
    std::vector<sf::RectangleShape> body;
    Direction direction;
public:
    Snake() {
        // Initialize the snake with one segment at the center of the grid
        sf::RectangleShape segment(sf::Vector2f(CELL_SIZE, CELL_SIZE));
        segment.setPosition(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
        segment.setFillColor(sf::Color::Green);
        body.push_back(segment);
        direction = RIGHT;
    }

    void changeDirection(Direction newDirection) {
        // Prevent reversing direction
        if ((direction == UP && newDirection != DOWN) ||
            (direction == DOWN && newDirection != UP) ||
            (direction == LEFT && newDirection != RIGHT) ||
            (direction == RIGHT && newDirection != LEFT)) {
            direction = newDirection;
        }
    }

    void move() {
        // Create a new head segment in the direction of movement
        sf::Vector2f newHeadPos = body[0].getPosition();
        switch (direction) {
            case UP: newHeadPos.y -= CELL_SIZE; break;
            case DOWN: newHeadPos.y += CELL_SIZE; break;
            case LEFT: newHeadPos.x -= CELL_SIZE; break;
            case RIGHT: newHeadPos.x += CELL_SIZE; break;
        }

        // Insert new head at the front
        sf::RectangleShape newHead(sf::Vector2f(CELL_SIZE, CELL_SIZE));
        newHead.setPosition(newHeadPos);
        newHead.setFillColor(sf::Color::Green);
        body.insert(body.begin(), newHead);
    }

    void shrink() {
        // Remove the last segment if the snake has not eaten
        body.pop_back();
    }

    void grow() {
        // Add a new segment at the tail's position
        sf::RectangleShape newSegment(sf::Vector2f(CELL_SIZE, CELL_SIZE));
        newSegment.setPosition(body.back().getPosition());
        newSegment.setFillColor(sf::Color::Green);
        body.push_back(newSegment);
    }

    void draw(sf::RenderWindow& window) {
        for (auto& segment : body) {
            window.draw(segment);
        }
    }

    bool checkCollision() {
        // Check if the snake collides with itself or the window boundaries
        sf::Vector2f headPos = body[0].getPosition();
        if (headPos.x < 0 || headPos.y < 0 || headPos.x >= WINDOW_WIDTH || headPos.y >= WINDOW_HEIGHT) {
            return true;
        }
        for (size_t i = 1; i < body.size(); ++i) {
            if (body[i].getPosition() == headPos) {
                return true;  // Collision with body
            }
        }
        return false;
    }

    sf::Vector2f getHeadPosition() {
        return body[0].getPosition();
    }

    const std::vector<sf::RectangleShape>& getBody() {
        return body;
    }
};

// Class representing the Food
class Food {
private:
    sf::RectangleShape food;
public:
    Food() {
        food.setSize(sf::Vector2f(CELL_SIZE, CELL_SIZE));
        food.setFillColor(sf::Color::Red);
        regenerate(std::vector<sf::RectangleShape>()); // Empty vector initially
    }

    void regenerate(const std::vector<sf::RectangleShape>& snakeBody) {
        int x, y;
        do {
            x = (rand() % NUM_CELLS) * CELL_SIZE; // Generate random x position
            y = (rand() % NUM_CELLS) * CELL_SIZE; // Generate random y position
        } while (isFoodOnSnakeBody(x, y, snakeBody)); // Ensure food does not spawn on snake

        food.setPosition(x, y); // Set food's new position
    }

    bool isFoodOnSnakeBody(int x, int y, const std::vector<sf::RectangleShape>& snakeBody) {
        for (const auto& segment : snakeBody) {
            if (segment.getPosition() == sf::Vector2f(x, y)) {
                return true; // Food spawns on the snake
            }
        }
        return false;
    }

    void draw(sf::RenderWindow& window) {
        window.draw(food);
    }

    sf::Vector2f getPosition() {
        return food.getPosition();
    }
};

// Game Over Screen
void gameOverScreen(sf::RenderWindow& window) {
    sf::Font font;

    // Ensure you have a font file available in your project directory, or adjust the path here
    if (!font.loadFromFile("arial.ttf")) {
        std::cerr << "Could not load font!" << std::endl;
        return;
    }

    sf::Text text;
    text.setFont(font);
    text.setString("Game Over! Press R to Restart or Q to Quit");
    text.setCharacterSize(24);
    text.setFillColor(sf::Color::White);
    text.setPosition(50, WINDOW_HEIGHT / 2);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::R) {
                    return; // Restarting by returning to the main game loop
                }
                if (event.key.code == sf::Keyboard::Q) {
                    window.close();
                }
            }
        }
        window.clear();
        window.draw(text);
        window.display();
    }
}

int main() {
    srand(static_cast<unsigned int>(time(0))); // Seed for random number generation
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Snake Game");
    Snake snake;
    Food food;

    sf::Clock clock;
    float delay = 0.2f; // Movement delay in seconds
    int score = 0;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        // Handle keyboard input
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
            snake.changeDirection(UP);
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
            snake.changeDirection(DOWN);
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            snake.changeDirection(LEFT);
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            snake.changeDirection(RIGHT);
        }

        // Update the game state at regular intervals
        if (clock.getElapsedTime().asSeconds() > delay) {
            snake.move();
            if (snake.getHeadPosition() == food.getPosition()) {
                snake.grow();
                food.regenerate(snake.getBody());
                score += 10; // Increment score by 10
            } else {
                snake.shrink(); // Only shrink if not eating
            }
            if (snake.checkCollision()) {
                std::cout << "Game Over! Your score: " << score << std::endl;
                gameOverScreen(window);
                score = 0; // Reset score on restart
                snake = Snake(); // Reset snake
                food.regenerate(snake.getBody()); // Reset food
            }
            clock.restart();
        }

        // Clear the window and draw the game
        window.clear();
        snake.draw(window);
        food.draw(window);
        window.display();
    }

    return 0;
}