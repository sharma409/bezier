#ifndef BEZIER_H
#define BEZIER_H

#include "vector.h"
#include "vertex.h"
#include <vector>


typedef struct {
  Vector point;
  Vector deriv;
} Derivative;

class Bezier {
  public:
    Vector patch[4][4];
    float threshold;
    int index;

    std::vector<Vertex> vertices;
    std::vector<std::vector<int> > triangles;

    void sampleUniformly();
    // void sampleAdaptively();
    // void split(int, int, int);

    Bezier(Vector patch[4][4], float param, bool adaptive);
    Derivative bezCurveInterp(Vector curve[4], float u);
    Vertex bezPatchInterp(float, float);

    void draw();
};

#endif
