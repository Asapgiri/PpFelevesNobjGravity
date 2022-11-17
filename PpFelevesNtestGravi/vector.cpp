#include <math.h>
#include "vector.h"


Vector Vector::operator + (Vector const& obj) {
	Vector res;

	res.x = x + obj.x;
	res.y = y + obj.y;
	res.z = z + obj.z;

	return res;
}

Vector Vector::operator - (Vector const& obj) {
	Vector res;

	res.x = x - obj.x;
	res.y = y - obj.y;
	res.z = z - obj.z;

	return res;
}

Vector Vector::operator * (double skalar) {
	Vector res;

	res.x = x * skalar;
	res.y = y * skalar;
	res.z = z * skalar;

	return res;
}

void Vector::operator += (Vector const& obj) {
	x = x + obj.x;
	y = y + obj.y;
	z = z + obj.z;
}

void Vector::operator -= (Vector const& obj) {
	x = x - obj.x;
	y = y - obj.y;
	z = z - obj.z;
}

void Vector::operator *= (double skalar) {
	x = x * skalar;
	y = y * skalar;
	z = z * skalar;
}

void Vector::operator += (double skalar) {
	x = x + skalar;
	y = y + skalar;
	z = z + skalar;
}

void Vector::operator -= (double skalar) {
	x = x - skalar;
	y = y - skalar;
	z = z - skalar;
}

void Vector::Zeros() {
	x = 0.0;
	y = 0.0;
	z = 0.0;
}

double Vector::Size() {
	return sqrt((x * x) + (y * y) + (z * z));
}

void Vector::operator = (Vector const& obj) {
	x = obj.x;
	y = obj.y;
	z = obj.z;
}
