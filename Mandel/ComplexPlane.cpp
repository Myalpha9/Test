#include "ComplexPlane.h"
#include <cmath>

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
    target.draw(m_vArray);
}

void ComplexPlane::updateRender() {
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

void ComplexPlane::zoomIn() {
    ++m_zoomCount;
    float xSize = BASE_WIDTH * std::pow(BASE_ZOOM, m_zoomCount);
    float ySize = BASE_HEIGHT * m_aspectRatio * std::pow(BASE_ZOOM, m_zoomCount);
    m_plane_size = { xSize, ySize };
    m_State = State::CALCULATING;
}

void ComplexPlane::zoomOut() {
    --m_zoomCount;
    float xSize = BASE_WIDTH * std::pow(BASE_ZOOM, m_zoomCount);
    float ySize = BASE_HEIGHT * m_aspectRatio * std::pow(BASE_ZOOM, m_zoomCount);
    m_plane_size = { xSize, ySize };
    m_State = State::CALCULATING;
}

void ComplexPlane::setCenter(sf::Vector2i pixel) {
    m_plane_center = mapPixelToCoords(pixel);
    m_State = State::CALCULATING;
}

void ComplexPlane::setMouseLocation(sf::Vector2i pixel) {
    m_mouseLocation = mapPixelToCoords(pixel);
}

void ComplexPlane::loadText(sf::Text& text) {
    std::ostringstream ss;
    ss << "Center: (" << m_plane_center.x << ", " << m_plane_center.y << ")"
       << " | Zoom: " << m_zoomCount
       << " | Mouse: (" << m_mouseLocation.x << ", " << m_mouseLocation.y << ")";
    text.setString(ss.str());
}

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

void ComplexPlane::iterationsToRGB(size_t count, sf::Uint8& r, sf::Uint8& g, sf::Uint8& b) const {
    if (count == MAX_ITER) {
        r = g = b = 0;
    } else {
        int val = static_cast<int>(255.0 * count / MAX_ITER);
        r = val; g = val; b = val; // grayscale
    }
}

sf::Vector2f ComplexPlane::mapPixelToCoords(sf::Vector2i pixel) const {
    float x = static_cast<float>(pixel.x);
    float y = static_cast<float>(pixel.y);

    float real = (x / m_pixel_size.x) * m_plane_size.x + (m_plane_center.x - m_plane_size.x / 2.0f);
    float imag = ((m_pixel_size.y - y) / m_pixel_size.y) * m_plane_size.y + (m_plane_center.y - m_plane_size.y / 2.0f);

    return { real, imag };
}


