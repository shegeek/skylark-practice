/* boomsketch.c
 *
 * testing boomerang shapes for the skylark screenhack
 */

#include <GL/glut.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include "boomerangs.h"

static int flyingboom = 0;
GLUnurbsObj *pNurb2 = NULL;

void nurbsError(GLenum errorcode)
{
  const GLubyte *errorstring;
  errorstring = gluErrorString(errorcode);
  fprintf (stderr, "Nurbs error: %s\n", errorstring);
  exit(0);
}


/* =========================================== */
    
    void display(void)
    {
      int i;
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
/* 	glLoadIdentity(); */
        
        glColor3f(0.,0.,0.);

	glPushMatrix();
	glScalef(1.3, 1.3, 1.);
	gluBeginCurve(pNurb2);
	gluNurbsCurve(pNurb2, nKnots2, Knots2, kStride,
		      &pointsWeights2[flyingboom][0][0], 
		      order2, GL_MAP1_VERTEX_4);
	gluEndCurve(pNurb2);

        
        // Draw the control points
        glPointSize(3.0f);
        glColor3f(1.,0.,0.);
        glBegin(GL_POINTS);
        for(i = 0; i < NPOINTS2; i++)
            glVertex3fv(pointsWeights2[flyingboom][i]);  
        glEnd();

        glPointSize(5.0f);
        glColor3f(0.,0.,1.);
        glBegin(GL_POINTS);
        for(i = 1; i < NPOINTS2; i += 2)
            glVertex3fv(pointsWeights2[flyingboom][i]);  
        glEnd();

	glColor3f(0., 1., 0.);
	glBegin(GL_POINTS);
	glVertex3fv(pointsWeights2[flyingboom][0]);
	if (flyingboom == NUMBOOMS - 1) glVertex3f(0., 0., 0.);
	glEnd();
	glPopMatrix();
        
        glFlush();
    }
 /*================================================================*/

void init(void)
{
  if (!pNurb2)
  {
    pNurb2 = gluNewNurbsRenderer();
    gluNurbsProperty(pNurb2, GLU_SAMPLING_TOLERANCE, 5.0f);
    gluNurbsProperty(pNurb2, GLU_DISPLAY_MODE, GLU_OUTLINE_POLYGON);
/*     gluNurbsProperty(pNurb2, GLU_DISPLAY_MODE, GLU_FILL); */
    gluNurbsCallback(pNurb2, GLU_ERROR, (GLvoid (*) ()) nurbsError);
  }
  glClearColor(1.0, 1.0, 1.0, 0.0);
  glShadeModel(GL_SMOOTH);
  glLineWidth(7.0);
  glTranslatef(0., 0., -1.5);
}


void reshape (int w, int h)
{
  GLfloat aspect = (GLfloat)h / (GLfloat)w;
   glViewport (0, 0, (GLsizei) w, (GLsizei) h); 
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ();
   glOrtho (-1.0, 1.0, -aspect, aspect, 0.5, 2.0);
/*    glOrtho (-1.0, 1.0, -1.0, 1.0, 0.5, 2.0); */
/*    glFrustum (-1.0, 1.0, -1.0, 1.0, 1.5, 20.0); */
   glMatrixMode (GL_MODELVIEW);
}



void keyboard(unsigned char key, int x, int y)
{
   switch (key) {
   case 'n':
     flyingboom = ++flyingboom % NUMBOOMS;
     glutPostRedisplay();
     break;
   case 'p':
     --flyingboom;
     if (flyingboom < 0) flyingboom += NUMBOOMS;
     glutPostRedisplay();
     break;
   case 'l':
     glRotatef(5.0, 0., 0., 1.);
     glutPostRedisplay();
     break;
   case 'r':
     glRotatef(-5.0, 0., 0., 1.);
     glutPostRedisplay();
     break;
   case 'f':
     glScalef(-1.0, 1., 1.);
     glutPostRedisplay();
     break;
       case 27:
         exit(0);
         break;
   default:
     break;
   }
}



int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
   glutInitWindowSize (700, 700); 
   glutInitWindowPosition (100, 100);
   glutCreateWindow (argv[0]);
   init ();
   glutDisplayFunc(display); 
   glutReshapeFunc(reshape);
   glutKeyboardFunc(keyboard);
   glutMainLoop();
   return 0;
}
