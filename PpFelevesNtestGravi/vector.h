#pragma once

class Vector {
public:
	double x;
	double y;
	double z;

	void Zeros();
	double Size();

	Vector operator + (Vector const& obj);
	Vector operator - (Vector const& obj);
	Vector operator * (double skalar);

	void operator += (Vector const& obj);
	void operator -= (Vector const& obj);
	void operator += (double skalar);
	void operator -= (double skalar);
	void operator *= (double skalar);

	void operator = (Vector const& obj);
};