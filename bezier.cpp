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

Bezier::Bezier(Vector patch_[4][4], float threshold_, bool uniform) {
  threshold = threshold_;
  for (int i=0; i<4; i++) {
    for (int j=0; j<4; j++) {
      patch[i][j]=patch_[i][j];
    }
  }
  if (uniform)
    sampleUniformly();
  else
    sampleAdaptively();
  }
}

Vertex Bezier::bezPatchInterp(float u, float v){
  // given a control patch and (u,v) values, find
  // the surface point and normal
  Vector vcurve[4];
  Vector ucurve[4];
  for(int i = 0; i < 4; i++){
    // build control points for a Bezier curve in v
    vcurve[i] = bezCurveInterp(patch[i], u)[0];

    // build control points for a Bezier curve in u
    Vector temp[4];
    temp[0] = patch[0][i];
    temp[1] = patch[1][i];
    temp[2] = patch[2][i];
    temp[3] = patch[3][i];
    ucurve[i] = bezCurveInterp(temp, v)[0];
  }

  // evaluate surface and derivative for u and v
  p_dPdv = bezCurveInterp(vcurve, v);
  p_dPdu = bezCurveInterp(ucurve, u);

  // take cross product of partials to find normal
  Vector n = p_dPdu[1].cross(p_dPdv[1]);
  n.normalize();

  return Vertex(p_dPdu[0], n, u, v);
};

VectorDeriv Bezier::bezCurveInterp(Vector curve[4], float u){
  // first, split each of the three segments
  // to form two new ones AB and BC
  Vector A = curve[0] * (1.0-u) + curve[1] * u;
  Vector B = curve[1] * (1.0-u) + curve[2] * u;
  Vector C = curve[2] * (1.0-u) + curve[3] * u;

  // now, split AB and BC to form a new segment DE
  Vector D = A * (1.0-u) + B * u;
  Vector E = B * (1.0-u) + C * u;

  Vector[2] p_dPdu;
  // finally, pick the right point on DE,
  // this is the point on the curve
  p_dPdu[0] = D * (1.0-u) + E * u;

  // compute derivative also
  p_dPdu[1] = (E - D) * 3;

  return p_dPdu;
};

void Bezier::sampleUniformly() {
  for (int u = 0; u * threshold <= 1; u++) {
    for (int v = 0; v * threshold <= 1; v++) {
      Vertex v = bezPatchInterp(u*threshold,v*threshold);
      vertices.push_back(v);
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
      triangles.push_back(indices);
      triangles.push_back(indices+1);
    }
  }
}

void Bezier::sampleAdaptively() {
  vertices.push_back(bezPatchInterp(0,0));
  vertices.push_back(bezPatchInterp(1,0));
  vertices.push_back(bezPatchInterp(0,1));
  vertices.push_back(bezPatchInterp(1,1));
  split(2,3,1);
  split(1,0,2);
}

void Bezier::split(int i1, int i2, int i3) {
  Vertex v1 = vertices[i1];
  Vertex v2 = vertices[i2];
  Vertex v3 = vertices[i3];
  Vertex p1 = bezPatchInterp((v1.u + v2.u)*0.5, (v1.v + v2.v)*0.5);
  Vertex p2 = bezPatchInterp((v2.u + v3.u)*0.5, (v2.v + v3.v)*0.5);
  Vertex p3 = bezPatchInterp((v3.u + v1.u)*0.5, (v3.v + v1.v)*0.5);

  bool e1 = ((v1.point + v2.point)*0.5 - p1.point).norm() > threshold;
  bool e2 = ((v1.point + v2.point)*0.5 - p2.point).norm() > threshold;
  bool e3 = ((v1.point + v2.point)*0.5 - p3.point).norm() > threshold;

  if (!e1 && !e2 && !e3) {
    int indices[3] = {i1,i2,i3};
    triangles.push_back(indices);
  }
  else if (e1 && !e2 && !e3) {
    vertices.push_back(p1);
    int p1_index = vertices.size()-1;
    split(i1,p1_index,i3);
    split(i3,p1_index,i2);
  }
  else if (!e1 && e2 && !e3) {
    vertices.push_back(p2);
    int p2_index = vertices.size()-1;
    split(i2,p2_index,i1);
    split(i1,p2_index,i3);
  }
  else if (!e1 && !e2 && e3) {
    vertices.push_back(p3);
    int p3_index = vertices.size()-1;
    split(i2,p3_index,i1);
    split(i3,p3_index,i2);
  }
  else if (e1 && e2 && !e3) {
    vertices.push_back(p1);
    vertices.push_back(p2);
    int p2_index = vertices.size()-1;
    int p1_index = p2_index-1;
    split(i1,p1_index,i3);
    split(p1_index,p2_index,i3);
    split(p1_index,i2,p2_index);
  }
  else if (!e1 && e2 && e3) {
    vertices.push_back(p2);
    vertices.push_back(p3);
    int p3_index = vertices.size()-1;
    int p2_index = p3_index-1;
    split(i2,p2_index,i1);
    split(p2_index,p3_index,i1);
    split(p2_index,i3,p3_index);
  }
  else if (e1 && !e2 && e3) {
    vertices.push_back(p1);
    vertices.push_back(p3);
    int p3_index = vertices.size()-1;
    int p1_index = p3_index-1;
    split(i3,p3_index,i2);
    split(p3_index,p1_index,i2);
    split(p3_index,i1,p1_index);
  }
  else {
    vertices.push_back(p1);
    vertices.push_back(p2);
    vertices.push_back(p3);
    int p3_index = vertices.size()-1;
    int p2_index = p3_index-1;
    int p1_index = p3_index-2;

    split(i1,p1_index,p3_index);
    split(p1_index,i2,p2_index);
    split(p3_index,p2_index,i3);
    split(p1_index,p2_index,p3_index);
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
