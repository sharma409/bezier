#include "vertex.h"

Vertex::Vertex (Vector& p) {
  point = p; hasNormal = false; u = -1; v = -1;
}

Vertext::Vertex (Vector& p, Vector& n)
  point = p; normal = n; hasNormal = true; u = -1; v = -1;
}

Vertex::Vertex (Vector& p, Vector& n, float u_, float v_) {
  point = p; normal = n; hasNormal = true; u = u_; v = v_;
}