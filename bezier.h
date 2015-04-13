#ifndef BEZIER_H
#define BEZIER_H

#include "vector.h"
#include "vertex.h"
#include <vector>

class Bezier {
  Vector patch[4][4];
  float threshold;

  std::vector<Vertex> vertices;
  std::vector<int*> triangles;

  void uniformSampling();
  void adaptiveSampling();
  void adaptiveSplit(int, int, int);

  Bezier(Vector patch[4][4], float param, bool adaptive);
  Vertex bezPatchInterp(float u, float v);

  void draw();
};

#endif
