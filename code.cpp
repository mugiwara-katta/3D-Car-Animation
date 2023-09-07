#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>

/*----------------------------------------------3D CAR ANIMATION --------------------------------------------*/

static void SpecialKeyFunc(int Key, int x, int y);

GLint Xsize = 1920;
GLint Ysize = 1080;
float theta;
float i;
GLint day = 1;

GLfloat xt = 0.0, yt = 0.0, zt = 0.0, xw = 0.0;
GLfloat xs = 1.0, ys = 1.0, zs = 1.0;
GLfloat xangle = 0.0, yangle = 0.0, zangle = 0.0, angle = 0.0;

// this is to set the r, g, b colors.
GLfloat r = 0, g = 0, b = 1;
GLfloat r1 = 1, g1 = 1, b1 = 1;
GLint light = 1;
int count = 1, flg = 1;
int aflag = 1;

int flag1 = 0;                // to switch car driving mode
int flag2 = 0, wheelflag = 0; // to switch fog effect

GLUquadricObj *t;

GLvoid Transform(GLfloat Width, GLfloat Height)
{
  glViewport(0, 0, Width, Height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45.0, Width / Height, 0.1, 100.0); // here within distance of 0.1 and 100 units only we are rendering and it is at an angle of 45 degrees.
  glMatrixMode(GL_MODELVIEW);
}

GLvoid InitGL(GLfloat Width, GLfloat Height)
{

  glClearColor(1.0, 1.0, 1.0, 1.0);
  glLineWidth(2.0);         /* Add line width,   ditto */
  Transform(Width, Height); /* Perform the transformation */
  t = gluNewQuadric();
  gluQuadricDrawStyle(t, GLU_FILL);

  //  creating the lighting source.
  glEnable(GL_LIGHTING);

  glEnable(GL_LIGHT0); // light source.

  GLfloat ambientLight[] = {0.2f, 0.2f, 0.2f, 1.0f};
  GLfloat diffuseLight[] = {0.8f, 0.8f, 0.8, 1.0f};
  GLfloat specularLight[] = {0.5f, 0.5f, 0.5f, 1.0f};
  GLfloat position[] = {1.5f, 1.0f, 4.0f, 1.0f};

  glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
  glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
  glLightfv(GL_LIGHT0, GL_POSITION, position);
}

GLvoid DrawGLScene()
{

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  if (count == 1)
    InitGL(Xsize, Ysize);
  if (aflag == 1) /* Initialize our window. */
    glClearColor(1, 1, 1, 1);
  else
    glClearColor(0.1, 0.1, 0.1, 0);

  glPushMatrix();
  glLoadIdentity();
  glTranslatef(-1.0, 0.0, -3.5);
  glRotatef(xangle, 1.0, 0.0, 0.0);
  glRotatef(yangle, 0.0, 1.0, 0.0);
  glRotatef(zangle, 0.0, 0.0, 1.0);
  glTranslatef(xt, yt, zt);
  glScalef(xs, ys, zs);

  if (flag2 == 1)
  {
    GLfloat fogcolour[4] = {1.0, 1.0, 1.0, 1.0};

    glFogfv(GL_FOG_COLOR, fogcolour);
    glFogf(GL_FOG_DENSITY, 0.1);
    glFogi(GL_FOG_MODE, GL_EXP);
    glFogf(GL_FOG_START, 3.0);
    glFogf(GL_FOG_END, 100.0);
    glHint(GL_FOG_HINT, GL_FASTEST);
    glEnable(GL_FOG);
  }
  if (flag2 == 0)
  {
    glDisable(GL_FOG);
  }

  if (!aflag)
  {
    glBegin(GL_POINTS);
    glColor3f(1, 1, 1);
    glPointSize(200.0);
    int ccount = 0;
    float x = 10, y = 10;
    while (ccount < 20)
    {
      glVertex2f(x, y);
      x += 10;
      y += 10;
      if (y > Ysize)
        y -= 10;
      if (x > Xsize)
        x -= 10;
      ccount++;
    }
    glEnd();
  }

  //  here we are creating head lights for the car.
  glEnable(GL_COLOR_MATERIAL);
  glColor3f(r1, g1, b1);
  glPointSize(30.0);
  glBegin(GL_TRIANGLE_FAN);
  glNormal3f(1.0f, 0.0f, 0.0f);
  glVertex3f(0.2f, 0.3f, 0.3f);
  for (int i = 0; i <= 360; i += 10)
  {
    GLfloat x = 0.2 + 0.05 * cos(i * 3.14159265f / 180.0f);
    GLfloat y = 0.3 + 0.05 * sin(i * 3.14159265f / 180.0f);
    glNormal3f(0.0f, 0.0f, 1.0f); // Set normal vector for each vertex
    glVertex3f(x, y, 0.3f);
  }
  glEnd();
  glEnable(GL_COLOR_MATERIAL);
  glColor3f(r1, g1, b1);
  glPointSize(30.0);
  glBegin(GL_TRIANGLE_FAN);
  glNormal3f(1.0f, 0.0f, 0.0f);
  glVertex3f(0.2f, 0.3f, 0.5f);
  for (int i = 0; i <= 360; i += 10)
  {
    GLfloat x = 0.2 + 0.05 * cos(i * 3.14159265f / 180.0f);
    GLfloat y = 0.3 + 0.05 * sin(i * 3.14159265f / 180.0f);
    glNormal3f(0.0f, 0.0f, 1.0f); // Set normal vector for each vertex
    glVertex3f(x, y, 0.5f);
  }
  glEnd();

  glPointSize(200.0);

  glBegin(GL_QUADS); /* OBJECT MODULE*/

  //------------------------------------------- FRONT BODY ------------------------------------------
  /* Top of cube*/
  glColor3f(r, g, b);
  glVertex3f(0.2, 0.4, 0.6);
  glVertex3f(0.6, 0.5, 0.6);
  glVertex3f(0.6, 0.5, 0.2);
  glVertex3f(0.2, 0.4, 0.2);

  /* bottom of cube*/
  glVertex3f(0.2, 0.4, 0.6);
  glVertex3f(0.6, 0.2, 0.6);
  glVertex3f(0.6, 0.2, 0.2);
  glVertex3f(0.2, 0.2, 0.2);

  /* front of cube*/
  glVertex3f(0.2, 0.2, 0.6);
  glVertex3f(0.2, 0.4, 0.6);
  glVertex3f(0.2, 0.4, 0.2);
  glVertex3f(0.2, 0.2, 0.2);

  /* back of cube.*/
  glVertex3f(0.6, 0.2, 0.6);
  glVertex3f(0.6, 0.5, 0.6);
  glVertex3f(0.6, 0.5, 0.2);
  glVertex3f(0.6, 0.2, 0.2);

  /* left of cube*/
  glVertex3f(0.2, 0.2, 0.6);
  glVertex3f(0.6, 0.2, 0.6);
  glVertex3f(0.6, 0.5, 0.6);
  glVertex3f(0.2, 0.4, 0.6);

  /* Right of cube */
  glVertex3f(0.2, 0.2, 0.2);
  glVertex3f(0.6, 0.2, 0.2);
  glVertex3f(0.6, 0.5, 0.2);
  glVertex3f(0.2, 0.4, 0.2);
  //---------------------------------------------------------------------------------------
  glVertex3f(0.7, 0.65, 0.6);
  glVertex3f(0.7, 0.65, 0.2);
  glVertex3f(1.7, 0.65, 0.2); // top cover
  glVertex3f(1.7, 0.65, 0.6);
  //--------------------------------------back guard----------------------------------------
  glColor3f(r, g, b);
  glVertex3f(1.8, 0.5, 0.6);
  glVertex3f(1.8, 0.5, 0.2);
  glVertex3f(2.1, 0.4, 0.2);
  glVertex3f(2.1, 0.4, 0.6);

  /* bottom of cube*/
  glVertex3f(2.1, 0.2, 0.6);
  glVertex3f(2.1, 0.2, 0.2);
  glVertex3f(1.8, 0.2, 0.6);
  glVertex3f(1.8, 0.2, 0.6);

  /* back of cube.*/
  glVertex3f(2.1, 0.4, 0.6);
  glVertex3f(2.1, 0.4, 0.2);
  glVertex3f(2.1, 0.2, 0.2);
  glVertex3f(2.1, 0.2, 0.6);

  /* left of cube*/
  glVertex3f(1.8, 0.2, 0.2);
  glVertex3f(1.8, 0.5, 0.2);
  glVertex3f(2.1, 0.4, 0.2);
  glVertex3f(2.1, 0.2, 0.2);

  /* Right of cube */
  glVertex3f(1.8, 0.2, 0.6);
  glVertex3f(1.8, 0.5, 0.6);
  glVertex3f(2.1, 0.4, 0.6);
  glVertex3f(2.1, 0.2, 0.6);

  /* MIDDLE BODY ------------------------------------------------------*/
  glVertex3f(0.6, 0.5, 0.6);
  glVertex3f(0.6, 0.2, 0.6);
  glVertex3f(1.8, 0.2, 0.6);
  glVertex3f(1.8, 0.5, 0.6);

  /* bottom of cube*/
  glVertex3f(0.6, 0.2, 0.6);
  glVertex3f(0.6, 0.2, 0.2);
  glVertex3f(1.8, 0.2, 0.2);
  glVertex3f(1.8, 0.2, 0.6);

  /* back of cube.*/
  glVertex3f(0.6, 0.5, 0.2);
  glVertex3f(0.6, 0.2, 0.2);
  glVertex3f(1.8, 0.2, 0.2);
  glVertex3f(1.8, 0.5, 0.2);

  /*   ENTERING WINDOW ---------------------------------------------*/
  glColor3f(0.3, 0.3, 0.3);
  glVertex3f(0.77, 0.63, 0.2);
  glVertex3f(0.75, 0.5, 0.2); // quad front window
  glVertex3f(1.2, 0.5, 0.2);
  glVertex3f(1.22, 0.63, 0.2);

  glVertex3f(1.27, 0.63, .2);
  glVertex3f(1.25, 0.5, 0.2); // quad back window
  glVertex3f(1.65, 0.5, 0.2);
  glVertex3f(1.67, 0.63, 0.2);

  glColor3f(r, g, b);
  glVertex3f(0.7, 0.65, 0.2);
  glVertex3f(0.7, 0.5, .2); // first separation
  glVertex3f(0.75, 0.5, 0.2);
  glVertex3f(0.77, 0.65, 0.2);

  glVertex3f(1.2, 0.65, 0.2);
  glVertex3f(1.2, 0.5, .2); // second separation
  glVertex3f(1.25, 0.5, 0.2);
  glVertex3f(1.27, 0.65, 0.2);

  glVertex3f(1.65, 0.65, 0.2);
  glVertex3f(1.65, 0.5, .2); // 3d separation
  glVertex3f(1.7, 0.5, 0.2);
  glVertex3f(1.7, 0.65, 0.2);

  glVertex3f(0.75, 0.65, 0.2);
  glVertex3f(0.75, 0.63, 0.2); // line strip
  glVertex3f(1.7, 0.63, 0.2);
  glVertex3f(1.7, 0.65, 0.2);

  glVertex3f(0.75, 0.65, 0.6);
  glVertex3f(0.75, 0.63, 0.6); // line strip
  glVertex3f(1.7, 0.63, 0.6);
  glVertex3f(1.7, 0.65, 0.6);

  glColor3f(0.3, 0.3, 0.3);
  glVertex3f(0.77, 0.63, 0.6);
  glVertex3f(0.75, 0.5, 0.6); // quad front window
  glVertex3f(1.2, 0.5, 0.6);
  glVertex3f(1.22, 0.63, 0.6);

  glVertex3f(1.27, 0.63, .6);
  glVertex3f(1.25, 0.5, 0.6); // quad back window
  glVertex3f(1.65, 0.5, 0.6);
  glVertex3f(1.67, 0.63, 0.6);

  glColor3f(r, g, b);
  glVertex3f(0.7, 0.65, 0.6);
  glVertex3f(0.7, 0.5, .6); // first separation
  glVertex3f(0.75, 0.5, 0.6);
  glVertex3f(0.77, 0.65, 0.6);

  glVertex3f(1.2, 0.65, 0.6);
  glVertex3f(1.2, 0.5, .6); // second separation
  glVertex3f(1.25, 0.5, 0.6);
  glVertex3f(1.27, 0.65, 0.6);

  glVertex3f(1.65, 0.65, 0.6);
  glVertex3f(1.65, 0.5, .6);
  glVertex3f(1.7, 0.5, 0.6);
  glVertex3f(1.7, 0.65, 0.6);
  glEnd();

  //**************************************************************
  glBegin(GL_QUADS);

  /* top of cube*/
  glColor3f(0.3, 0.3, 0.3);
  glVertex3f(0.6, 0.5, 0.6);
  glVertex3f(0.6, 0.5, 0.2); // quad front window
  glVertex3f(0.7, 0.65, 0.2);
  glVertex3f(0.7, 0.65, 0.6);

  glVertex3f(1.7, 0.65, .6);
  glVertex3f(1.7, 0.65, 0.2); // quad back window
  glVertex3f(1.8, 0.5, 0.2);
  glVertex3f(1.8, 0.5, 0.6);

  if (flag1)
  {
    glPushMatrix();
    glTranslatef(xw, 0, 0);
    glColor3f(0, 1, 0);
    glVertex3f(-100, 0.1, -100);
    glVertex3f(-100, 0.1, -1); // creating green environment.
    glVertex3f(100, 0.1, -1);
    glVertex3f(100, 0.1, -100);

    glColor3f(0.7, 0.7, 0.7);
    glVertex3f(-100, 0.1, -1);
    glVertex3f(-100, 0.1, -0.05); // a long road
    glVertex3f(100, 0.1, -0.05);
    glVertex3f(100, 0.1, -1);

    glColor3f(1.0, 0.75, 0.0);
    glVertex3f(-100, 0.1, -0.05); // a median
    glVertex3f(-100, 0.1, 0.05);
    glVertex3f(100, 0.1, 0.05);
    glVertex3f(100, 0.1, -0.05);

    glColor3f(0.7, 0.7, 0.7);
    glVertex3f(-100, 0.1, 0.05);
    glVertex3f(-100, 0.1, 1); // a long road
    glVertex3f(100, 0.1, 1);
    glVertex3f(100, 0.1, 0.05);

    glColor3f(0, 1, 0);
    glVertex3f(-100, 0.1, 1);
    glVertex3f(-100, 0.1, 100); // creating green environment.
    glVertex3f(100, 0.1, 100);
    glVertex3f(100, 0.1, 1);
    glPopMatrix();
  }
  glEnd();

  if (wheelflag)
  {
    glPushMatrix();
    glTranslatef(xw, 0, 0);
    // creating the strips for showing  the wheel movement.
    glColor3f(0.5, .2, 0.3);
    glBegin(GL_QUADS);
    for (i = 0; i < 200; i += 0.2)
    {
      glVertex3f(-100.1 + i, 0, 1);
      glVertex3f(-99.8 + i, 0, 1);
      glVertex3f(-99.9 + i, 0.2, 1);
      glVertex3f(-100 + i, 0.2, 1);
      i += 0.5;
    }
    for (i = 0; i < 200; i += 0.2)
    {
      glVertex3f(-100.1 + i, 0, -1);
      glVertex3f(-99.8 + i, 0, -1);
      glVertex3f(-99.9 + i, 0.2, -1);
      glVertex3f(-100 + i, 0.2, -1);
      i += 0.5;
    }
    glEnd();
    glPopMatrix();
  }

  glBegin(GL_TRIANGLES); /* start drawing the cube.*/

  /* top of cube*/
  glColor3f(0.3, 0.3, 0.3);
  glVertex3f(0.6, 0.5, 0.6);
  glVertex3f(0.7, 0.65, 0.6); // tri front window
  glVertex3f(0.7, 0.5, 0.6);

  glVertex3f(0.6, 0.5, 0.2);
  glVertex3f(0.7, 0.65, 0.2); // tri front window
  glVertex3f(0.7, 0.5, 0.2);

  glVertex3f(1.7, 0.65, 0.2);
  glVertex3f(1.8, 0.5, 0.2); // tri back window
  glVertex3f(1.7, 0.5, 0.2);

  glVertex3f(1.7, 0.65, 0.6);
  glVertex3f(1.8, 0.5, 0.6); // tri back window
  glVertex3f(1.7, 0.5, 0.6);

  glEnd();
  // here we are creating the Ignition system.
  glPushMatrix();
  glColor3f(0.3, 0.3, 0.7);
  glTranslatef(1.65, 0.2, 0.3);
  glRotatef(90.0, 0, 1, 0);
  gluCylinder(t, 0.02, 0.03, .5, 10, 10);
  glPopMatrix();

  //  Here we are creating the wheels using the solidtorus function.
  glColor3f(0.7, 0.7, 0.7);
  glPushMatrix();
  //  these are the line strip functions
  glBegin(GL_LINE_STRIP);
  for (theta = 0; theta < 360; theta = theta + 40)
  {
    glVertex3f(0.6, 0.2, 0.62);
    glVertex3f(0.6 + (0.08 * (cos(((theta + angle) * 3.14) / 180))), 0.2 + (0.08 * (sin(((theta + angle) * 3.14) / 180))), 0.62);
  }
  glEnd();

  glBegin(GL_LINE_STRIP);
  for (theta = 0; theta < 360; theta = theta + 40)
  {
    glVertex3f(0.6, 0.2, 0.18);
    glVertex3f(0.6 + (0.08 * (cos(((theta + angle) * 3.14) / 180))), 0.2 + (0.08 * (sin(((theta + angle) * 3.14) / 180))), 0.18);
  }
  glEnd();

  glBegin(GL_LINE_STRIP);
  for (theta = 0; theta < 360; theta = theta + 40)
  {
    glVertex3f(1.7, 0.2, 0.18);
    glVertex3f(1.7 + (0.08 * (cos(((theta + angle) * 3.14) / 180))), 0.2 + (0.08 * (sin(((theta + angle) * 3.14) / 180))), 0.18);
  }
  glEnd();

  glBegin(GL_LINE_STRIP);
  for (theta = 0; theta < 360; theta = theta + 40)
  {
    glVertex3f(1.7, 0.2, 0.62);
    glVertex3f(1.7 + (0.08 * (cos(((theta + angle) * 3.14) / 180))), 0.2 + (0.08 * (sin(((theta + angle) * 3.14) / 180))), 0.62);
  }
  glEnd();
  glTranslatef(0.6, 0.2, 0.6);
  glColor3f(0, 0, 0);
  glutSolidTorus(0.025, 0.07, 10, 25);

  glTranslatef(0, 0, -0.4);
  glutSolidTorus(0.025, 0.07, 10, 25);

  glTranslatef(1.1, 0, 0);
  glutSolidTorus(0.025, 0.07, 10, 25);

  glTranslatef(0, 0, 0.4);
  glutSolidTorus(0.025, 0.07, 10, 25);
  glPopMatrix();
  glPopMatrix();
  glEnable(GL_DEPTH_TEST);
  glutPostRedisplay();
  glutSwapBuffers();
}
//  these are some of the key board functions given below with which we can rotate and translate the car.
void NormalKey(GLubyte key, GLint x, GLint y)
{
  switch (key)
  {

  case 'x':
    xangle += 5.0;
    glutPostRedisplay();
    break;

  case 'X':
    xangle -= 5.0;
    glutPostRedisplay();
    break;

  case 'y':
    yangle += 5.0;
    glutPostRedisplay();
    break;

  case 'Y':
    yangle -= 5.0;
    glutPostRedisplay();
    break;

  case 'z':
    zangle += 5.0; // rotating about z direction by 5 degrees.
    glutPostRedisplay();
    break;

  case 'Z':
    zangle -= 5.0; // rotating about z direction by -5 degrees.
    glutPostRedisplay();
    break;

  case 'u': /* Move up */
    yt += 0.2;
    glutPostRedisplay();
    break;

  case 'U':
    yt -= 0.2; /* Move down */
    glutPostRedisplay();
    break;

  case 'f': /* Move forward */
    zt += 0.2;
    glutPostRedisplay();
    break;

  case 'F':
    zt -= 0.2; /* Move away */
    glutPostRedisplay();
    break;

  case 'a':
    xt += 0.2;
    glutPostRedisplay();
    break;
  case 'A':
    xt -= 0.2;
    glutPostRedisplay();
    break;

  default:
    break;
  }
}

static void SpecialKeyFunc(int Key, int x, int y)
{
  switch (Key)
  {
  case GLUT_KEY_RIGHT:
    if (!wheelflag)
      xt += 0.2;
    if (wheelflag)
    {
      angle += 5;
      xw += 0.2;
    }
    glutPostRedisplay();
    break;

  case GLUT_KEY_LEFT:
    if (!wheelflag)
      xt -= 0.2;
    if (wheelflag)
    {
      angle += 5;
      xw -= 0.2;
    }
    glutPostRedisplay();
    break;
  }
}

//  this is the mymenu function with which we created the menu
void myMenu(int id)
{
  if (id == 1)
  {
    flag1 = 0;
    wheelflag = 0;
    glutPostRedisplay();
    // Default mode
  }
  if (id == 2)
  {
    flag1 = 1;
    flag2 = 0;
    wheelflag = 0;
    xangle += 5.0;
    glutPostRedisplay();
    //  this is Driving mode where we need to update the variables for rendering the environment and road.
  }
  if (id == 4)
  {
    wheelflag = 1;
    glutPostRedisplay();
    //    Activating the wheel effect by updating wheel flag equal to 1.
  }

  if (id == 3)
  {
    r1 = 1.0f;
    g1 = 0.75f;
    b1 = 0.0f;
    glutPostRedisplay();
    //  this is when head lights are on
  }

  if (id == 5)
  {
    r1 = 1.0f;
    g1 = 1.0f;
    b1 = 1.0f;
    glutPostRedisplay();
    //   this is when head lights are off.
  }
  if (id == 12)
  {
    aflag = 1;
    day = 1;
    glClearColor(1, 1, 1, 1);
    glDisable(GL_FOG);
    glutPostRedisplay();
    // this is the Day mode which is Default mode.
  }

  if (id == 13)
  {
    aflag = 0;
    day = 0;
    flag2 = 2;
    glClearColor(0.1, 0.1, 0.1, 0);
    GLfloat fogcolour[4] = {0.0, 0.0, 0.0, 1.0};

    glFogfv(GL_FOG_COLOR, fogcolour);
    glFogf(GL_FOG_DENSITY, 0.5);
    glFogi(GL_FOG_MODE, GL_EXP);
    glHint(GL_FOG_HINT, GL_FASTEST);
    glEnable(GL_FOG);

    glutPostRedisplay();
    //  This is the Night mode.
  }
}

void colorMenu(int id)
{
  if (id == 6)
  {
    r = g = 0;
    b = 1;
    glutPostRedisplay();
  }
  if (id == 7)
  {
    r = 0.8;
    b = g = 0;
    glutPostRedisplay();
  }
  if (id == 8)
  {
    g = 1;
    r = b = 0;
    glutPostRedisplay();
  }
  if (id == 9)
  {
    r = b = g = 0;
    glutPostRedisplay();
  }
  if (id == 10)
  {
    b = 0;
    r = g = 1;
    glutPostRedisplay();
  }
  if (id == 11)
  {
    b = r = g = .7;
    glutPostRedisplay();
  }
}

int main(int argc, char **argv)
{

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
  glutInitWindowSize(Xsize, Ysize);
  glutInitWindowPosition(0, 0);
  glutCreateWindow("3D CAR ANIMATION");
  glutDisplayFunc(DrawGLScene);
  InitGL(Xsize, Ysize);
  int submenu = glutCreateMenu(colorMenu);
  glutAddMenuEntry("blue", 6);
  glutAddMenuEntry("red", 7);
  glutAddMenuEntry("green", 8);
  glutAddMenuEntry("black", 9);
  glutAddMenuEntry("yellow", 10);
  glutAddMenuEntry("grey", 11);
  glutCreateMenu(myMenu);
  glutAddMenuEntry("car model mode", 1);
  glutAddMenuEntry("car driving mode", 2);
  glutAddMenuEntry("wheel effect", 4);
  glutAddMenuEntry("Lighting on", 3);
  glutAddMenuEntry("Lighting off", 5);
  glutAddSubMenu("car colors", submenu);
  glutAddMenuEntry("Day mode", 12);
  glutAddMenuEntry("Night mode", 13);
  glutAttachMenu(GLUT_RIGHT_BUTTON);
  glutKeyboardFunc(NormalKey);
  glutSpecialFunc(SpecialKeyFunc);
  glutMainLoop();
  return 1;
}


/*referencees: https://community.khronos.org/t/model-car-3d/76847*/
