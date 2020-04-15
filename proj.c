#include <GL/glut.h>
#include <stdlib.h>
#include<stdio.h>
#include<string.h>
#include<unistd.h>
static float x; // (x,y,z)-> sta gledamo
static float y; // (xv,yv,zv)->odakle gledamo , koordinate za gluLookAt, globalne promenjive jer cemo ih menjati
static float z;
static float xv;
static float yv;
static float zv;
int w=0;  /* w, a, d, c, v su indikatori za pritisnute tastere*/
int a=0;
int d=0;
int c=0;
int v=0;
int score=0; // trenutni rezultat
int highScore=0; // najveci rezultat
float visina_skoka=1; // koliko cemo se pomeriti prilikom skoka
double start=0; //predjen put
double offset=0; // velicina koja oznacava za koliko ce se objekti daleko crtati od svoje pocetne lokacije
double okvir=6; // velicina platforme (tla)
double jump_timer=0; // tajmer koji oznacava koliko cemo dugo biti u skoku
static void ravan(double okvir,double offset);	/* Detaljni opisi funkcija se nalaze kod njihove definicije*/
static void on_keyboard(unsigned char key, int x, int y);
static void on_display(void);
static void on_timer(int value);
static void prepreke(double offset);
static void new_game(void);
static void sudar(double zv, double z1, double z2, double x);
static void output(double x,double y,double z,const char *string);
static void miniMenu();
int main(int argc, char **argv){
    x=-100;y=0;z=0;        
    xv=okvir;yv=0.5;zv=0; 
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);

     /*SVE INICIJALIZACE PREUZETE SA SAJTA ASISTENTA*/
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
	/*
	 * PODESAVANJE SVETLA
	 * */
	glShadeModel(GL_SMOOTH);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);
	GLfloat light_position[] = { xv, yv+2, zv+1, 1 }; // IZVOR SVETLOSTI SE KRECE SA NAMA (Mi smo izvor svetla), svetlost se ne baca na sve strane vec samo ispred nas
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	
	GLfloat light_ambient[] = { 0, 0, 0, 1 };
	GLfloat ambient_coeffs[] = { 1, 1, 1, 1 };
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient_coeffs);
	
	GLfloat light_diffuse[] = {1.5, 1.5, 1.5, 1 };
	GLfloat diffuse_coeffs[] = { 0.0, 0.0, 1, 1 };
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse_coeffs);
	
	GLfloat light_specular[] = { 0.9, 0, 0, 1 };
	GLfloat specular_coeffs[] = { 1, 1, 1, 1 };
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular_coeffs);
	
	GLfloat shininess = 120;
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
	/*
	  KRAJ PODESAVANJA SVETLA
	 */
	
	glColor3f(1,1,1);
	glDisable(GL_LINE_SMOOTH);
	if(start>=okvir/8){ //Funkcija ravan crta tlo po kojem se krecemo, start oznacava rastojanje koje smo presli, kada predjemo 1/8 tla crta se tlo ispred nas
	  ravan(okvir,offset+2*okvir);
	  prepreke(offset+2*okvir); // Funckija prepreke iscrtava prepreke
	}
	if(start>=2*okvir){// Kada predjemo celo tlo, azuriramo start i offset tako da se sada iscrtava samo tlo na kojem smo(ono sto je iza nas se vise ne crta)
	  start=0;
	  offset+=2*okvir;
	}
	miniMenu();// Funkcija za pracenje rezultata
	ravan(okvir,offset);//Ovim pozivom funkcije ravan se crta tlo na kojem smo trenutno,
	prepreke(offset);// kao i prepreke
	double clip[]={1,0,0,10-xv}; // Klip ravan sluzi da sakrije ta nova iscrtavanja tla, klip ravan se pomera sa nama i daje iluziju da se sve ispred nas glatko generise
	glEnable(GL_CLIP_PLANE0);
	glClipPlane(GL_CLIP_PLANE0,clip);
	glViewport(0, 0, 800, 600);
	glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(
            40,
            800/(float)600,
            1, 25);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        gluLookAt(
            xv, yv, zv,  /*Globalne promenljive koje se azuriraju pritiskom tastera*/
            x, y, z, 
            0, 1, 0 
                 );
	glutSwapBuffers();
}
static void on_keyboard(unsigned char key, int x, int y){
switch (key) {	//PRITISKANJE TASTERA VISE PUTA NEMA EFEKAT
    case 27:
        exit(0);
        break;
    case 'c':	 //SKOK
      c=1;
      v=0;
      break;
    case 'a':	//LEVO
	a=1;d=0;
	break;
    case 'd':	//DESNO
	a=0;d=1;
	break;
    case 'w':	//NAPRED
	w=1;a=0;d=0;	
	break;
    case ' ':	//PAUZA
	w=0;a=0;d=0;
	break;
   }
}
static void ravan(double okvir,double offset){
  /*Funckija ravan slika tlo po kojem se krecemo, sastoji se iz dva dela, jedan crta okvir a drugi grid*/
glColor3f(0,1,0);
    /*OKVIR*/
glBegin(GL_LINE_STRIP);
	glVertex3f(okvir-offset,0,okvir);
	glVertex3f(okvir-offset,0,-okvir);
	glVertex3f(-okvir-offset,0,-okvir);
	glVertex3f(-okvir-offset,0,okvir);
	glVertex3f(okvir-offset,0,okvir);
glEnd();
glBegin(GL_LINE_STRIP);
	glVertex3f(okvir-offset,2,okvir);
	glVertex3f(okvir-offset,2,-okvir);
	glVertex3f(-okvir-offset,2,-okvir);
	glVertex3f(-okvir-offset,2,okvir);
	glVertex3f(okvir-offset,2,okvir);
glEnd();
  /*GRID*/
int i;
float x=okvir;
float z=okvir;
glBegin(GL_LINES);
	for(i=0;i<okvir*8;i++){
	glVertex3f(x-offset,0,z);
	glVertex3f(x-offset,0,-z);
	x=x-0.25;
	}

glEnd();
x=okvir;
z=okvir;
glBegin(GL_LINES);
	for(i=0;i<okvir*8;i++){
	glVertex3f(x-offset,0,z);
	glVertex3f(-x-offset,0,z);
	z=z-0.25;
	}

glEnd();

glBegin(GL_LINES);
	for(i=0;i<okvir*8;i++){
	glVertex3f(x-offset,2,z);
	glVertex3f(x-offset,2,-z);
	x=x-0.25;
	}

glEnd();
x=okvir;
z=okvir;
glBegin(GL_LINES);
	for(i=0;i<okvir*8;i++){
	glVertex3f(x-offset,2,z);
	glVertex3f(-x-offset,2,z);
	z=z-0.25;
	}

glEnd();
}
static void on_timer(int value){
	if(value!=0)
		return;
	if(yv<-5){ /*Ako yv (y koordinata nase trenutne lokacije) postane -5 igra se resetuje jer to znaci da smo pali sa platforme*/
	 new_game();
	}
	if(zv>okvir || zv<-okvir || yv<0){ /*Ako izadjemo van okvira padamo sa platforme ,yv (y koordinata nase trenutne lokacije) se smanjuje*/
	  yv-=0.2;
	  c=0;
	}
	if(c==1){ // Komanda za skok
	    yv=visina_skoka; // nasa y kordinata postaje visina_skoka
	    jump_timer++; // tajmer koji broji koliko smo dugo u vazduhu
	    if(jump_timer >= 16){ // kada se dostigne odrednjena vrednost tajmera vracamo se na tlo
	    v=1;
	    c=0;
	    jump_timer=0;
	    yv=0.5;
	    }
	}
	
	/*Razne kombinacije kretanja komandi W, A, D, azuriranje rezultata i prednjenog puta*/
	if(w==1 && a==1){
	   xv-=0.1;
	   x-=0.1;
	   zv+=0.1;
	   z+=0.1;
	   start+=0.1;
	   score+=5;
	}else if(w==1 && d==1){
	  xv-=0.1;
	  x-=0.1;
	  zv-=0.1;
	  z-=0.1;
	  start+=0.1;
	  score+=5;
	}else if(w==1){
	  xv-=0.1;
	  x-=0.1;
	  start+=0.1;
	  score+=5;
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
static void prepreke(double offset){
  /*Iscrtavanje prepreka je hardkodovano, offset oznacava koliko daleko ce se iscrtati prepreka od pocetka (centra prve platforme)*/
glColor3f(1,0,0);
glBegin(GL_QUADS);
  glVertex3f(0-offset,0,4);
  glVertex3f(0-offset,0.5,4);
  glVertex3f(0-offset,0,2);
  glVertex3f(0-offset,0.5,2);
glEnd();
  /*Posle svake nacrtane prepreke se poziva funckija sudar koja proverava da li se igrac sudario bas sa tom preprekom */
sudar(zv,2,4,0-offset);

glColor3f(0,0,1);
glBegin(GL_QUADS);
  glVertex3f(2-offset,0,2);
  glVertex3f(2-offset,0.5,2);
  glVertex3f(2-offset,0.5,-1);
  glVertex3f(2-offset,0,-1);
glEnd();

sudar(zv,-1,2,2-offset);

glColor3f(1,1,0);
glBegin(GL_QUADS);

  glVertex3f(0-offset,0,-2);
  glVertex3f(0-offset,1,-2);
  glVertex3f(0-offset,0,-5);
  glVertex3f(0-offset,1,-5);
glEnd();

sudar(zv,-5,-2,0-offset);

glBegin(GL_TRIANGLES);

  glVertex3f(4-offset,0,0);
  glVertex3f(4-offset,1,0);
  glVertex3f(4-offset,0,3);
glEnd();

sudar(zv,0,3,4-offset);

glColor3f(0,1,1);
glBegin(GL_TRIANGLES);
 
  glVertex3f(4-offset,0,-2);
  glVertex3f(4-offset,1,-3);
  glVertex3f(4-offset,0,-5);
glEnd();

sudar(zv,-5,-2,4-offset);

glBegin(GL_TRIANGLES);
  
  glVertex3f(8-offset,0,4);
  glVertex3f(8-offset,1,4);
  glVertex3f(8-offset,0,6);
glEnd();


sudar(zv,4,6,8-offset);

glBegin(GL_QUADS);
  
  glVertex3f(9-offset,0,-2);
  glVertex3f(9-offset,0.5,-2);
  glVertex3f(9-offset,0.5,1);
  glVertex3f(9-offset,0,1);
glEnd();

sudar(zv,-2,1,9-offset);



glBegin(GL_QUADS);
 
  glVertex3f(-2-offset,0,6);
  glVertex3f(-2-offset,0.5,6);
  glVertex3f(-2-offset,0,4);
  glVertex3f(-2-offset,0.5,4);
glEnd();

sudar(zv,4,6,-2-offset);

glColor3f(1,1,1);
glBegin(GL_QUADS);

  glVertex3f(4-offset,0,5);
  glVertex3f(4-offset,0.5,5);
  glVertex3f(4-offset,0.5,4);
  glVertex3f(4-offset,0,4);
glEnd();

sudar(zv,4,5,4-offset);


glBegin(GL_QUADS);
 
  glVertex3f(-5-offset,0,-2);
  glVertex3f(-5-offset,1,-2);
  glVertex3f(-5-offset,0,-5);
  glVertex3f(-5-offset,1,-5);
glEnd();

sudar(zv,-5,-2,-5-offset);



}
static void new_game(void){
  /*Ova funckija resetuje sve globalne promenljive i vraca nas na pocetak, azurira High Score i vraca trenutni score na 0*/
w=0;
a=0;
d=0;
start=0;
offset=0;
okvir=6;
x=-100;y=0;z=0;
xv=okvir;yv=0.5;zv=0;
if(score >= highScore)
  highScore=score;
score=0;
}
static void sudar(double zv, double z1, double z2, double x){ 
  /*Funcija sudar uzima trenutnu koordinatu na kojoj se nalazi igrac u ravni na kojoj se on krece (zv) kao i sirinu objekta (z1 z2),
   x je koordinata na kojoj se objekat nalazi*/
  if(c == 1) /*Ako je c==1 znaci da smo skocili i da mozemo proci "kroz" objekat tj. iznad njega , u suprotnom...*/
    return;
  if (zv >= z1 && zv <= z2 && xv <= x && xv >= x-0.1) 
 /*ako je nasa lokacija u ravni(zv) izmedju z1 i z2 i ako je nasa X koordinata (xv) "ista" kao lokacija objekta (x) detektuje se sudar i igra se zavrsava*/
    new_game();
}
static void output(double x,double y,double z,const char *string){//Preuzeto sa man strane za glutBitmapCharacter, ispis rezultata
  int len,i;
  glRasterPos3f(x,y,z);
  len=(int)strlen(string);
  for(i=0;i<len;i++){
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,string[i]);
  }
  
}
static void miniMenu(){ /*Odredjivanje lokacije gde ce biti ispisan rezultat (funkcija output)*/
glColor3f(1,1,1);
output(xv-6,2+yv,zv+2.8,"Current score: ");
char str[12];
sprintf(str, "%d", score);
output(xv-6,2+yv,zv+1.5,str);
output(xv-6,1.8+yv,zv+2.8,"This session's highest score: ");
char str2[12];
sprintf(str2, "%d", highScore);
output(xv-6,1.8+yv,zv+1,str2);
}



