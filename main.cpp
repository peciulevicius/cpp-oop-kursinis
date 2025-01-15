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

// #include <SFML/Graphics.hpp>
// #include <vector>
// #include <iostream>
// #include <cstdlib> // Dėl rand() ir srand()
// #include <ctime>   // Dėl time()
//
// // Konstantos lango dydžiui ir tinklo dydžiui
// const int WINDOW_WIDTH = 600;
// const int WINDOW_HEIGHT = 600;
// const int CELL_SIZE = 20;
// const int NUM_CELLS = WINDOW_WIDTH / CELL_SIZE; // Tinklo langelių skaičius
//
// // Kryptys, kuriomis gali judėti gyvatė
// enum Direction { UP, DOWN, LEFT, RIGHT };
//
// // Klase, atstovaujanti gyvatei
// class Snake {
// private:
//     std::vector<sf::RectangleShape> body; // Gyvatės segmentai
//     Direction direction; // Dabartinė gyvatės kryptis
//
// public:
//     // Konstruktorius: inicializuoja gyvatę su vienu segmentu viduryje
//     Snake() {
//         sf::RectangleShape segment(sf::Vector2f(CELL_SIZE, CELL_SIZE));
//         segment.setPosition(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2); // Nustato segmentą į pradžios padėtį
//         segment.setFillColor(sf::Color::Green); // Fragmento spalva
//         body.push_back(segment); // Prideda segmentą į gyvatės kūną
//         direction = RIGHT; // Pradinių kryptis
//     }
//
//     // Keičia gyvatės kryptį, neleisdama grįžti į priešingą pusę
//     void changeDirection(Direction newDirection) {
//         // Patikrina, ar nauja kryptis nėra priešinga dabartinei
//         if ((direction == UP && newDirection != DOWN) ||
//             (direction == DOWN && newDirection != UP) ||
//             (direction == LEFT && newDirection != RIGHT) ||
//             (direction == RIGHT && newDirection != LEFT)) {
//             direction = newDirection; // Keičia kryptį
//         }
//     }
//
//     // Judina gyvatę dabartine kryptimi
//     void move() {
//         sf::Vector2f newHeadPos = body[0].getPosition(); // Dabartinė galvos padėtis
//         switch (direction) {
//             case UP: newHeadPos.y -= CELL_SIZE; break; // Judėjimas aukštyn
//             case DOWN: newHeadPos.y += CELL_SIZE; break; // Judėjimas žemyn
//             case LEFT: newHeadPos.x -= CELL_SIZE; break; // Judėjimas į kairę
//             case RIGHT: newHeadPos.x += CELL_SIZE; break; // Judėjimas į dešinę
//         }
//
//         // Įterpia naują galvos segmentą į priekį
//         sf::RectangleShape newHead(sf::Vector2f(CELL_SIZE, CELL_SIZE));
//         newHead.setPosition(newHeadPos); // Nustato naujo segmentą padėtį
//         newHead.setFillColor(sf::Color::Green); // Spalva
//         body.insert(body.begin(), newHead); // Prideda naują segmentą prie gyvatės kūno
//     }
//
//     // Sumažina gyvatės ilgį
//     void shrink() {
//         if (body.size() > 1) { // Užtikrina, kad yra daugiau nei 1 segmentas
//             body.pop_back(); // Pašalina paskutinį segmentą
//         }
//     }
//
//     // Prideda naują segmentą prie gyvatės, kai ji suvalgo maistą
//     void grow() {
//         sf::RectangleShape newSegment(sf::Vector2f(CELL_SIZE, CELL_SIZE));
//         newSegment.setPosition(body.back().getPosition()); // Nustato naujo segmentą padėtį
//         newSegment.setFillColor(sf::Color::Green); // Spalva
//         body.push_back(newSegment); // Prideda naują segmentą į gyvatės kūną
//     }
//
//     // Brėžia gyvatę lango lange
//     void draw(sf::RenderWindow& window) {
//         for (auto& segment : body) {
//             window.draw(segment); // Brėžia kiekvieną segmentą
//         }
//     }
//
//     // Tikrina, ar yra susidūrimų su siena ar savimi
//     bool checkCollision() {
//         sf::Vector2f headPos = body[0].getPosition(); // Galvos padėtis
//         // Patikrina, ar galva nepatenka į lango ribas
//         if (headPos.x < 0 || headPos.y < 0 || headPos.x >= WINDOW_WIDTH || headPos.y >= WINDOW_HEIGHT) {
//             return true; // Susidūrimas su lango riba
//         }
//         // Patikrina, ar galva susiduria su kūno segmentais
//         for (size_t i = 1; i < body.size(); ++i) {
//             if (body[i].getPosition() == headPos) {
//                 return true; // Susidūrimas su savimi
//             }
//         }
//         return false;
//     }
//
//     // Grąžina gyvatės galvos padėtį
//     sf::Vector2f getHeadPosition() {
//         return body[0].getPosition(); // Grąžina galvos padėtį
//     }
//
//     // Grąžina gyvatės kūno segmentus
//     const std::vector<sf::RectangleShape>& getBody() {
//         return body; // Grąžina gyvatės kūno segmentos
//     }
// };
//
// // Klase, atstovaujanti maistui
// class Food {
// private:
//     sf::RectangleShape food; // Maisto atvaizdavimas kaip stačiakampis
//
// public:
//     // Konstruktorius nustato maisto savybes
//     Food() {
//         food.setSize(sf::Vector2f(CELL_SIZE, CELL_SIZE)); // Nustato maisto dydį
//         food.setFillColor(sf::Color::Red); // Raudona spalva
//         regenerate(std::vector<sf::RectangleShape>()); // Inicializuoja tuščią gyvatės kūną
//     }
//
//     // Regeneruoja maistą tinkamuose taškuose, kad nesusidurtų su gyvate
//     void regenerate(const std::vector<sf::RectangleShape>& snakeBody) {
//         int x, y;
//         do {
//             x = (rand() % NUM_CELLS) * CELL_SIZE; // Generuoja atsitiktinę x padėtį
//             y = (rand() % NUM_CELLS) * CELL_SIZE; // Generuoja atsitiktinę y padėtį
//         } while (isFoodOnSnakeBody(x, y, snakeBody)); // Patikrina, ar maistas nesusiduria su gyvate
//
//         food.setPosition(x, y); // Nustato maisto naują padėtį
//     }
//
//     // Tikrina, ar maistas nesusiduria su gyvatės kūno segmentais
//     bool isFoodOnSnakeBody(int x, int y, const std::vector<sf::RectangleShape>& snakeBody) {
//         for (const auto& segment : snakeBody) {
//             if (segment.getPosition() == sf::Vector2f(x, y)) {
//                 return true; // Maistas randamas ant gyvatės
//             }
//         }
//         return false; // Maistas negalima ant gyvatės
//     }
//
//     // Brėžia maistą lango lange
//     void draw(sf::RenderWindow& window) {
//         window.draw(food); // Brėžia maistą
//     }
//
//     // Grąžina maisto padėtį
//     sf::Vector2f getPosition() {
//         return food.getPosition(); // Grąžina maisto padėtį
//     }
// };
//
// // Žaidimo pabaigos ekranas
// void gameOverScreen(sf::RenderWindow& window) {
//     sf::Font font;
//
//     // Įrašo šriftą, kad parodytų pranešimą apie žaidimo pabaigą
//     if (!font.loadFromFile("arial.ttf")) {
//         std::cerr << "Nepavyko įkelti šrifto!" << std::endl;
//         return;
//     }
//
//     sf::Text text;
//     text.setFont(font);
//     text.setString("Žaidimo pabaiga! Paspauskite R, kad pradėtumėte iš naujo arba Q, kad išeitumėte");
//     text.setCharacterSize(24); // Šrifto dydis
//     text.setFillColor(sf::Color::White); // Spalva
//     text.setPosition(50, WINDOW_HEIGHT / 2); // Pozicija ekrane
//
//     // Rodo žaidimo pabaigos ekraną
//     while (window.isOpen()) {
//         sf::Event event;
//         while (window.pollEvent(event)) {
//             if (event.type == sf::Event::Closed) {
//                 window.close(); // Uždaro langą
//             }
//             if (event.type == sf::Event::KeyPressed) {
//                 if (event.key.code == sf::Keyboard::R) {
//                     return; // Pradėti iš naujo
//                 }
//                 if (event.key.code == sf::Keyboard::Q) {
//                     window.close(); // Uždaryti žaidimą
//                 }
//             }
//         }
//         window.clear(); // Išvalyti langą
//         window.draw(text); // Brėžti pranešimą
//         window.display(); // Atvaizduoti langą
//     }
// }
//
// int main() {
//     srand(static_cast<unsigned int>(time(0))); // Inicializuoja atsitiktinių skaičių generatorių
//     sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Gyvatės Žaidimas"); // Sukuria langą
//     Snake snake; // Sukuria gyvatės objektą
//     Food food;   // Sukuria maisto objektą
//
//     sf::Clock clock; // Valdo žaidimo laiką
//     float delay = 0.2f; // Judėjimo atidėjimas (sekundėmis)
//     int score = 0; // Žaidėjo taškai
//
//     // Pagrindinis žaidimo ciklas
//     while (window.isOpen()) {
//         sf::Event event;
//         while (window.pollEvent(event)) {
//             if (event.type == sf::Event::Closed) {
//                 window.close(); // Uždaro langą
//             }
//         }
//
//         // Apdoroja klaviatūros įvesties kryptis
//         if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
//             snake.changeDirection(UP);
//         } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
//             snake.changeDirection(DOWN);
//         } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
//             snake.changeDirection(LEFT);
//         } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
//             snake.changeDirection(RIGHT);
//         }
//
//         // Atnaujina žaidimo būseną reguliariais intervalais
//         if (clock.getElapsedTime().asSeconds() > delay) {
//             snake.move(); // Judina gyvatę
//             if (snake.getHeadPosition() == food.getPosition()) {
//                 snake.grow(); // Augina gyvatę, kai ji suvalgo maistą
//                 food.regenerate(snake.getBody()); // Regeneruoja maistą naujoje padėtyje
//                 score += 10; // Padidina taškų skaičių
//             } else {
//                 snake.shrink(); // Sumažina gyvatę, jei nelaukia
//             }
//             if (snake.checkCollision()) {
//                 std::cout << "Žaidimo pabaiga! Jūsų taškai: " << score << std::endl; // Spausdina taškus
//                 gameOverScreen(window); // Rodo žaidimo pabaigos ekraną
//                 score = 0; // Atkuria taškus
//                 snake = Snake(); // Atkuria gyvatę
//                 food.regenerate(snake.getBody()); // Atkuria maisto padėtį
//             }
//             clock.restart(); // Iš naujo paleidžia laiką
//         }
//
//         // Išvalyti, piešti ir atvaizduoti žaidimo langą
//         window.clear(); // Išvalyti langą
//         snake.draw(window); // Brėžia gyvatę
//         food.draw(window); // Brėžia maistą
//         window.display(); // Atvaizduojamas atnaujintas langas
//     }
//
//     return 0; // Programos pabaiga
// }