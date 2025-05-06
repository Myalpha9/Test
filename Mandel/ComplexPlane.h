#pragma once

#include <SFML/Graphics.hpp>
#include <complex>    //Libraries implement into the program
#include <sstream>

const unsigned int MAX_ITER = 64;   //Constants 
const float BASE_WIDTH = 4.0f;        
const float BASE_HEIGHT = 4.0f;
const float BASE_ZOOM = 0.5f;

enum class State { CALCULATING, DISPLAYING }; //defines the state its in for rendering 

class ComplexPlane : public sf::Drawable {        //complex Plane class inherits Drawable (SFML) 
public:
    ComplexPlane(int pixelWidth, int pixelHeight);        //Constructer: ComplexPlane the rest is the 
    void updateRender();                                    // functions within the ComplexPlane
    void zoomIn();
    void zoomOut();
    void setCenter(sf::Vector2i mousePixel);
    void setMouseLocation(sf::Vector2i mousePixel);
    void loadText(sf::Text& text);

private:         //Private variables
    sf::VertexArray m_vArray;
    sf::Vector2u m_pixel_size;
    float m_aspectRatio;
    sf::Vector2f m_plane_center;
    sf::Vector2f m_plane_size;
    int m_zoomCount;
    State m_State;
    sf::Vector2f m_mouseLocation;

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    size_t countIterations(sf::Vector2f coord) const;
//counts the interaction it takes for a point to escape

    void iterationsToRGB(size_t count, sf::Uint8& r, sf::Uint8& g, sf::Uint8& b) const;    
//converts iteration count to RGB values to determine correct pixel color


    sf::Vector2f mapPixelToCoords(sf::Vector2i pixel) const;
 // converts pixel coordinates to complex coordinates

};




