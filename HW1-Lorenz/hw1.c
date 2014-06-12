#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>


GLuint Lorenz_Curve;
static GLfloat xrot = 25.0, yrot = 50.0, zrot = 35.0;
double dimx = 5.0, dimy = 5.0, dimz = 5.0;
/*  Lorenz Parameters  */
double s  = 10;
double b  = 2.6666;
double r  = 28;

//Print function from ex6.c
#define LEN 8192
void Print(const char* format, ...)
{
  char buf[LEN];
  char* ch=buf;
  va_list args;
  va_start(args, format);
  vsnprintf(buf,LEN,format,args);
  va_end(args);
  while(*ch)
    {
      glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *ch++);
    }
}  

// Lorenz Integration
void lorenzPntGen(){
  // Coordinates and time step
  double x = 1;
  double y = 1; 
  double z = 1; 
  double dt = 0.001;
  int i;	 
  glBegin(GL_LINE_STRIP);
  for (i=0; i<50000; i++)
    {
      double dx = s*(y-x);
      double dy = x*(r-z)-y;
      double dz = x*y - b*z;
      x += dt*dx;
      y += dt*dy;
      z += dt*dz;
      glColor3d(x/y, y/z, z/x);
      glVertex3d(x*0.05, y*0.05, z*0.05);
    } 
  glEnd();
}

// Display list
static void init(void)
{
  Lorenz_Curve = glGenLists (1);
  glNewList(Lorenz_Curve, GL_COMPILE);
  lorenzPntGen();
  glEndList();  
}
// Display function drawing from display list
void display(){
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glPushMatrix();	
  glRotatef(xrot, 1.0, 0.0, 0.0);
  glRotatef(yrot, 0.0, 1.0, 0.0);
  glRotatef(zrot, 0.0, 0.0, 1.0);
  glCallList(theLine);
  glColor3f(1.0, 1.0, 1.0);
  glBegin(GL_LINES);
  glVertex3d(0,0,0);
  glVertex3d(1,0,0);
  glVertex3d(0,0,0);
  glVertex3d(0,1,0);
  glVertex3d(0,0,0);
  glVertex3d(0,0,1);
  glEnd(); 
  glRasterPos3d(1,0,0);
  Print("X");
  glRasterPos3d(0,1,0);
  Print("Y");
  glRasterPos3d(0,0,1);
  Print("Z");
  glWindowPos2i(5,5);
  Print("s = %.3f b = %.3f r = %.3f",s,b,r); 
  glFlush();
  glutSwapBuffers();
  glPopMatrix();
}

// Function for reshaping window
void reshape(int width, int height){
  double w2h = (double)width/height;
  glViewport(0, 0, (GLint) width, (GLint) height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  if(w2h > 1)
    {
      glOrtho(-dimx*w2h, dimx*w2h, -dimy, dimy, -dimz, dimz);
    }
  else
    {
      glOrtho(-dimx, dimx, -dimy/w2h, dimy/w2h, -dimz, dimz);
    }
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity(); 
}

// Keyboard shortcuts for rotation
void special( int k, int x, int y){
  switch (k) {
  // negative rotate of y axis with left arrow key
  case GLUT_KEY_LEFT:
    yrot -= 5.0;
    break;
  // positive rotate of y axis with right arrow key
  case GLUT_KEY_RIGHT:
    yrot += 5.0;
    break;
  // negative rotate of x axis with down arrow key
  case GLUT_KEY_DOWN:
    xrot -= 5.0;
    break;
  // positive rotate of x axis with up arrow key	  
  case GLUT_KEY_UP:
    xrot += 5.0;
    break;
  default:
    return;
  }
  glutPostRedisplay();
}

// Keyboard shortcuts for changing Lorenz Parameters and zooming
void key( unsigned char k, int x, int y)
{
  int width = glutGet(GLUT_WINDOW_WIDTH);
  int height = glutGet(GLUT_WINDOW_HEIGHT);
  double w2h = width/height;
  switch(k)
    {
    // Zoom in with f key
    case 'f':
      glMatrixMode(GL_PROJECTION);
      glLoadIdentity();
      dimx -= 0.2;
      dimy -= 0.2;
      if(w2h > 1)
	{
	  glOrtho(-dimx*w2h, dimx*w2h, -dimy, dimy, -dimz, dimz);
	}
      else
	{
	  glOrtho(-dimx, dimx, -dimy/w2h, dimy/w2h, -dimz, dimz);
	}
      glMatrixMode(GL_MODELVIEW);
      glLoadIdentity();
      break;
    // Zoom out with g key
    case 'g':
      glMatrixMode(GL_PROJECTION);
      glLoadIdentity();
      dimx += 0.2;
      dimy += 0.2;
      if(w2h > 1)
	{
	  glOrtho(-dimx*w2h, dimx*w2h, -dimy, dimy, -dimz, dimz);
	}
      else
	{
	  glOrtho(-dimx, dimx, -dimy/w2h, dimy/w2h, -dimz, dimz);
	}
      glMatrixMode(GL_MODELVIEW);
      glLoadIdentity();
      break;
    // Increase s parameter with z key
    case 'z':
      s += .1;
      init();
      break;
    // Decrease s parameter with x key
    case 'x':
      s -= .1;
      init();
      break;
    // Increase b parameter with a key
    case 'a':
      b += .1;
      init(); 
      break;
    // Decrease b parameter with s key
    case 's':
      b -= .1;
      init();
      break;
    // Increase r parameter with q key
    case 'q':
      r += .1;
      init();
      break;
    // Decrease r parameter with w key
    case 'w':
      r -= .1;
      init();
      break;
    }
  glutPostRedisplay();
} 
// Main
int main(int argc, char* argv[]){
  // Initialize glut with parameters	
  glutInit(&argc, argv);
  // Set display to double buffered with color	
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
  // Initialize window size
  glutInitWindowSize(500, 500);
  // Create window with title
  glutCreateWindow("HW1:Lorenz Attractor - Chris Rooney");
  init();
  // Draws scene
  glutDisplayFunc(display);
  // Reshapes window
  glutReshapeFunc(reshape);
  // Handles user input with arrow keys
  glutSpecialFunc(special);
  // Handles user input with z/x/a/s/q/w/f/g keys
  glutKeyboardFunc(key);
  // Loop until program exits	
  glutMainLoop();
  return EXIT_SUCCESS;
}
