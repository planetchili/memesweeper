#include "Vector2D.h"

Vector2D::Vector2D(int _x, int _y)
    :
    x(_x),
    y(_y)
{
}

Vector2D Vector2D::operator+(const Vector2D& vect) const
{
    return Vector2D(this->x + vect.x, this->y + vect.y);
}

Vector2D& Vector2D::operator+=(const Vector2D& vect)
{
    return *this = *this + vect;
}

Vector2D Vector2D::operator-(const Vector2D& vect) const
{
    return Vector2D(this->x - vect.x, this->y - vect.y);
}

Vector2D& Vector2D::operator-=(const Vector2D& vect)
{
    return *this = *this - vect;
}

Vector2D Vector2D::operator*(const int multiplyer) const
{
    return Vector2D(this->x * multiplyer, this->y * multiplyer);


}

Vector2D& Vector2D::operator*=(const int multiplyer)
{
    return *this = *this * multiplyer;
}


