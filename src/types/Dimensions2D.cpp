/**
 * Dimensions.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include "Exceptions.h"
#include "Dimensions2D.h"

namespace spelunker::types {
    Dimensions2D::Dimensions2D(int w, int h)
        : width{w}, height{h} {
        if (w < 0 || h < 0)
            throw IllegalDimensions(w, h);
    }

    bool Dimensions2D::operator==(const Dimensions2D &other) const noexcept {
        return width == other.getWidth() && height == other.getHeight();
    }

    Dimensions2D Dimensions2D::operator+(const Dimensions2D &other) const noexcept {
        return Dimensions2D{width + other.getWidth(), height + other.getHeight()};
    }

    Dimensions2D Dimensions2D::operator*(const int scalar) const noexcept {
        return Dimensions2D{scalar * width, scalar * height};
    }

    Dimensions2D Dimensions2D::operator/(int scalar) const {
        return Dimensions2D{width / scalar, height / scalar};
    }

    bool Dimensions2D::cellInBounds(const int x, const int y) const noexcept {
        return x >= 0 && x < width && y >= 0 && y < height;
    }
    void Dimensions2D::checkCell(int x, int y) const {
        if (!cellInBounds(x, y))
            throw OutOfBoundsCoordinates(x, y);
    }

    void Dimensions2D::checkCell(const Cell &c) const {
        const auto [x,y] = c;
        checkCell(x, y);
    }
}