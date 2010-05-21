/* nurbpractice.c
 *
 * wrapper for Jan C. Hardenbergh's nurbs practice piece
 */

#include <GL/glut.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>

#define kStride 4

/* ================================================================*/
/* contents of nurbrender.c, copied over some time after edition 2 */

GLUnurbsObj *pNurb = NULL;
GLUnurbsObj *pNurb2 = NULL;
#define nCtrlPoints  18    
GLfloat ctrlPoints[nCtrlPoints][4];
GLfloat pointsWeights[nCtrlPoints][4]  = {    

    {.5, 0., 0, 1},
    {.5, .3, 0, 1},
    {.3, .5, 0, 1},

    {0., .5, 0, 1},
    {-.1, .5, 0, 1},

    {-0.3, 0.3, 0., 1},
    {-0.1, 0.3, 0, 1},
    {0.1, 0.25, 0, 1},
    {0., 0., 0, 1},
    {0., -0.1, 0, 1},
    {0., -0.2, 0, 1},
    {0.,-0.3, 0, 1},
    {-0.3, -0.3, 0., 1},

    {-.3,-.5, 0, 1},

      {0, -.5, 0, 1},
    {.3,-.5, 0, 1},
    {.5,-.3, 0, 1},

    {.5, 0., 0, 1}};
  
    // Knot vector
    int order = 5;
    int nKnots = 23;
GLfloat Knots[23] = {0, 0, 0, 0, 0,  1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 14, 14, 14, 14 };

/* ========================================== */

#define NPOINTS2 13
GLfloat pointsWeights2[NPOINTS2][4] = {
  {0.2, 0.4, 0., 1.},
  {0., 0.5, 0., 1.},

  {-0.2, 0.4, 0., 1.},
  {0.05, 0.2, 0., 1.},
  {0.2, -0.1, 0., 1.},
  
  {0.0, -0.2, 0., 1.},
  {-0.2, -0.5, 0., 1.},


  {0., -0.5, 0., 1.},
  {0.2, -0.5, 0., 1.},
  {0.4, -0.3, 0., 1.},
  {0.5, -0.1, 0., 1.},


  {0.5, 0.2, 0., 1.},
  {0.2, 0.4, 0., 1.}};


int order2 = 5;
int nKnots2 = 18;
GLfloat Knots2[18] = {0, 0, 0, 0, 0,  1, 2, 3, 4, 5, 6, 7, 8, 9, 9, 9, 9, 9};
    
/* =========================================== */
    

    // Called to draw scene
    void RenderScene(void)
    {
        int i, j;
        
        // Clear the window with current clearing color
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
/* 	glLoadIdentity(); */
        
        // Render the NURB
        
        // make sure we are setup
        if (!pNurb)
        {
            // Setup the Nurbs object
            pNurb = gluNewNurbsRenderer();
            gluNurbsProperty(pNurb, GLU_SAMPLING_TOLERANCE, 5.0f);
            //gluNurbsProperty(pNurb, GLU_DISPLAY_MODE, GLU_OUTLINE_POLYGON);
            //gluNurbsProperty(pNurb, GLU_DISPLAY_MODE, (GLfloat)GLU_FILL);
            
            // make the NURBS control points from the points & weights
            for(i = 0; i < nCtrlPoints; i++)
            {
                ctrlPoints[i][3] = pointsWeights[i][3];
                for (j = 0; j < 3; j++)
                {
                    ctrlPoints[i][j] = pointsWeights[i][j]*ctrlPoints[i][3];
                }
/* 		printf("Control point %d: %f, %f, %f, %f\n", i, ctrlPoints[i][0], ctrlPoints[i][1], ctrlPoints[i][2], ctrlPoints[i][3]); */
            }
        }

	// make shegeek's nurb
	if (!pNurb2)
	  {
	    pNurb2 = gluNewNurbsRenderer();
	    gluNurbsProperty(pNurb, GLU_SAMPLING_TOLERANCE, 5.0f);
	    //weights are all 1
	  }

        
        glColor3f(0.,0.,0.);
        
        // Begin the NURB definition
        gluBeginCurve(pNurb);
        
        // Send the Non Uniform Rational BSpline
/*         gluNurbsCurve(pNurb, nKnots, Knots, kStride,  */
/*             &ctrlPoints[0][0], order, GL_MAP1_VERTEX_4); */
        
        gluEndCurve(pNurb);

	// make a second curve with first nurb
/* 	glColor3f(1., 0., 0.); */
	glPushMatrix();
/* 	glTranslatef(-0.2, 0.2, 0.); */
	glScalef(1.2, 1.2, 1.);

/*         gluNurbsCurve(pNurb, nKnots, Knots, kStride,  */
/*             &ctrlPoints[0][0], order, GL_MAP1_VERTEX_4); */
        
        gluEndCurve(pNurb);
	glPopMatrix();

	// draw shegeek's nurb
/* 	glColor3f(0., 1., 1.); */
	glPushMatrix();
	glScalef(1.3, 1.3, 1.);
	gluBeginCurve(pNurb2);
	gluNurbsCurve(pNurb2, nKnots2, Knots2, kStride,
		      &pointsWeights2[0][0], order2, GL_MAP1_VERTEX_4);
	gluEndCurve(pNurb2);

        
        // Draw the control points in red
        glPointSize(3.0f);
        glColor3f(1.,0.,0.);
        glBegin(GL_POINTS);
        for(i = 0; i < NPOINTS2; i++)
            glVertex3fv(pointsWeights2[i]);  
        glEnd();

        glPointSize(5.0f);
        glColor3f(0.,0.,1.);
        glBegin(GL_POINTS);
        for(i = 1; i < NPOINTS2; i += 2)
            glVertex3fv(pointsWeights2[i]);  
        glEnd();

	glColor3f(0., 0., 0.);
	glBegin(GL_POINTS);
	glVertex3fv(pointsWeights2[0]);
	glEnd();
	glPopMatrix();
        
        // Flush drawing commands
        glFlush();
    }
 /*================================================================*/






void init(void)
{
  glClearColor(1.0, 1.0, 1.0, 0.0);
  glShadeModel(GL_SMOOTH);
  glLineWidth(5.0);
  glTranslatef(0., 0., -1.5);
}


void display(void)
{
  RenderScene();
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
