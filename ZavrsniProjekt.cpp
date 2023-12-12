#include <GL/gl.h>
#include <GL/glut.h>
#include "Mjesec.c"
#include "earth.c"
#include "sun.c"

float X = 4, Z = 4, camera = 0.1;
bool enableTexture = false;
bool enableLight = false;
float durationDay = 0.024;
float hour = durationDay / 24;
float day = hour * 24, EarthAroundSun, MoonAroundEarth, EarthAroundAxis;

void setSunMaterial() {
  GLfloat mat_diffuse[] = {0.8, 0.8, 0.8, 1.0};
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
  GLfloat light_position[] = {1.0, 0, 0, 0.0};
  glLightfv(GL_LIGHT0, GL_POSITION, light_position);
  GLfloat lmodel_ambient[] = {0.1, 0.1, 0.1, 1.0};
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);

  if (!enableLight) {
    GLfloat mat_ambient[] = {0.6, 0.6, 0.6, 1.0};
    GLfloat no_light[] = {0, 0, 0, 0};
    GLfloat white_light[] = {1.0, 1.0, 1.0, 1.0};

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient);
    glLightfv(GL_LIGHT0, GL_SPECULAR, no_light);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, white_light);

  } else {
    GLfloat mat_specular[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat mat_shininess[] = {100.0};
    GLfloat gray_light[] = {0.6, 0.6, 0.6, 1};
    GLfloat white_light[] = {1.0, 1.0, 1.0, 1.0};

    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, gray_light);
    glLightfv(GL_LIGHT0, GL_SPECULAR, white_light);
  }
}

void setEarthMaterial() {
  if (!enableLight) {
    glColor3f(0.0694, 0.0520, 0.400);
    glDisable(GL_LIGHTING);
    glDisable(GL_LIGHT0);
  }

  else {
    GLfloat ambient[] = {0, 1, 0, 1.0};
    GLfloat diffuse[] = {0, 0, 0.8, 1.0};
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
  }
}

void setMoonMaterial() {
  if (!enableLight) {
    glColor3f(0.410, 0.402, 0.402);
  } else {
    GLfloat ambient[] = {0.9, 0.9, 0.9, 1.0};
    GLfloat diffuse[] = {0.4, 0.4, 0.4, 1.0};
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
  }
}

void setSunTexture() {
  static GLuint texture = 0;

  if (enableTexture) {
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, gimp_image.width, gimp_image.height,
                 0, GL_RGB, GL_UNSIGNED_BYTE, gimp_image.pixel_data);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
    glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
  } else {
    glDeleteTextures(1, &texture);
    glDisable(GL_TEXTURE_GEN_S);
    glDisable(GL_TEXTURE_GEN_T);
    glDisable(GL_TEXTURE_2D);

    setSunMaterial();
  }
}

void setEarthTexture() {
  static GLuint texture = 0;

  if (enableTexture) {
    glColor3f(1, 1, 1);
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, gimp_image_earth.width,
                 gimp_image_earth.height, 0, GL_RGB, GL_UNSIGNED_BYTE,
                 gimp_image_earth.pixel_data);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
    glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
  }

  else {
    glDeleteTextures(1, &texture);
    glDisable(GL_TEXTURE_GEN_S);
    glDisable(GL_TEXTURE_GEN_T);
    glDisable(GL_TEXTURE_2D);

    setEarthMaterial();
  }
}

void setMoonTexture() {
  static GLuint texture = 0;

  if (enableTexture) {
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, gimp_image_moon.width,
                 gimp_image_moon.height, 0, GL_RGB, GL_UNSIGNED_BYTE,
                 gimp_image_moon.pixel_data);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
    glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
  }

  else {
    glDeleteTextures(1, &texture);
    glDisable(GL_TEXTURE_GEN_S);
    glDisable(GL_TEXTURE_GEN_T);
    glDisable(GL_TEXTURE_2D);

    setMoonMaterial();
  }
}

void drawSun() {
  glPushMatrix();
  setSunTexture();
  setSunMaterial();
  glutSolidSphere(0.8, 200, 200);
  glPopMatrix();
}

void drawEarth() {
  glPushMatrix();
  glRotatef(EarthAroundSun, 0, 1, 0);
  glTranslatef(0, 0, -2.2);
  glRotatef(EarthAroundAxis, 0, 1, 0);
  setEarthMaterial();
  setEarthTexture();
  glutSolidSphere(0.4, 200, 200);
}

void drawMoon() {
  setMoonTexture();
  glRotatef(MoonAroundEarth, 0, 1, 0);
  glTranslatef(0, 0, -0.8);
  setMoonMaterial();
  glutSolidSphere(0.2, 200, 200);
  glPopMatrix();
}

void display(void) {
  glMatrixMode(GL_MODELVIEW);
  glClearColor(0, 0, 0, 0.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();
  gluLookAt(X, 0, Z, 0, 0, 0, 0, 1, 0);
  glShadeModel(GL_SMOOTH);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_TEXTURE_GEN_S);
  glEnable(GL_TEXTURE_GEN_T);
  glEnable(GL_TEXTURE_2D);
 
  drawSun();
  drawEarth();
  drawMoon();

  glFlush();
  glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y) {
  if (key == 27) exit(0);
  if (key == 'l') enableLight = !enableLight;

  if (key == 't') enableTexture = !enableTexture;

  glutPostRedisplay();
}
void specialKeys(int key, int x, int y) {
  if (key == GLUT_KEY_UP) {
    Z += camera;

  } else if (key == GLUT_KEY_DOWN) {
    Z -= camera;
  }

  else if (key == GLUT_KEY_RIGHT) {
    X += camera;
  }

  else if (key == GLUT_KEY_LEFT) {
    X -= camera;
  }
  glutPostRedisplay();
}

void reshape(int w, int h) {
  glViewport(0, 0, w, h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45, (GLfloat)w / h, 0.2, 10);
}

void idle() {
  hour += (durationDay / 24);
  day += durationDay;
  EarthAroundSun = 360 * day / 365;
  MoonAroundEarth = 360 * day * 12 / 365;
  EarthAroundAxis = 360 * hour / 24;

  glutPostRedisplay();
}

void mouse(int button, int state, int x, int y) {
  if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
    durationDay *= 2;
  }
  if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
    durationDay /= 2;
  }

  glutPostRedisplay();
}

int main(int argc, char **argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
  glutInitWindowSize(1200, 700);
  glutInitWindowPosition(400, 100);
  glutCreateWindow("Svemir");
  glutKeyboardFunc(keyboard);
  glutMouseFunc(mouse);
  glutDisplayFunc(display);
  glutIdleFunc(idle);
  glutReshapeFunc(reshape);
  glutSpecialFunc(specialKeys);
  glutMainLoop();
  return 0;
}
