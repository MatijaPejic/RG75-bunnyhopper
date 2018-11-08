#include <GL/glut.h>
#include <stdlib.h>
#include<stdio.h>
static float x;
static float y;
static float z;
static float xv;
static float yv;
static float zv;
int w=0;
int a=0;
int d=0;
double start=0;
double offset=0;
double okvir=20;
static void ravan(double okvir,double offset);
static void on_keyboard(unsigned char key, int x, int y);
static void on_display(void);
static void on_timer(int value);
static void prepreke(int num);
int main(int argc, char **argv)
{
    x=-100;y=0;z=0;//sta gledam
    xv=20;yv=0.5;zv=0;//odakle gledam
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);

   
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow(argv[0]);

   
    glutDisplayFunc(on_display);
    glutKeyboardFunc(on_keyboard);
    glutTimerFunc(50, on_timer, 0);
    glEnable(GL_DEPTH_TEST);
    glClearColor(0, 0, 0, 0);

   
    glutMainLoop();

    return 0;
}
static void on_display(void){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_LINE_SMOOTH);
	glLineWidth(20);
	glBegin(GL_LINES);//x osa , crvena
	glColor3f(1,0,0);
	glVertex3f(0,0,0);
	glVertex3f(5,0,0);
	glEnd();

	glBegin(GL_LINES);//y osa , zelena
	glColor3f(0,1,0);
	glVertex3f(0,0,0);
	glVertex3f(0,20,0);
	glEnd();

	glBegin(GL_LINES);//z osa , plava
	glColor3f(0,0,1);
	glVertex3f(0,0,0);
	glVertex3f(0,0,5);
	glVertex3f(0,0,0);
	glVertex3f(0,0,-5);
	glEnd();

	glColor3f(1,1,1);
	glDisable(GL_LINE_SMOOTH);
	if(start>=20){
	  ravan(okvir,offset+2*okvir);
	  //offset+=2*okvir;
	}
	if(start>=40){
	  start=0;
	  offset+=2*okvir;
	}
	ravan(okvir,offset);
	glViewport(0, 0, 800, 600);// za prilagodjavanje slike velicini ekrana
	glMatrixMode(GL_PROJECTION);//ucitavamo matricu za projekciju
        glLoadIdentity();// i identitet
        gluPerspective(
            40,// ugao koji pogled zahvata u stepenima(sto veci to dalje)
            800/(float)600,//odnos sirine i visine
            1, 25);//rastojanje nablize i najdalje tacke
        glMatrixMode(GL_MODELVIEW);//ucitavamo matricu za gledanje
        glLoadIdentity();//i identitet
        gluLookAt(
            xv, yv, zv, //pozicija sa koje se gleda
            x, y, z, // u sta gledas (centar scene)
            0, 1, 0 // u koju stranu gledamo
                 );
	/*glPushMatrix();//rotacija sfere, pocetak 
          glRotatef(rotacija, 0, 1, 1);//ugao i ravan oko koje se rotira
	  glColor3f(1,1,1);//boja
          glutWireSphere(1, 20, 20);//sama sfera
        glPopMatrix();//kraj rotacije*/
    	prepreke(20);
/*-----------------------------------------------------------------------------------------------------------*/
	glutSwapBuffers();
}
static void on_keyboard(unsigned char key, int x, int y){
switch (key) {
    case 27:
        exit(0);
        break;	
    case 'a':
	a=1;d=0;
	break;
    case 'd':
	a=0;d=1;
	break;
    case 'w':
	w=1;a=0;d=0;	
	break;
    case ' ':
	w=0;a=0;d=0;
	break;
   }
}
static void ravan(double okvir,double offset){
//printf("start je : %f\n",start);
glColor3f(1,1,0);
glBegin(GL_LINE_STRIP);
	glVertex3f(okvir-offset,0,okvir);
	glVertex3f(okvir-offset,0,-okvir);
	glVertex3f(-okvir-offset,0,-okvir);
	glVertex3f(-okvir-offset,0,okvir);
	glVertex3f(okvir-offset,0,okvir);
glEnd();
int i;
float x=okvir;
float z=okvir;
glBegin(GL_LINES);
	for(i=0;i<160;i++){
	glVertex3f(x-offset,0,z);
	glVertex3f(x-offset,0,-z);
	x=x-0.25;
	}

glEnd();
x=okvir;
z=okvir;
glBegin(GL_LINES);
	for(i=0;i<160;i++){
	glVertex3f(x-offset,0,z);
	glVertex3f(-x-offset,0,z);
	z=z-0.25;
	}

glEnd();
}
static void on_timer(int value){
	if(value!=0)
		return;
	if(w==1 && a==1){
	   xv-=0.1;
	   zv+=0.1;
	   z+=0.1;
	   start+=0.1;
	}else if(w==1 && d==1){
	  xv-=0.1;
	  zv-=0.1;
	  z-=0.1;
	  start+=0.1;
	}else if(w==1){
	  xv-=0.1;
	  start+=0.1;
	}else if(a==1){
	  zv+=0.1;
	  z+=0.1;
	}else if(d==1){
	  zv-=0.1;
	  z-=0.1;
	}
		
        glutPostRedisplay();
	glutTimerFunc(50, on_timer, 0);
}
static void prepreke(int num){
	/*TODO*/
}





