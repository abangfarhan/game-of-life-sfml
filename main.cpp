#include <SFML/Graphics.hpp>
#include <iostream>
#include <time.h>

#define WHITE sf::Color::White
#define BLACK sf::Color::Black
#define GRAY sf::Color(153, 153, 153)

int wrapValue(int v, int vMax)
{
    // To generalize this, use modulo
    if (v == -1) return vMax - 1;
    if (v == vMax) return 0;
    return v;
}

int main()
{
    const int DELAY = 0;
    const int CELL_SIZE = 30;
    const int GRID_WIDTH = 30;
    const int GRID_HEIGHT = 20;
    const int N_CELLS = GRID_WIDTH * GRID_HEIGHT;
    int grid[N_CELLS];
    int gridNext[N_CELLS];
    srand(time(NULL));
    for (int i = 0; i < N_CELLS; i++)
        grid[i] = (double(rand())/RAND_MAX < 0.1) ? 1 : 0;

    sf::RenderWindow window(sf::VideoMode(CELL_SIZE*GRID_WIDTH, CELL_SIZE*GRID_HEIGHT), "SFML Window");
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
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
                cell.setSize(sf::Vector2f(CELL_SIZE, CELL_SIZE)); // TODO: can Vector2f be made into a variable?
                cell.setOutlineThickness(1);
                cell.setOutlineColor(GRAY);
                if (grid[x + y * GRID_WIDTH] == 1)
                    cell.setFillColor(BLACK);
                else
                    cell.setFillColor(WHITE);
                window.draw(cell);

                // prepare gridNext
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

        // move gridNext to grid
        for (int i = 0; i < N_CELLS; i++)
            grid[i] = gridNext[i];

        window.display();
        sf::sleep(sf::milliseconds(DELAY));
    }
}
