#include "renderer.h"
#include <algorithm>
#include <SFML/Graphics.hpp>


void render_series(std::vector<double>& series){
    sf::VertexArray lines(sf::LinesStrip, series.size());

    int margin = 20;
    int x_max = 800;
    int y_max = 600;
    int num_pts = static_cast<int>(series.size());

    double min_val = std::ranges::min(series);
    double max_val = std::ranges::max(series);

    for (int i=0; i<num_pts; i++){
        double val = series[i];
        double x = static_cast<double>(i)/num_pts*(x_max - 2*margin) + margin;
        double y = y_max - ((val - min_val )/(max_val - min_val)*(y_max - 2*margin) + margin);

        // for each point i:
        lines[i].position = sf::Vector2f(x, y);
        lines[i].color = sf::Color::White;
    }
    
    sf::VideoMode scr_dims(x_max, y_max);
    sf::RenderWindow window(scr_dims, "SFML window");
    
    // Limit the framerate to 60 frames per second (this step is optional)
    window.setFramerateLimit(60);
    
    // The main loop - ends as soon as the window is closed
    while (window.isOpen()){
        // Event processing
        sf::Event event;
        while (window.pollEvent(event)){
            // Request for closing the window
            if (event.type == sf::Event::Closed)
            window.close();
        }
        // Clear the whole window before rendering a new frame
        window.clear();
        // // Draw some graphical entities
        window.draw(lines);
        // End the current frame and display its contents on screen
        window.display();
    }
};