#ifndef __ASCII_CANVAS__
#define __ASCII_CANVAS__

#include "char_proxy_strategies.hpp"
#include "point.hpp"
#include <iosfwd>
#include <vector>

namespace ascii {

class Canvas {
private:
    class CharProxy {
    public:
        CharProxy(const CharProxyStrategy &a_strategy, char *a_buffer, size_t a_idx);
        CharProxy(const CharProxy &a_other) = default;
        CharProxy &operator=(const CharProxy &a_other);
        ~CharProxy() = default;

        operator char() const;
        CharProxy &operator=(const char a_newVal);

    private:
        const CharProxyStrategy &m_strategy;
        char *m_buffer;
        size_t m_idx;
    };

public:
    Canvas(size_t a_width, size_t a_height);
    Canvas(const Canvas &a_other) = default;
    Canvas &operator=(const Canvas &a_other) = default;
    ~Canvas() = default;

    void Clear();
    CharProxy operator[](const Point &a_point);
    char operator[](const Point &a_point) const;
    CharProxy operator()(int a_x, int a_y);
    char operator()(int a_x, int a_y) const;

    size_t GetWidth() const;
    size_t GetHeight() const;
    bool IsLegalPosition(const Point &a_point) const;
    bool IsLegalPosition(int a_x, int a_y) const;

    void Print(::std::ostream &a_out) const;

private:
    size_t SingleLineSize() const;

    size_t m_width; // must be initialized BEFORE m_buffer (SingleLineSize() depends on it)
    size_t m_height;
    ::std::vector<char> m_buffer;
};

// API:
void DrawLine(Canvas &a_canvas, char a_brush, int a_startX, int a_startY, int a_endX, int a_endY);
void DrawLine(Canvas &a_canvas, char a_brush, const Point &a_start, const Point &a_end);
void DrawCircle(Canvas &a_canvas, char a_brush, int a_centerX, int a_centerY, double a_radius);
void DrawCircle(Canvas &a_canvas, char a_brush, const Point &a_center, double a_radius);

} // ascii

#endif
