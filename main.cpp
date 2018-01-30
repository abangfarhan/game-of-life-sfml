#include <SFML/Graphics.hpp>
#include <iostream>
#include <time.h>

#define WHITE sf::Color::White
#define BLACK sf::Color::Black
#define GRAY sf::Color(153, 153, 153)
#define PAUSE_KEY sf::Keyboard::P

int wrapValue(int v, int vMax)
{
    // To generalize this, use modulo
    if (v == -1) return vMax - 1;
    if (v == vMax) return 0;
    return v;
}

int main()
{
    const int DELAY = 100;
    const int CELL_SIZE = 30;
    const sf::Vector2f CELL_VECTOR = sf::Vector2f(CELL_SIZE, CELL_SIZE);
    const int GRID_WIDTH = 30;
    const int GRID_HEIGHT = 20;
    const int N_CELLS = GRID_WIDTH * GRID_HEIGHT;
    int grid[N_CELLS] = {  };
    int gridNext[N_CELLS];
    srand(time(NULL));
    // for (int i = 0; i < N_CELLS; i++)
    //     grid[i] = (double(rand())/RAND_MAX < 0.1) ? 1 : 0;
    grid[0 + 0 * GRID_WIDTH] = 1;
    grid[1 + 1 * GRID_WIDTH] = 1;
    grid[2 + 1 * GRID_WIDTH] = 1;
    grid[0 + 2 * GRID_WIDTH] = 1;
    grid[1 + 2 * GRID_WIDTH] = 1;

    bool isPlaying = true;

    sf::RenderWindow window(sf::VideoMode(CELL_SIZE*GRID_WIDTH, CELL_SIZE*GRID_HEIGHT), "SFML Window");
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::KeyPressed:
                if (event.key.code == PAUSE_KEY)
                    isPlaying = !isPlaying;
                break;
            case sf::Event::MouseButtonPressed:
                if (!isPlaying && event.mouseButton.button == sf::Mouse::Left)
                {
                    int x = double(event.mouseButton.x)/CELL_SIZE;
                    int y = double(event.mouseButton.y)/CELL_SIZE;
                    grid[x + y * GRID_WIDTH] = !grid[x + y * GRID_WIDTH];
                }
                break;
            }
        }

        // display grid and prepare gridNext
        window.clear(WHITE);
        for (int x = 0; x < GRID_WIDTH; x++)
        {
            for (int y = 0; y < GRID_HEIGHT; y++)
            {
                // draw cell
                sf::RectangleShape cell;
                cell.setPosition(x * CELL_SIZE, y * CELL_SIZE);
                cell.setSize(CELL_VECTOR);
                cell.setOutlineThickness(1);
                cell.setOutlineColor(GRAY);
                if (grid[x + y * GRID_WIDTH] == 1)
                    cell.setFillColor(BLACK);
                else
                    cell.setFillColor(WHITE);
                window.draw(cell);

                // prepare gridNext
                if (isPlaying)
                {
                    int neighborSum = 0;
                    for (int i = -1; i < 2; i++)
                        for (int j = -1; j < 2; j++)
                        {
                            int xi = wrapValue(x + i, GRID_WIDTH);
                            int yj = wrapValue(y + j, GRID_HEIGHT);
                            neighborSum += grid[xi + yj * GRID_WIDTH];
                        }

                    int current = x + y * GRID_WIDTH;
                    neighborSum -= grid[current];
                    gridNext[current] = grid[current];
                    if (grid[current] == 1 && (neighborSum < 2 || neighborSum > 3))
                        gridNext[current] = 0;
                    else if (neighborSum == 3)
                        gridNext[current] = 1;
                }
            }
        }

        // move gridNext to grid
        if (isPlaying)
            for (int i = 0; i < N_CELLS; i++)
                grid[i] = gridNext[i];

        window.display();
        sf::sleep(sf::milliseconds(DELAY));
    }
}
