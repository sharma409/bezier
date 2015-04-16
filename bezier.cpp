#ifdef _WIN32
#include <windows.h>
#else
#include <sys/time.h>
#endif

#ifdef OSX
#include <GLUT/glut.h>
#include <OpenGL/glu.h>
#else
#include <GL/glut.h>
#include <GL/glu.h>
#endif

#include "bezier.h"
#include <iostream>
Bezier::Bezier(Vector patch_[4][4], float threshold_, bool uniform) {
  threshold = threshold_;
  for (int i=0; i<4; i++) {
    for (int j=0; j<4; j++) {
      patch[i][j]=patch_[i][j];
    }
  }
  uniform = true;
  if (uniform)
    sampleUniformly();
  else
    ;
    // sampleAdaptively();
}

Vertex Bezier::bezPatchInterp(float u, float v){
  // given a control patch and (u,v) values, find
  // the surface point and normal
  Vector vcurve[4];
  Vector ucurve[4];
  for(int i = 0; i < 4; i++){
    // build control points for a Bezier curve in v
    vcurve[i] = bezCurveInterp(patch[i], u).point;

    // build control points for a Bezier curve in u
    Vector temp[4];
    temp[0] = patch[0][i];
    temp[1] = patch[1][i];
    temp[2] = patch[2][i];
    temp[3] = patch[3][i];
    ucurve[i] = bezCurveInterp(temp, v).point;
  }

  // evaluate surface and derivative for u and v
  Derivative p_dPdv = bezCurveInterp(vcurve, v);
  Derivative p_dPdu = bezCurveInterp(ucurve, u);

  // take cross product of partials to find normal
  Vector n = p_dPdu.deriv.cross(p_dPdv.deriv);
  n.normalize();

  return Vertex(p_dPdu.point, n, u, v);
};

Derivative Bezier::bezCurveInterp(Vector curve[4], float u){
  // first, split each of the three segments
  // to form two new ones AB and BC
  // Vector A = curve[0] * (1.0f-u) + curve[1] * u;
  Vector A = curve[0] * (1.0f);
  Vector B = curve[1] * (1.0f-u) + curve[2] * u;
  Vector C = curve[2] * (1.0f-u) + curve[3] * u;

  // now, split AB and BC to form a new segment DE
  Vector D = A * (1.0f-u) + B * u;
  Vector E = B * (1.0f-u) + C * u;

  Derivative p_dPdu;
  // finally, pick the right point on DE,
  // this is the point on the curve
  p_dPdu.point = D * (1.0f-u) + E * u;

  // compute derivative also
  p_dPdu.deriv = (E - D) * 3;

  return p_dPdu;
};

void Bezier::sampleUniformly() {
  for (int u = 0; u * threshold <= 1; u++) {
    for (int v = 0; v * threshold <= 1; v++) {
      Vertex vert = bezPatchInterp(u * threshold, v * threshold);
      vertices.push_back(vert);
    }
  }

  int total_steps = (int)(1/threshold)+1;

  for (int i = 0; i < total_steps-1; i++) {
    for (int j = 0; j < total_steps-1; j++) {
      int indices[4];
      indices[0] = i*total_steps+j;
      indices[1] = (i+1)*total_steps+j;
      indices[2] = indices[0]+1;
      indices[3] = indices[1]+1;

      std::vector<int> tri1 = std::vector<int>(indices,indices+3);
      std::vector<int> tri2 = std::vector<int>(indices+1,indices+4);
      triangles.push_back(tri1);
      triangles.push_back(tri2);
    }
  }
}


void Bezier::draw() {
  for (int i=0; i < triangles.size(); i++) {
    glBegin(GL_TRIANGLES);
    for (int j=0; j < 3; j++){
      int index = triangles[i][j];
      Vertex v = vertices[index];
      glVertex3f(v.point.x, v.point.y, v.point.z);
      if (v.hasNormal) {
        glNormal3f(v.normal.x, v.normal.y, v.normal.z);
      }
    }
    glEnd();
  }
}
