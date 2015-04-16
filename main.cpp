#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <cmath>
#include <limits>
#include <sstream>
#include <fstream>


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

#include <cstdlib>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "vector.h"
#include "bezier.h"
using std::cout;
using std::endl;

#include <fstream>
using std::ifstream;

#include <cstring>

#define SPACEBAR 32
const int MAX_CHARS_PER_LINE = 512;
const int MAX_TOKENS_PER_LINE = 12;
const char* const DELIMITER = " ";

using std::vector;

enum Shading {
  FLAT,
  SMOOTH
};
Shading shading = SMOOTH;

enum Mode {
  FILLED,
  WIREFRAME,
  HIDDEN
};
Mode mode = FILLED;

class Viewport;

class Viewport {
  public:
    int w, h; // width and height
};


//****************************************************
// Global Variables
//****************************************************
Viewport  viewport;
std::vector<std::vector<Bezier> > bez;
std::vector<double> bezCenters;
std::vector<double> trans;
std::vector<double> rot;
int current_obj = 0;

float curr_fill_amb[3] = {0.3f, 0.0f, 0.0f};
float curr_fill_diff[3] = {1.0f, 0.0f, 0.0f};
float curr_fill_spec[3] = {1.0f, 1.0f, 1.0f};

float curr_wire_amb[3] = {1.0f, 0.0f, 0.0f};
float curr_wire_diff[3] = {0.5f, 0.0f, 0.0f};
float curr_wire_spec[3] = {0.0f, 0.0f, 0.0f};

float fill_amb[3] = {0.0f, 0.3f, 0.3f};
float fill_diff[3] = {0.0f, 1.0f, 1.0f};
float fill_spec[3] = {1.0f, 1.0f, 1.0f};

float wire_amb[3] = {0.0f, 1.0f, 1.0f};
float wire_diff[3] = {0.0f, 0.5f, 0.5f};
float wire_spec[3] = {0.0f, 0.0f, 0.0f};


//****************************************************
// reshape viewport if the window is resized
//****************************************************
void myReshape(int w, int h) {
  viewport.w = w;
  viewport.h = h;
  
  glViewport (0,0,viewport.w,viewport.h);
  glMatrixMode(GL_PROJECTION);
  if (h == 0) {
    h = 5;
  }
  float ratio = (float)w /(float)h;
  glLoadIdentity();
  gluPerspective(45.0f, ratio, 0.1f, 200.0f);
}


//****************************************************
// Simple init function
//****************************************************
void initScene(){

  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClearDepth(1.0f);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);
  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

//   GLuint depthTexture;
//   glGenTextures(1, &depthTexture);
//   glBindTexture(GL_TEXTURE_2D, depthTexture);

  GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
  GLfloat mat_shininess[] = { 50.0 };
  GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };
  glClearColor (0.0, 0.0, 0.0, 0.0);

  glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
  glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
  glLightfv(GL_LIGHT0, GL_POSITION, light_position);

  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);

}

//****************************************************
// read in a bez file
//****************************************************
void readBEZ(std::string filename, double threshold, bool uniform){


	// create a file-reading object
	ifstream fin;
	fin.open(filename); // open a file
	if (!fin.good())
		printf("file not found");
		exit(0);
	vector<Bezier> bez;
	float coords[48];
	int line = 0;
	// read each line of the file

	
	while (!fin.eof())
	{	
		if (line == 0){
			continue;
		}
		// read an entire line into memory
		char buf[MAX_CHARS_PER_LINE];
		fin.getline(buf, MAX_CHARS_PER_LINE);

		// parse the line into blank-delimited tokens
		int n = 0; // a for-loop index

		// array to store memory addresses of the tokens in buf
		const char* token[MAX_TOKENS_PER_LINE] = {}; // initialize to 0

		// parse the line
		token[0] = strtok(buf, DELIMITER); // first token
		if (token[0]) // zero if line is blank
		{
			for (n = 1; n < MAX_TOKENS_PER_LINE; n++)
			{
				token[n] = strtok(0, DELIMITER); // subsequent tokens
				if (!token[n]) break; // no more tokens
			}
		}
		//place tokens into array of coords
		for (int i = 0; i < n; i++) // n = #of tokens
			coords[line * 4 + i] = atof(token[i]);

		// process (print) the tokens
		if (line == 4){ //process coords into bez object
			Vector v1 =  Vector(coords[0], coords[1], coords[2]);
			Vector v2 = Vector(coords[12], coords[13], coords[14]);
			Vector v3 = Vector(coords[24], coords[25], coords[26]);
			Vector v4 = Vector(coords[36], coords[37], coords[38]);

			Vector v5 = Vector(coords[3], coords[4], coords[5]);
			Vector v6 = Vector(coords[15], coords[16], coords[17]);
			Vector v7 = Vector(coords[27], coords[28], coords[29]);
			Vector v8 = Vector(coords[39], coords[40], coords[41]);

			Vector v9 = Vector(coords[6], coords[7], coords[8]);
			Vector v10 = Vector(coords[18], coords[19], coords[20]);
			Vector v11 = Vector(coords[30], coords[31], coords[32]);
			Vector v12 = Vector(coords[42], coords[43], coords[44]);

			Vector v13 = Vector(coords[9], coords[10], coords[11]);
			Vector v14 = Vector(coords[21], coords[22], coords[23]);
			Vector v15 = Vector(coords[33], coords[34], coords[35]);
			Vector v16 = Vector(coords[45], coords[46], coords[47]);
		
			Vector vectors[4][4] = {v1,v2,v3,v4,
									v5,v6,v7,v8,
									v9,v10,v11,v12,
									v13,v14,v15,v16};
			bez.push_back(Bezier(vectors,threshold,uniform));
			line = -1;
		}
		
		line++;
	}
}

void argParser(int* argc, char** argv) {
  std::string input_filename = "";
  std::string ext = "";
  bool uniform = true;
  double threshold = 0.1;

  std::string arg;
  for (int i=1; i < *argc; i++) {
    arg = argv[i];

    if (i==1) {
      input_filename = arg;

      int dotIndex = input_filename.find_last_of(".");
      ext = input_filename.substr(dotIndex+1,input_filename.size());
      
    }
    else if (i==2 && std::atof(argv[i])!=0) {
      threshold = std::atof(argv[i]);
    }
    else if (arg.compare("-a")==0) {
      uniform = false;
    }
  }

  if (ext.compare("bez")==0) {
    readBEZ(input_filename,threshold,uniform);
  }

  //Initial offset to fit model on screen
  double center[3] = {0,0,0};
  bezCenters = std::vector<double>(center,center+3);
  double translation[3] = {0,0,-10};
  trans = std::vector<double>(translation,translation+3);
  double rotation[3] = {0,0,0};
  rot = std::vector<double>(rotation,rotation+3);
}

//****************************************************
// functions that do the actual drawing of stuff
//***************************************************
void setColor() {
  if (mode==FILLED) { 
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, curr_fill_amb);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, curr_fill_diff);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, curr_fill_spec);
  } else {
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, curr_wire_amb);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, curr_wire_diff);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, curr_wire_spec);
  } 
}

void drawObjects() {
  int obj_index;
  for (int k=0; k<bez.size(); k++){
    setColor();
    glLoadIdentity(); // make sure transformation is "zero'd"
    glTranslatef(trans[0], trans[1], trans[2]);
    glRotatef(rot[0],1.0,0.0,0.0);
    glRotatef(rot[1],0.0,1.0,0.0);
    glRotatef(rot[2],0.0,0.0,1.0);
    glTranslatef(-bezCenters[0], -bezCenters[1], -bezCenters[2]);
    for (int i=0; i<bez[k].size(); i++) {
      bez[k][i].draw();
    }
  }
}

void myDisplay() {

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);                // clear the color buffer (sets everything to black), and the depth buffer.

  glMatrixMode(GL_MODELVIEW);			        // indicate we are specifying camera transformations

  // Code to draw objects
  if (shading==FLAT) glShadeModel(GL_FLAT);
  else glShadeModel(GL_SMOOTH);

  if (mode==FILLED) {
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  }
  else {
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  }
  drawObjects();

  if (mode==HIDDEN) {
    // http://www.glprogramming.com/red/chapter14.html#name16
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glEnable(GL_POLYGON_OFFSET_FILL);
    glPolygonOffset(1.0, 1.0);
    glColor3f(0.0f, 0.0f, 0.0f); // Background color
    glDisable(GL_LIGHTING);
    drawObjects();
    glEnable(GL_LIGHTING);
    glDisable(GL_POLYGON_OFFSET_FILL);
  }

  glutPostRedisplay();
  glFlush();
  glutSwapBuffers();					// swap buffers (we earlier set double buffer)
}

//****************************************************
// function to process keyboard input
//***************************************************
void myKeyboard(unsigned char key, int x, int y) {
  if (key == 'q') {
    exit(0);
  }
  if (key == 's') {
    //toggle between flat and smooth shading
    if (shading==SMOOTH) shading=FLAT;
    else shading=SMOOTH;
  }
  if (key == 'w') {
    //toggle between filled and wireframe mode
    if (mode!=WIREFRAME) mode=WIREFRAME;
    else mode=FILLED;
  }
  if (key == 'h') {
    //toggle between filled and hidden-line mode
    if (mode!=HIDDEN) mode=HIDDEN;
    else mode=WIREFRAME;
  }
  if (key == '+') {
    trans[2]+=0.1;
  }
  if (key == '-') {
    trans[2]-=0.1;
  }

  glutPostRedisplay();
}

void myArrowKeys(int key, int x, int y) {
  int modifier = glutGetModifiers();
  if (modifier&GLUT_ACTIVE_SHIFT != 0) {
    //Translate
    switch(key) {
      case GLUT_KEY_UP:
        trans[1]+=0.1;
        break;
      case GLUT_KEY_DOWN:
        trans[1]-=0.1;
        break;
      case GLUT_KEY_LEFT:
        trans[0]-=0.1;
        break;
      case GLUT_KEY_RIGHT:
        trans[0]+=0.1;
        break;
    }
  } else {
    //Rotate
    switch(key) {
      case GLUT_KEY_UP:
        rot[0]+=2;
        break;
      case GLUT_KEY_DOWN:
        rot[0]-=2;
        break;
      case GLUT_KEY_LEFT:
        rot[1]-=2;
        break;
      case GLUT_KEY_RIGHT:
        rot[1]+=2;
        break;
    }
  }
}


//****************************************************
// the usual stuff, nothing exciting here
//****************************************************
int main(int argc, char *argv[]) {

  //Parses Args
  argParser(&argc, argv);


  //This initializes glut
  glutInit(&argc, argv);

  //This tells glut to use a double-buffered window with red, green, and blue channels, and a depth buffer.
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

  // Initalize theviewport size
  viewport.w = 400;
  viewport.h = 400;

  //The size and position of the window
  glutInitWindowSize(viewport.w, viewport.h);
  glutInitWindowPosition(0,0);
  glutCreateWindow(argv[0]);

  initScene();							// quick function to set up scene

  glutKeyboardFunc(myKeyboard);           // function to run when its time to read keyboard input
  glutSpecialFunc(myArrowKeys);           // function to run when arrow keys are pressed
  glutDisplayFunc(myDisplay);             // function to run when its time to draw something
  glutReshapeFunc(myReshape);             // function to run when the window gets resized
  glutMainLoop();                         // infinite loop that will keep drawing and resizing

  return 0;
}
