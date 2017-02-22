#pragma once

namespace OGraphics
{
    struct Vertex
    {
        GLfloat x, y, z;
        bool operator== (const Vertex& other) const { return x == other.x && y == other.y && z == other.z; }
        bool operator<(const Vertex& other) const { return x*y < other.x * other.y; }
    };
    struct UV
    {
        GLfloat u, v;
        bool operator== (const UV& other) const { return u == other.u && v == other.v; }
        bool operator<(const UV& other) const { return u*v < other.u * other.v; }
    };
    struct Rect
    {
        float left, top, width, height;
        bool operator == (const Rect& other) const { return left == other.left && top == other.top && width == other.width && height == other.height; }
    };
}

namespace OGUI
{
    struct TexturePos
    {
        int l1, l2, l3, l4, t1, t2, t3, t4;
    };

    struct WidgetRects
    {
        OGraphics::Rect topLeft, top, topRight, left, center, right, bottomLeft, bottom, bottomRight;
    };
}
