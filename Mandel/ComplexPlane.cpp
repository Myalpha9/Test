#include "ComplexPlane.h"
#include <cmath>
#include <iostream>		//Library Implemented 
#include <sstream>
#include <complex>


//Constructor sets the intial view
ComplexPlane::ComplexPlane(int pixelWidth, int pixelHeight) {
    m_pixel_size = { pixelWidth, pixelHeight }; 
    m_aspectRatio = static_cast<float>(pixelHeight) / static_cast<float>(pixelWidth);
    m_plane_center = { 0.0f, 0.0f };
    m_plane_size = { BASE_WIDTH, BASE_HEIGHT * m_aspectRatio };
    m_zoomCount = 0;
    m_State = State::CALCULATING;

    m_vArray.setPrimitiveType(sf::Points);
    m_vArray.resize(pixelWidth * pixelHeight);
}

void ComplexPlane::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(m_vArray); //draws the complex plane
}

void ComplexPlane::updateRender() {	//updates the image
    if (m_State != State::CALCULATING) return;

    for (unsigned int i = 0; i < m_pixel_size.y; ++i) {
        for (unsigned int j = 0; j < m_pixel_size.x; ++j) {
            sf::Vector2f coord = mapPixelToCoords({ static_cast<int>(j), static_cast<int>(i) });
            size_t count = countIterations(coord);

            sf::Uint8 r = 0, g = 0, b = 0;
            iterationsToRGB(count, r, g, b);

            size_t index = j + i * m_pixel_size.x;
            m_vArray[index].position = { static_cast<float>(j), static_cast<float>(i) };
            m_vArray[index].color = sf::Color(r, g, b);
        }
    }
    m_State = State::DISPLAYING;
}


//zoom in function
void ComplexPlane::zoomIn() {
    ++m_zoomCount;
    float xSize = BASE_WIDTH * std::pow(BASE_ZOOM, m_zoomCount);
    float ySize = BASE_HEIGHT * m_aspectRatio * std::pow(BASE_ZOOM, m_zoomCount);
    m_plane_size = { xSize, ySize };
    m_State = State::CALCULATING;
}



//zoom out function
void ComplexPlane::zoomOut() {
    --m_zoomCount;
    float xSize = BASE_WIDTH * std::pow(BASE_ZOOM, m_zoomCount);
    float ySize = BASE_HEIGHT * m_aspectRatio * std::pow(BASE_ZOOM, m_zoomCount);
    m_plane_size = { xSize, ySize };
    m_State = State::CALCULATING;
}


//sets center
void ComplexPlane::setCenter(sf::Vector2i pixel) {
    m_plane_center = mapPixelToCoords(pixel);
    m_State = State::CALCULATING;
}
//mouse location
void ComplexPlane::setMouseLocation(sf::Vector2i pixel) {
    m_mouseLocation = mapPixelToCoords(pixel);
}

//updates text
void ComplexPlane::loadText(sf::Text& text) 

{
    std::ostringstream ss;
    ss << "Center: (" << m_plane_center.x << ", " << m_plane_center.y << std::endl;
    ss << "Zoom Count: " << m_zoomCount << std::endl;
    ss << "Cursor: (" << m_mouseLocation.x << ", " << m_mouseLocation.y << ")" << std::endl;
    ss << "Left Click to Zoom in" << std::endl;
    ss << "Right Click to Zoom in" << std::endl;


    text.setString(ss.str());
}
//runs calculatioin to determine how many iterations it took to escape
size_t ComplexPlane::countIterations(sf::Vector2f coord) const {
    std::complex<double> c(coord.x, coord.y);
    std::complex<double> z = c;
    size_t iter = 0;

    while (std::abs(z) < 2.0 && iter < MAX_ITER) {
        z = z * z + c;
        ++iter;
    }
    return iter;
}

//based on iterations determine sthe rgb color
void ComplexPlane::iterationsToRGB(size_t count, sf::Uint8& r, sf::Uint8& g, sf::Uint8& b) const
{
	const size_t range = MAX_ITER / 5;
		if (count == MAX_ITER)
		{
			r = 0;
			g = 0;
			b = 0;
		}
		else if (count < range)
		{
			r = 75;  
			g = 0;   
			b = 130; 
		}
		else if (count < 2 * range)
		{
			r = 0;
			g = 255;
			b = 255;
		}
		else if (count < 3 * range)
		{
			r = 0;
			g = 255;
			b = 0;
		}
		else if (count < 4 * range)
		{
			r = 255;
			g = 255;
			b = 0;
		}
		else
		{
			r = 255;
			g = 0;
			b = 0;
		}
	
}		



 //turns the x y coordinates into numbers to calculate into the Madelbrol set

sf::Vector2f ComplexPlane::mapPixelToCoords(sf::Vector2i pixel) const {
    float x = static_cast<float>(pixel.x);
    float y = static_cast<float>(pixel.y);

    float real = (x / m_pixel_size.x) * m_plane_size.x + (m_plane_center.x - m_plane_size.x / 2.0f);
    float imag = ((m_pixel_size.y - y) / m_pixel_size.y) * m_plane_size.y + (m_plane_center.y - m_plane_size.y / 2.0f);

    return { real, imag };
}


