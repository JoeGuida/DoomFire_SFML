#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <algorithm>
#include <iostream>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

// Fire Colors
sf::Color colors[37] = {
    sf::Color(0, 0, 0),
    sf::Color(7, 7, 7),
    sf::Color(31, 7, 7),
    sf::Color(47, 15, 7),
    sf::Color(71, 15, 7),
    sf::Color(87, 23, 7),
    sf::Color(103, 31, 7),
    sf::Color(119, 31, 7),
    sf::Color(143, 39, 7),
    sf::Color(159, 47, 7),
    sf::Color(175, 63, 7),
    sf::Color(191, 171, 7),
    sf::Color(199, 71, 7),
    sf::Color(223, 79, 7),
    sf::Color(223, 87, 7),
    sf::Color(223, 87, 7),
    sf::Color(215, 95, 7),
    sf::Color(215, 103, 15),
    sf::Color(207, 111, 15),
    sf::Color(207, 119, 15),
    sf::Color(207, 127, 15),
    sf::Color(207, 135, 23),
    sf::Color(199, 135, 23),
    sf::Color(199, 143, 23),
    sf::Color(199, 151, 31),
    sf::Color(191, 159, 31),
    sf::Color(191, 159, 31),
    sf::Color(191, 167, 39),
    sf::Color(191, 167, 39),
    sf::Color(191, 175, 47),
    sf::Color(183, 175, 47),
    sf::Color(183, 183, 47),
    sf::Color(183, 183, 55),
    sf::Color(207, 207, 111),
    sf::Color(223, 223, 159),
    sf::Color(239, 239, 199),
    sf::Color(255, 255, 255),
};

// Window dimensions and max framerate
const int WIDTH = 300;
const int HEIGHT = 300;
const int FRAMERATE_LIMIT = 60;

// Returns the index of a color
int find(sf::Color arr[], int len, sf::Color seek)
{
    for (int i = 0; i < len; ++i)
    {
        if (arr[i].r == seek.r && 
            arr[i].g == seek.g && 
            arr[i].b == seek.b) return i;
    }
    return -1;
}

void spreadFire(sf::VertexArray &frameBuffer, int src) {
    // Get the index of the color of the pixel at frameBuffer[src]
    int index = find(colors, 37, frameBuffer[src].color);
    
    int randomNumber = rand() % 4;
    int distance = src - randomNumber + 1;

    if(distance - WIDTH >= 0)
        frameBuffer[distance - WIDTH] = sf::Vertex(frameBuffer[distance - WIDTH].position, colors[index - (randomNumber & 1)]);
}

void doFire(sf::VertexArray &frameBuffer) {
    for (int x = 0; x < WIDTH; x++)
        for (int y = 0; y < HEIGHT; y++) {
            spreadFire(frameBuffer, x + (-y + HEIGHT - 1) * HEIGHT);
        }
}

int main()
{
    srand(time(NULL));

    // Create and initialize the framebuffer
    // Set the bottom row of pixels to white (36)
    // Set everything else to black (0)
    sf::VertexArray framebuffer(sf::Points, WIDTH * HEIGHT);
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            if (y == HEIGHT - 1) 
                framebuffer[x + y * HEIGHT] = sf::Vertex(sf::Vector2f(x, y), colors[36]);
            else
                framebuffer[x + y * HEIGHT] = sf::Vertex(sf::Vector2f(x, y), colors[0]);
        }
    }

    // Create the window and set the framerate limit
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Doom Fire", sf::Style::Close);
    window.setFramerateLimit(FRAMERATE_LIMIT);
    while (window.isOpen())
    {
        // Click to close the window
        sf::Event event;
        while (window.pollEvent(event))
            if (event.type == sf::Event::Closed)
                window.close();

        // Propogate the fire
        doFire(framebuffer);

        window.clear();
        window.draw(framebuffer);
        window.display();
    }
    return 0;
}