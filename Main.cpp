<<<<<<< HEAD
=======
//
//1207766 Yanting

>>>>>>> 00e875470a66d10cc9f9316d80ca7f9f7ad79d01
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <list>
#include <ctime>
#include <iostream>
using namespace std;
#include <string.h>
// for file saving loading
//#include <string>
#include <fstream>
ofstream myfile;
ifstream yourfile;
string line;

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
float angle = 20;
int material = 1;
float camPos[] = {20, 20, 20};	//where the camera is
int mouseX = 0, mouseY = 0; 
float light_pos1[] = {800,800, 0,1};//light 1
float light_pos2[] = {0,800,800,1};//light 2
list<struct Object> track;//object list

struct Object{
	float posX, posY, posZ = 0;
	float rotateX, rotateY, rotateZ =0;
	float scaleX, scaleY, scaleZ =0;
	int shape =0;
	int m =0;//material
	float x_offset, y_offset, z_offset =0;//six plane, each float represent two plane
	bool intersect = 0;
};

typedef struct materialStruct {//define 5 different material
	float m_amb[4];
	float m_dif[4]; 
	float m_spec[4]; 
	float shiny; 
} materialStruct;

materialStruct m1 = {//jade
	{0.135,0.2225,0.1575},
	{0.54,0.89,0.63},
	{0.316228,0.316228,0.316228},
	0.1
};

materialStruct m2 = {//brass
	{0.329412,0.223529,0.027451},
    {0.780392,0.568627,0.113725},
    {0.992157,0.941176,0.807843},
    0.21794872
};

materialStruct m3 = {//gold
	{0.24725,0.1995,0.0745},
    {0.75164,0.60648,0.22648},
    {0.628281,0.555802,0.366065},
    0.4
};

materialStruct m4 = {//pearl
	{0.25,0.20725,0.20725},
    {1,0.829,0.829},
    {0.296648,0.296648,0.296648},
    0.088
};

materialStruct m5 = {//turquoise
	{0.1,0.18725,0.1745},
    {0.396,0.74151,0.69102},
    {0.297254,0.30829,0.306678},
    0.1
};

<<<<<<< HEAD
void setMaterial(int num) {//set five material
=======
/* TEXTURE */
GLubyte* image;
GLubyte* image1;
GLubyte* image2;

int width, height, max1;
GLuint myTex[3];

GLubyte* LoadPPM(char* file, int* width, int* height, int* max1)
{
    GLubyte* img;
    FILE *fd;
    int n, m;
    int  k, nm;
    char c;
    int i;
    char b[100];
    float s;
    int red, green, blue;
    
    /* first open file and check if it's an ASCII PPM (indicated by P3 at the start) */
    fd = fopen(file, "r");
    fscanf(fd,"%[^\n] ",b);
    if(b[0]!='P'|| b[1] != '3')
    {
        printf("%s is not a PPM file!\n",file);
        exit(0);
    }
    printf("%s is a PPM file\n", file);
    fscanf(fd, "%c",&c);
    
    /* next, skip past the comments - any line starting with #*/
    while(c == '#')
    {
        fscanf(fd, "%[^\n] ", b);
        printf("%s\n",b);
        fscanf(fd, "%c",&c);
    }
    ungetc(c,fd);
    
    /* now get the dimensions and max colour value from the image */
    fscanf(fd, "%d %d %d", &n, &m, &k);
    
    printf("%d rows  %d columns  max value= %d\n",n,m,k);
    
    /* calculate number of pixels and allocate storage for this */
    nm = n*m;
    img = (GLubyte*)malloc(3*sizeof(GLuint)*nm);
    s=255.0/k;
    
    /* for every pixel, grab the read green and blue values, storing them in the image data array */
    for(i=0;i<nm;i++)
    {
        fscanf(fd,"%d %d %d",&red, &green, &blue );
        img[3*nm-3*i-3]=red*s;
        img[3*nm-3*i-2]=green*s;
        img[3*nm-3*i-1]=blue*s;
    }
    
    /* finally, set the "return parameters" (width, height, max) and return the image array */
    *width = n;
    *height = m;
    *max1 = k;
    
    return img;
}


void setTexture(int num) {
	switch (num) {
		case 6:
		    /* Set the image parameters*/
		    glBindTexture(GL_TEXTURE_2D, myTex[0]);
		    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		     /*Get and save image*/
		    image = LoadPPM("image.ppm",&width, &height, &max1);
		    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
		                 GL_UNSIGNED_BYTE, image);

		    //glBindTexture(GL_TEXTURE_2D, myTex[0]);
		    break;
		case 7:
			/* Set the image parameters*/
		    glBindTexture(GL_TEXTURE_2D, myTex[1]);
		    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);    
		    /*Get and save image*/
		    image1 = LoadPPM("image1.ppm",&width, &height, &max1);
		    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
		                 GL_UNSIGNED_BYTE, image1);

		    //glBindTexture(GL_TEXTURE_2D, myTex[1]);
	    	break;

		case 8:
			/* Set the image parameters*/
		    glBindTexture(GL_TEXTURE_2D, myTex[2]);
		    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		    
		    
		    /*Get and save image*/
		    image2 = LoadPPM("image2.ppm",&width, &height, &max1);
		    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
		                 GL_UNSIGNED_BYTE, image2);

		    //glBindTexture(GL_TEXTURE_2D, myTex[2]);
		    break;
	}
}

void setMaterial(int num) {
>>>>>>> 00e875470a66d10cc9f9316d80ca7f9f7ad79d01
	switch (num) {
		case 1:
			//printf("%s\n", "m1");
			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, m1.m_amb);
    		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, m1.m_dif);
    		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, m1.m_spec);
    		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, m1.shiny);
    		glColorMaterial(GL_AMBIENT, GL_DIFFUSE);
    		break;
		case 2:
		//printf("%s\n", "m2");
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

void load(){//file loader
	track.clear();
	struct Object obj;
	printf("%s\n", "Enter file name with .txt");
	char str[80];
	scanf("%79s", str);
	yourfile.open(str);
	float temp;

	// clear everything on current scene

	//loading scene graph

	if (yourfile.is_open()){
	 	while (getline(yourfile,line)){
			if(line==""){
				break;
	 		}
	 		temp = line.find(",");
	 		  obj.posX=stof(line.substr(0,temp));
	    	line=line.substr(temp+1);

	    	temp = line.find(",");
	 		obj.posY=stof(line.substr(0,temp));
	    	line=line.substr(temp+1);
	    	
	    	temp = line.find(",");
	 		obj.posZ=stof(line.substr(0,temp));
	    	line=line.substr(temp+1);

	    	temp = line.find(",");
	 		obj.rotateX=stof(line.substr(0,temp));
	    	line=line.substr(temp+1);
	    

	    	temp = line.find(",");
	 		obj.rotateY=stof(line.substr(0,temp));
	    	line=line.substr(temp+1);
	    		
	    	temp = line.find(",");
	 		obj.rotateZ=stof(line.substr(0,temp));
	    	line=line.substr(temp+1);

	    	temp = line.find(",");
	 		obj.scaleX=stof(line.substr(0,temp));
	    	line=line.substr(temp+1);
	    		
	    	temp = line.find(",");
	 		obj.scaleY=stof(line.substr(0,temp));
	    	line=line.substr(temp+1);

	    	temp = line.find(",");
	 		obj.scaleZ=stof(line.substr(0,temp));
	    	line=line.substr(temp+1);	

	    	temp = line.find(",");
	 		obj.shape=stof(line.substr(0,temp));
	    	line=line.substr(temp+1);

	    	temp = line.find(",");
	 		obj.m=stof(line.substr(0,temp));
	    	line=line.substr(temp+1);
	    		
	    	temp = line.find(",");
	 		obj.x_offset=stof(line.substr(0,temp));
	    	line=line.substr(temp+1);

	    	temp = line.find(",");
	 		obj.y_offset=stof(line.substr(0,temp));
	    	line=line.substr(temp+1);
	    		    	
	    	temp = line.find(",");
	 		obj.z_offset=stof(line.substr(0,temp));
	    	line=line.substr(temp+1);
	    	track.push_back(obj);
	 	}
	 	yourfile.close();

	} else{
		printf("%s\n", "No such file, load fail");
	}
}


void save(){
	printf("%s\n", "Enter file name with .txt");
	char str[80];
	scanf("%79s", str);
	myfile.open(str);
	list<struct Object>::iterator p = track.begin();
		while(p != track.end()){
			myfile<< (*p).posX<<","<<(*p).posY<<","<<(*p).posZ<<","<<(*p).rotateX<<
			","<<(*p).rotateY<<","<<(*p).rotateZ<<","<<(*p).scaleX<<","<<(*p).scaleY<<","<<(*p).scaleZ<<
			","<<(*p).shape<<","<<(*p).m<<","<<(*p).x_offset<<","<<(*p).y_offset<<","<<
			(*p).z_offset<<","<<(*p).intersect<<"\n";
			p++;
	}
	myfile.close();

}



void eraseSelected() {
	list<struct Object>::iterator p = track.begin();
	while(p != track.end()){
		if ((*p).intersect){
			track.erase(p);
			break;
		}
		p++;
	}
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
	if(pt[0] > teapot.posX - teapot.x_offset && pt[0] < teapot.posX + teapot.x_offset &&
		pt[1] > teapot.posY - teapot.y_offset && pt[1] < teapot.posY + teapot.y_offset &&
		pt[2] > teapot.posZ - teapot.z_offset && pt[2] < teapot.posZ + teapot.z_offset)
		teapot.intersect = true;
	else
		teapot.intersect = false;

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
	result.m = material;
	result.shape = rand() % 6;
	if (result.shape == 4) {
		result.x_offset = 0.65;
		result.y_offset = 0.65;
		result.z_offset = 0.65;
	} else if (result.shape == 1) {
		result.x_offset = 0.85;
		result.y_offset = 0.85;
		result.z_offset = 0.85;
	}else {
		result.x_offset = 0.75;
		result.y_offset = 0.75;
		result.z_offset = 0.75;
	}

	return result;
}

void changeSelectedMaterial() {//change material for a selected object
	list<struct Object>::iterator p = track.begin();
	while(p != track.end()){
		if ((*p).intersect) {
			(*p).m = material;
		}
		
		p++;
	}
}

void rotateSelected(int key) {//keyboard control rotation
	list<struct Object>::iterator p = track.begin();
	while(p != track.end()){
		if ((*p).intersect) {
			switch (key) {
				case 'a':
					(*p).rotateZ--;
					break;
				case 'd':
					(*p).rotateZ++;
					break;
				case 'w':
					(*p).rotateX--;
					break;
				case 's':
					(*p).rotateX++;
					break;
				case 'q':
					(*p).rotateY--;
					break;
				case 'e':
					(*p).rotateY++;
					break;
			}
			break;
		}		
		p++;
	}	
}


void scaleSelected(int key) {//keyboard control scale
	list<struct Object>::iterator p = track.begin();
	while(p != track.end()){
		if ((*p).intersect) {
			switch (key) {
				case 'A':
					if ((*p).scaleZ > 0.2) {
						(*p).scaleZ-=0.2;
						(*p).z_offset -= 0.2;//(*p).scaleZ+0.2;
					} else {
						printf("%s\n", "Too small! try to scale another axis!");//cannot make the scale less than zero
					}
					
					break;
				case 'D':
					(*p).scaleZ+=0.2;
					(*p).z_offset += 0.25;//((*p).scaleZ+0.5);
					//printf("%d\n", (*p).scaleZ);
					break;
				case 'W':
					if ((*p).scaleX > 0.2) {
						(*p).scaleX-=0.2;
						(*p).x_offset -= 0.2;
					} else {
						printf("%s\n", "Too small! try to scale another axis!");
					}
					break;
				case 'S':
					(*p).scaleX+=0.2;
					(*p).x_offset += 0.25;
					break;
				case 'Q':
					if ((*p).scaleY > 0.2) {
						(*p).scaleY-=0.2;
						(*p).y_offset -= 0.2;
					} else {
						printf("%s\n", "Too small! try to scale another axis!");
					}
					break;
				case 'E':
					(*p).scaleY+=0.2;
					(*p).y_offset += 0.25;
					break;
			}
			(*p).intersect = true;
			break;
		}		
		p++;
	}	
}

void keyboard(unsigned char key, int xIn, int yIn)
{
	int mod = glutGetModifiers();
	if (mod == GLUT_ACTIVE_ALT) {
		printf("%s\n", "Enter rotate selected object mode");
		rotateSelected(key);
	} else if (mod == GLUT_ACTIVE_SHIFT) {
		printf("%s\n", "Enter scale selected object mode");
		scaleSelected(key);
	} 

	else {
		switch (key)
		{
			case 'q':
			case 27:	//27 is the esc key
				exit(0);
				break;
			case 'z':
				{
				list<struct Object>::iterator p = track.begin();
				while(p != track.end()){
					(*p).intersect = false;
					p++;
				}
				printf("%s\n", "Add a shape to the scene");
				
				track.push_back(createObject(rand()%10, rand()%10+1, rand()%10, 0,0,0,1,1,1));
				break;
				}

			case 's':
				printf("%s\n," "Save Scene");
				save(); 
				break;
			case 'j':
          		//decrease y position of light
          		light_pos1[1] -= 20;
          		light_pos2[1] -= 20;
          		break;
          	case 'u':
	          	//increase y position of light
	          	light_pos1[1] += 20;
	          	light_pos2[1] += 20;
	          	break;
			case 'h':
				light_pos1[0] -= 20;
	          	light_pos2[0] -= 20;
	          	break;
			case 'k':
				light_pos1[0] += 20;
	          	light_pos2[0] += 20;
	          	break;
	        case 'y':
	        	light_pos1[2] -= 20;
	          	light_pos2[2] -= 20;
	          	break;
	        case 'i':
	        	light_pos1[2] += 20;
	          	light_pos2[2] += 20;
	          	break;
			case 'l':
				printf("%s\n", "Load scene from file");
				load();
				break;
			case 'r':
				printf("%s\n", "Reset the scene to empty");
				track.clear();
				break;
			case 'm':
				printf("%s\n", "Change the material of selected object to current material");
				changeSelectedMaterial();
				break;
			case '1':
				material = 1;
				printf("%s\n", "Switch material to m1");
				break;
			case '2':
				material = 2;
				printf("%s\n", "Switch material to m2");
				break;
			case '3':
				material = 3;
				printf("%s\n", "Switch material to m3");
				break;
			case '4':
				material = 4;
				printf("%s\n", "Switch material to m4");
				break;
			case '5':
				material = 5;
				printf("%s\n", "Switch material to m5");
				break;
<<<<<<< HEAD
=======
			case '6':
				texture = 6;
				printf("%s\n", "Switch texture to image");
				break;
			case '7':
				texture = 7;
				printf("%s\n", "Switch texture to image1");
				break;
			case '8':
				texture = 8;
				printf("%s\n", "Switch texture to image2");
				break;
>>>>>>> 00e875470a66d10cc9f9316d80ca7f9f7ad79d01
			
		}
	}
	//to get the lighting work
	glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos1);
    glLightfv(GL_LIGHT1, GL_POSITION, light_pos2);
	glutPostRedisplay();
}

void translateSelected(int key) {//keyboard control translate
	list<struct Object>::iterator p = track.begin();
	while(p != track.end()){
		if ((*p).intersect) {
			switch (key) {
				case GLUT_KEY_LEFT:
					(*p).posX--;
					break;
				case GLUT_KEY_RIGHT:
					(*p).posX++;
					break;
				case GLUT_KEY_UP:
					(*p).posZ--;
					break;
				case GLUT_KEY_DOWN:
					(*p).posZ++;
					break;
			}
			break;
		}		
		p++;
	}	
}


void special(int key, int x, int y) {
	int mod = glutGetModifiers();
	if (mod == GLUT_ACTIVE_ALT){
		printf("%s\n", "Enter translate selected object mode");
		translateSelected(key);
	} else {
		switch (key) {
			case GLUT_KEY_LEFT:
				camPos[2] += 1;
				break;
			case GLUT_KEY_RIGHT:
				camPos[2] -= 1;
				break;
			case GLUT_KEY_UP:
				camPos[1] += 1;
				break;
			case GLUT_KEY_DOWN:
				if (camPos[1] >= 1) {
					camPos[1]-=1;
				} else {
					printf("%s\n", "Flliping is not allowed");
				}
				break;
				
		}
	}
	glutPostRedisplay();
}
void mouse(int btn, int state, int x, int y){

	mouseX = x;
	mouseY = WINDOW_SIZE_HEIGHT - y;
	list<struct Object>::iterator p = track.begin();
	while(p != track.end()){
		CalcIntersections(*p);
		p++;
		
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
	glEnable(GL_NORMALIZE);
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

void newShape() {//Customer shape
	glBegin(GL_QUADS);
		glNormal3f(0,1,0);
		glVertex3f(-0.5,0,-0.5);
		glNormal3f(0,1,0);
		glVertex3f(-0.5,0,0.5);
		glNormal3f(0,1,0);
		glVertex3f(0.5,0,0.5);
		glNormal3f(0,1,0);
		glVertex3f(0.5,0,-0.5);
	glEnd();
	glBegin(GL_QUADS);
		glNormal3f(0,1,0);
		glVertex3f(-0.5,1,-0.5);
		glNormal3f(0,1,0);
		glVertex3f(-0.5,1,0.5);
		glNormal3f(0,1,0);
		glVertex3f(0.5,1,0.5);
		glNormal3f(0,1,0);
		glVertex3f(0.5,1,-0.5);
	glEnd();
	glBegin(GL_QUADS);
		glNormal3f(1,-1,0);
		glVertex3f(0,1,0.5);
		glNormal3f(1,1,0);
		glVertex3f(0,0,0.5);
		glNormal3f(1,-1,0);
		glVertex3f(0,0,-0.5);
		glNormal3f(1,1,0);
		glVertex3f(0,1,-0.5);
	glEnd();
}

void draw(struct Object in) {
	setMaterial(in.m);
<<<<<<< HEAD
=======
	setTexture(in.t);

>>>>>>> 00e875470a66d10cc9f9316d80ca7f9f7ad79d01
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
			glutSolidOctahedron();
			break;
		case 4:
			glutSolidIcosahedron();
			break;
		case 5:
			newShape();
	}
}


//display function, also calls the position update and ray calc
void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);    
     
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(camPos[0], camPos[1], camPos[2], 0, 0, 0, 0,1,0);

	drawAxis();
	drawPlane();
	list<struct Object>::iterator p = track.begin();
	while(p != track.end()){
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
	//switch our buffers for a smooth animation
	glutSwapBuffers();
}


void callBackInit(){
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutSpecialFunc(special);
}

void info() {
	printf("%s\n\n", "Material: m1 to m5. default as m1");
	printf("%s\n\n", "Object shape: cube, teapot, sphere, octahedron, Icosahedron");
	printf("%s\n\n", "Extra feature: customer shape as a rotated 'H'");
	printf("%s\n\n", "Unselected object: yellow");
	printf("%s\n\n", "Selected object: red");
	printf("%s\n\n", "Note: the actual colour may looks weird because of the material");
	printf("%s\n\n", "q: quit the program");
	printf("%s\n\n", "1 to 5: switch current material");
	printf("%s\n\n", "m: apply current material to selected object");
<<<<<<< HEAD
=======
	printf("%s\n\n", "6 to 8: switch current texture");
	printf("%s\n\n", "t: apply current texture to selected object");
	
>>>>>>> 00e875470a66d10cc9f9316d80ca7f9f7ad79d01
	printf("%s\n\n", "l: load from file");
	printf("%s\n\n", "s: save to file");
	printf("%s\n\n", "click(right/left): select object");
	printf("%s\n\n", "right click object: delete object from scene");
	printf("%s\n\n", "up key/down key/left key/right key: camera control");
	printf("%s\n\n", "alt + up key/down key/left key/right key: translate selected object");
	printf("%s\n\n", "alt + w/a/s/d/q/e: rotate selected object");
	printf("%s\n\n", "shift + w/a/s/d/q/e: scale selected object");
	printf("%s\n\n", "h/j/k/y/u/i: control two lighting source on xyz axis");
}

int main(int argc, char** argv)
{
	info();
	glutInit(&argc, argv);		//starts up GLUT
	srand(time(NULL));
	init();
	callBackInit();
	

	glutMainLoop();				//starts the event glutMainLoop
	return(0);					//return may not be necessary on all compilers
}
