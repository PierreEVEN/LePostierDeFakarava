
#pragma once

struct ProceduralLandscapeVector
{
	ProceduralLandscapeVector() {}

	ProceduralLandscapeVector(float posX, const float& posY, const float& posZ)
		: X(posX), Y(posY), Z(posZ) {}

	float X;
	float Y;
	float Z;

	ProceduralLandscapeVector operator-(const ProceduralLandscapeVector& other)
	{
		return ProceduralLandscapeVector(X - other.X, Y - other.Y, Z - other.Z);
	}

	ProceduralLandscapeVector operator+(const ProceduralLandscapeVector& other)
	{
		return ProceduralLandscapeVector(X + other.X, Y + other.Y, Z + other.Z);
	}

	ProceduralLandscapeVector operator/(const float& other)
	{
		return ProceduralLandscapeVector(X / other, Y / other, Z / other);
	}

	ProceduralLandscapeVector Normalize() const
	{
		return ProceduralLandscapeVector(X, Y, Z) / GetLength();
	}

	float GetLength() const
	{
		return sqrt(X * X + Y * Y + Z * Z);
	}

	static ProceduralLandscapeVector CrossProduct(const ProceduralLandscapeVector& A, const ProceduralLandscapeVector& B)
	{
		return ProceduralLandscapeVector
		(
			A.Y * B.Z - A.Z * B.Y,
			A.Z * B.X - A.X * B.Z,
			A.X * B.Y - A.Y * B.X
		);
	}
};

struct ProceduralLandscapeVector2D
{
	ProceduralLandscapeVector2D() {}
	ProceduralLandscapeVector2D(const float& posX, const float& posY)
		: X(posX), Y(posY) {}

	ProceduralLandscapeVector2D(const float& pos)
		: X(pos), Y(pos) {}

	float X;
	float Y;

	ProceduralLandscapeVector2D operator-(const ProceduralLandscapeVector2D& other)
	{
		return ProceduralLandscapeVector2D(X - other.X, Y - other.Y);
	}

};

struct ProceduralLandscapeColor
{
	char r;
	char g;
	char b;
	char a;
};
