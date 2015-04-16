#ifndef VECTOR_H
#define VECTOR_H


class Vector {
  public:
    float x;
    float y;
    float z;
    Vector();
    Vector(float,float,float);
    float norm();
    void normalize();
    Vector cross(Vector&);
    float dot(Vector&);
    Vector operator - ();
    Vector operator + (const Vector&);
    Vector& operator += (const Vector&);
    Vector operator - (const Vector&);
    Vector& operator -= (const Vector&);
    Vector operator * (const float);
    Vector& operator *= (const float);
    Vector operator / (const float);
    Vector& operator /= (const float);
};

#endif