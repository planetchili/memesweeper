#pragma once

class Vector2D
{
public:
	Vector2D() = default;
	Vector2D(int _x, int _y);
	Vector2D operator+(const Vector2D& vect) const;
	Vector2D& operator+=(const Vector2D& vect);
	Vector2D operator-(const Vector2D& vect) const;
	Vector2D& operator-=(const Vector2D& vect);
	Vector2D operator*(const int multiplyer) const;
	Vector2D& operator*=(const int multiplyer);

public:
	int x;
	int y;
};

