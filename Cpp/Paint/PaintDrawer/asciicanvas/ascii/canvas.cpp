#include "canvas.hpp"
#include <stdexcept>
#include <iostream>
#include <algorithm>

static const size_t NULLCHAR_SIZE = sizeof('\0');

namespace ascii {

static const CharProxyStrategy &g_legalPositionCharStrategy = LegalBoundsStrategy();
static const CharProxyStrategy &g_outOfBoundsCharStrategy = OutOfBoundsStrategy();

Canvas::CharProxy::CharProxy(const CharProxyStrategy &a_strategy, char *a_buffer, size_t a_idx)
: m_strategy(a_strategy)
, m_buffer(a_buffer)
, m_idx(a_idx) {
}

Canvas::CharProxy &Canvas::CharProxy::operator=(const CharProxy &a_other) {
    m_strategy.setChar(m_buffer, m_idx, (char)a_other);
    return *this;
}

Canvas::CharProxy::operator char() const {
    return m_strategy.getChar(m_buffer, m_idx);
}

Canvas::CharProxy &Canvas::CharProxy::operator=(const char a_newVal) {
    m_strategy.setChar(m_buffer, m_idx, a_newVal);
    return *this;
}


Canvas::Canvas(size_t a_width, size_t a_height)
: m_width(a_width)
, m_height(a_height)
, m_buffer(SingleLineSize() * a_height, ' ') {
    for (int y = 0 ; y < m_height ; ++y) {
        m_buffer[y * SingleLineSize() + m_width] = '\0';
    }
}


void Canvas::Clear() {
    for (int y = 0 ; y < m_height ; ++y) {
        std::fill(m_buffer.begin() + (y * SingleLineSize()), m_buffer.begin() + (y * SingleLineSize()) + m_width, ' ');
    }
}

size_t Canvas::SingleLineSize() const {
    return m_width + NULLCHAR_SIZE;
}

Canvas::CharProxy Canvas::operator[](const Point &a_point) {
    return operator()(a_point.GetX(), a_point.GetY());
}

char Canvas::operator[](const Point &a_point) const {
    return const_cast<Canvas *>(this)->operator()(a_point.GetX(), a_point.GetY());
}

Canvas::CharProxy Canvas::operator()(int a_x, int a_y) {
    return CharProxy(
        IsLegalPosition(a_x, a_y) ? g_legalPositionCharStrategy : g_outOfBoundsCharStrategy, 
        m_buffer.data(), a_x + a_y * SingleLineSize());
}

char Canvas::operator()(int a_x, int a_y) const {
    return const_cast<Canvas *>(this)->operator()(a_x, a_y);
}

size_t Canvas::GetWidth() const {
    return m_width;
}

size_t Canvas::GetHeight() const {
    return m_height;
}

bool Canvas::IsLegalPosition(const Point &a_point) const { 
    return IsLegalPosition(a_point.GetX(), a_point.GetY());
}

bool Canvas::IsLegalPosition(int a_x, int a_y) const {
    return (a_x >= 0) && (a_y >= 0) && (a_x < m_width) && (a_y < m_height);
}

void Canvas::Print(std::ostream &a_out) const {
    for (size_t y = 0 ; y < m_height ; ++y) {
        a_out << &m_buffer[y * SingleLineSize()] << std::endl;
    }
}

void DrawLine(Canvas &a_canvas, char a_brush, int a_startX, int a_startY, int a_endX, int a_endY) {
    unsigned int dx = abs(a_endX - a_startX);   // dx is the delta for x
    unsigned int dy = abs(a_endY - a_startY);   // dy is the delta for y
    unsigned int stepVal = 0;  // variable for figuring out when to the other axis 
    if(dx > dy) {   // The slope is less than 45 degres
        if (a_startX > a_endX) {
            std::swap(a_startX, a_endX);
            std::swap(a_startY, a_endY);
        }
        int yDirection = a_startY <= a_endY ? 1 : -1;
        for ( ; a_startX <= a_endX; ++a_startX) {
            stepVal += dy;
            if(stepVal >= dx) { // Increment y if enough x steps have been taken.
                stepVal -= dx;    // "Reset" StepVal by removing a dx amount
                a_startY += yDirection;
            }

            a_canvas(a_startX, a_startY) = a_brush; // Draw a point
        }
    }
    else { // The slope is greater than 45 degrees, just like above, but with y instead of x.
        if (a_startY > a_endY) {
            std::swap(a_startX, a_endX);
            std::swap(a_startY, a_endY);
        }
        int xDirection = a_startX <= a_endX ? 1 : -1;
        for ( ; a_startY <= a_endY; ++a_startY) {
            stepVal += dx;
            if(stepVal >= dy) { // Increment y if enough x steps have been taken.
                stepVal -= dy;    // "Reset" StepVal by removing a dx amount
                a_startX += xDirection;
            }

            a_canvas(a_startX, a_startY) = a_brush; // Draw a point
        }
    }        
}

// draw circle using mid-point circle algorithm 
void DrawCircle(Canvas &a_canvas, char a_brush, int a_centerX, int a_centerY, double a_radius) {
    int dx = a_radius, dy = 0;
      
    // Printing the initial point on the axes after translation
    a_canvas(a_centerX + dx, a_centerY) = a_brush;
      
    // When radius is zero only a single point will be printed
    if (a_radius > 0.0) {
        a_canvas(a_centerX - dx, a_centerY) = a_brush;
        a_canvas(a_centerX, a_centerY + dx) = a_brush;
        a_canvas(a_centerX, a_centerY - dx) = a_brush;
    }
      
    // Initialising the value of P
    double P = 1 - a_radius;
    for (int dy = 1 ; dx > dy ; ++dy) { 
        // Mid-point is inside or on the perimeter
        if (P <= 0) {
            P = P + 2*dy + 1;
        }
        else { // Mid-point is outside the perimeter
            --dx;
            // All the perimeter points have already been printed
            if (dx < dy) {
                break;
            }

            P = P + 2*dy - 2*dx + 1;
        }
          
          
        a_canvas(a_centerX + dx, a_centerY + dy) = a_brush;
        a_canvas(a_centerX - dx, a_centerY + dy) = a_brush;
        a_canvas(a_centerX + dx, a_centerY - dy) = a_brush;
        a_canvas(a_centerX - dx, a_centerY - dy) = a_brush;
          
        // If the generated point is on the line x = y then 
        // the perimeter points have already been printed
        if (dx != dy) {
            a_canvas(a_centerX + dy, a_centerY + dx) = a_brush;
            a_canvas(a_centerX - dy, a_centerY + dx) = a_brush;
            a_canvas(a_centerX + dy, a_centerY - dx) = a_brush;
            a_canvas(a_centerX - dy, a_centerY - dx) = a_brush;
        }
    }
}

void DrawLine(Canvas &a_canvas, char a_brush, const Point &a_start, const Point &a_end) {
    DrawLine(a_canvas, a_brush, a_start.GetX(), a_start.GetY(), a_end.GetX(), a_end.GetY());
}

void DrawCircle(Canvas &a_canvas, char a_brush, const Point &a_center, double a_radius) {
    DrawCircle(a_canvas, a_brush, a_center.GetX(), a_center.GetY(), a_radius);
}

} // ascii
