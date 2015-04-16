#ifndef VERTEX_H
#define VERTEX_H

#include "vector.h"

class Vertex {
  public:
	  Vector point;
	  Vector normal;
	  bool hasNormal;
	  float u,v;
	  Vertex(Vector& );
	  Vertex(Vector&, Vector&);
	  Vertex(Vector&, Vector&, float, float);
};

#endif
