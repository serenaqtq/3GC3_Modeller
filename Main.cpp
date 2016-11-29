#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <list>
#include <ctime>
using namespace std;

#ifdef __APPLE__
#  include <OpenGL/gl.h>
#  include <OpenGL/glu.h>
#  include <GLUT/glut.h>
#else
#  include <GL/gl.h>
#  include <GL/glu.h>
#  include <GL/freeglut.h>
#endif

#define WINDOW_SIZE_WIDTH 600
#define WINDOW_SIZE_HEIGHT 600
#define TEAPOT_SPEED 0.01
#define BOUND_OFFSET 0.55
float angle = 2;
int material = 1;
float camPos[] = {20, 20, 20};	//where the camera is
int mouseX = 0, mouseY = 0; 
float light_pos1[] = {8,8, 0,1};//light 1
float light_pos2[] = {0,8,8,1};//light 2
list<struct Object> track;

struct Object{
	float posX, posY, posZ;
	float rotateX, rotateY, rotateZ;
	float scaleX, scaleY, scaleZ;
	int shape;
	bool intersect;
};
typedef struct materialStruct {
	float m_amb[4];
	float m_dif[4]; 
	float m_spec[4]; 
	float shiny; 
} materialStruct;

materialStruct m1 = {
	{0.3, 0.0, 0.0, 1.0},
	{0.6, 0.0, 0.0, 1.0},
	{0.8, 0.6, 0.6, 1.0},
	32.0
};

materialStruct m2 = {
	{0.83, 0.52, 0.63, 1.0},
    {0.28, 0.87, 0.11, 1.0},
    {0.99, 0.91, 0.81, 1.0},
    27
};

materialStruct m3 = {
	{0.5, 0.0, 0.8, 1.0},
    {0.5, 0.8, 0.0, 1.0},
    {0.8, 0.20, 0.5, 1.0},
    38
};

materialStruct m4 = {
	{0.42, 0.24, 0.0, 1.0},
    {0.7, 0.22, 0.35, 1.0},
    {0.99, 0.1, 0.24, 1.0},
    42
};

materialStruct m5 = {
	{0.12, 0.88, 0.42, 1.0},
    {0.88, 0.12, 0.0, 1.0},
    {0.18, 0.24, 0.68, 1.0},
    50
};

void setMaterial(int num) {
	switch (num) {
		case 1:
			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, m1.m_amb);
    		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, m1.m_dif);
    		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, m1.m_spec);
    		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, m1.shiny);
    		glColorMaterial(GL_AMBIENT, GL_DIFFUSE);
    		break;
		case 2:
			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, m2.m_amb);
    		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, m2.m_dif);
    		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, m2.m_spec);
    		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, m2.shiny);
    		glColorMaterial(GL_AMBIENT, GL_DIFFUSE);
    		break;
		case 3:
			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, m3.m_amb);
    		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, m3.m_dif);
    		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, m3.m_spec);
    		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, m3.shiny);
    		glColorMaterial(GL_AMBIENT, GL_DIFFUSE);
    		break;
		case 4:
			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, m4.m_amb);
    		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, m4.m_dif);
    		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, m4.m_spec);
    		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, m4.shiny);
    		glColorMaterial(GL_AMBIENT, GL_DIFFUSE);
    		break;
		case 5:
			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, m5.m_amb);
    		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, m5.m_dif);
    		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, m5.m_spec);
    		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, m5.shiny);
    		glColorMaterial(GL_AMBIENT, GL_DIFFUSE);
    		break;
	}
}
void eraseSelected() {
	list<struct Object>::iterator p = track.begin();
	while(p != track.end()){
		if ((*p).intersect){
			printf("%i\n", (*p).intersect);
			track.erase(p);
			//return p;
		}
		p++;
	}
	//return p;
}

void CalcIntersections(struct Object &teapot){
	//---Construct ray-----------------------------------------------------
	//construct Ray
	GLdouble R0[3], R1[3], Rd[3];
	GLdouble modelMat[16], projMat[16];
	GLint viewMat[4];

	//populate mpv matricies
	glGetDoublev(GL_MODELVIEW_MATRIX, modelMat);
	glGetDoublev(GL_PROJECTION_MATRIX, projMat);
	glGetIntegerv(GL_VIEWPORT, viewMat);

	//calculate near point
	gluUnProject(mouseX, mouseY, 0.0, modelMat, projMat, viewMat, &R0[0], &R0[1], &R0[2]);
	//calculate far point
	gluUnProject(mouseX, mouseY, 1.0, modelMat, projMat, viewMat, &R1[0], &R1[1], &R1[2]);

	//calcualte our ray from R0 and R1
	Rd[0] = R1[0] - R0[0];
	Rd[1] = R1[1] - R0[1];
	Rd[2] = R1[2] - R0[2];

	//turn ray Rd into unit ray 
	GLdouble m = sqrt(Rd[0]*Rd[0] + Rd[1]*Rd[1] + Rd[2]*Rd[2]);
	Rd[0] /= m;
	Rd[1] /= m;
	Rd[2] /= m;

	//printf("R0: %f, %f, %f | ", R0[0], R0[1], R0[2]);
	//printf("R1: %f, %f, %f | ", R1[0], R1[1], R1[2]);
	//printf("Rd: %f, %f, %f | ", Rd[0], Rd[1], Rd[2]);

	//---calculate intersection point now-----------------------------------
	//approx the teapot with a box of radius 1 centered around the teapot centered
	//goes against the xy plane to test the Intersection
	//NOTE: this is not the code from slides, but rather proof of concept
	//using assumtions which are true for this example only. 

	//calculate t value from z dir;
	double t = (((double)teapot.posZ) - R0[2])/Rd[2];

	//printf("t: %f | ", t);

	//use t value to find x and y of our intersection point
	double pt[3];
	pt[0] = R0[0] + t * Rd[0];
	pt[1] = R0[1] + t * Rd[1];
	pt[2] = teapot.posZ;
	
	//printf("pt: %f, %f, %f | ", pt[0], pt[1], pt[2]);

	//now that we have our point on the xy plane at the level of the teapot,
	//use it to see if this point is inside a box centered at the teapots
	//location
	if(pt[0] > teapot.posX - BOUND_OFFSET && pt[0] < teapot.posX + BOUND_OFFSET &&
		pt[1] > teapot.posY - BOUND_OFFSET && pt[1] < teapot.posY + BOUND_OFFSET &&
		pt[2] > teapot.posZ - BOUND_OFFSET && pt[2] < teapot.posZ + BOUND_OFFSET)
		teapot.intersect = true;
	else
		teapot.intersect = false;

	//printf("\n");
}
struct Object createObject(int x, int y, int z, int rX, int rY, int rZ, int sX, int sY, int sZ) {
	struct Object result;
	result.posX = x;
	result.posY = y;
	result.posZ = z;
	result.rotateX = rX;
	result.rotateY = rY;
	result.rotateZ = rZ;
	result.scaleX = sX;
	result.scaleY = sY;
	result.scaleZ = sZ;
	result.intersect = true;
	result.shape = rand() % 5;
	return result;
}
//keyboard for exiting when q or escape is pressed
void keyboard(unsigned char key, int xIn, int yIn)
{
	//int mod = glutGetModifiers();
	switch (key)
	{
		case 'q':
		case 27:	//27 is the esc key
			exit(0);
			break;
		case 'z':
			list<struct Object>::iterator p = track.begin();
			while(p != track.end()){
				(*p).intersect = false;
				p++;
			}
			printf("%s\n", "Add a shape to the scene");
			
			track.push_back(createObject(rand()%15, rand()%15, rand()%15, rand()%10, rand()%10, rand()%10, rand()%3+1, rand()%3+1, rand()%3+1));
			break;
	}
	glutPostRedisplay();
}

void mouse(int btn, int state, int x, int y){

	mouseX = x;
	mouseY = WINDOW_SIZE_HEIGHT - y;
	list<struct Object>::iterator p = track.begin();
	while(p != track.end()){
		CalcIntersections(*p);
	}
	if(btn == GLUT_RIGHT_BUTTON) {
		if (state == GLUT_DOWN){
			
			if (!track.empty()) {
				eraseSelected();
			} else {
				printf("%s\n", "No object in the scene");
			}

		
		}
		
	}
	
	glutPostRedisplay();
	
}

void drawAxis()
{
	glBegin(GL_LINES);
		glColor3f(1, 0, 0);
		glVertex3f(0,0,0);
		glVertex3f(50,0,0);
		glColor3f(0,1,0);
		glVertex3f(0,0,0);
		glVertex3f(0,50,0);
		glColor3f(0,0,1);
		glVertex3f(0,0,0);
		glVertex3f(0,0,50);
	glEnd();
}

void drawPlane() {
	glBegin(GL_QUADS);
		glColor3f(1,0.43,0.78);
		glNormal3f(0,1,0);		
		glNormal3f(0,1,0);
		glVertex3f(0,0,0);
		glNormal3f(0,1,0);
		glVertex3f(0,0,50);
		glNormal3f(0,1,0);
		glVertex3f(50,0,50);
		glNormal3f(0,1,0);
		glVertex3f(50,0,0);
	glEnd();
}

void init(void)
{
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

	glutInitWindowSize(WINDOW_SIZE_WIDTH, WINDOW_SIZE_HEIGHT);
	glutInitWindowPosition(50, 50);

	glutCreateWindow("Modeller");	//creates the window
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);

    //light
	glLightfv(GL_LIGHT0, GL_POSITION, light_pos1);
    glLightfv(GL_LIGHT1, GL_POSITION, light_pos2);
	
	glClearColor(0,0,0,0);
	glColor3f(1,1,1);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	gluPerspective(45, 1, 1, 100);

	//culling
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);
}


void draw(struct Object in) {
	switch (in.shape){
		case 0:
			glutSolidCube(1);
			break;
		case 1:
			glutSolidTeapot(1);
			break;
		case 2:
			glutSolidSphere(1,20,20);
			break;
		case 3:
			glutSolidCone(1,1,20,20);
			break;
		case 4:
			glutSolidTetrahedron();
			break;
	}
}


//display function, also calls the position update and ray calc
void display(void)
{
	//CalculatePosition();
	
	
	//glClearColor(0,0,0,0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    
    //enable material
    setMaterial(material);
    
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(camPos[0], camPos[1], camPos[2], 0, 0, 0, 0,1,0);

	drawAxis();
	drawPlane();
	list<struct Object>::iterator p = track.begin();
	while(p != track.end()){
		/*if (p != track.end()--) {
			
		}*/
		//CalcIntersections(*p);
		glPushMatrix();
			glTranslatef((*p).posX, (*p).posY, (*p).posZ);
			glRotatef(angle,(*p).rotateX, (*p).rotateY, (*p).rotateZ);
			glScalef((*p).scaleX, (*p).scaleY, (*p).scaleZ);

			if((*p).intersect)
				glColor3f(1,0,0); //red
			else
				glColor3f(1, 1, 0); // yellow

				draw(*p);
		glPopMatrix();
		p++;
	}
	printf("%i\n", track.size());
	//switch our buffers for a smooth animation
	glutSwapBuffers();
}


void callBackInit(){
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	//glutMotionFunc(motion);
	//glutPassiveMotionFunc(passive);
	//glutReshapeFunc(reshape);
	//glutTimerFunc(0, FPS, 0);
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);		//starts up GLUT
	srand(time(NULL));
	init();
	callBackInit();
	

	glutMainLoop();				//starts the event glutMainLoop
	return(0);					//return may not be necessary on all compilers
}