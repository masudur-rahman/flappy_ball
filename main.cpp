#include <bits/stdc++.h>
#include <windows.h>
#include <GL/gl.h>
#include <GL/glut.h>
using namespace std;

double cx=-120, cy=-70, jumpLim=-40, xr=190, xl=185;
bool jumpUp=false, jumpDown=false;
double start=clock();
int highScore=0, currentScore=0;
void Timer(int n)
{
    glutPostRedisplay();
    glutTimerFunc(10, Timer, 10);
}
void delay(){
    double length=clock()/CLOCKS_PER_SEC+.4;
    while(clock()/CLOCKS_PER_SEC<length);
}
void earth(){
    glBegin(GL_LINES);
        for(double i=0; i<10; i++){
            glColor3f(0+i/10, 0+i/10, 0+i/10);
            glVertex2d(-180, -80.3-i);
            glVertex2d(180, -80.3-i);
        }
    glEnd();
}

void circle(double cx, double cy){
    glBegin(GL_TRIANGLE_FAN);
    glColor3f(0, 0, 1);
    for(double i=0.0; i<360; i+=2){
        double theta=3.1415926*i/180.0;
        double x=5.0*cos(theta);
        double y=5.0*sin(theta);
        glVertex2f(x+cx, y+cy);
        //cout<<x+cx<<' '<<y+cy<<endl;
    }
    glEnd();
}
void obstacles(){
    glColor3f(1, 0, 0);
    glBegin(GL_QUADS);
    glVertex2d(xl, -80);
    glVertex2d(xr, -80);
    glVertex2d(xr, -60);
    glVertex2d(xl, -60);
    glEnd();
}
void restart(){
    cx=-120; cy=-70; jumpLim=-40; xr=190; xl=185;
}
bool isEverythingOkay(){
    for(double i=360.0; i>180; i-=20){
        double theta=3.1415926*i/180.0;
        double x=5.0*cos(theta)+cx;
        double y=5.0*sin(theta)+cy;
        if(x>=xl and x<=xr and y<=-61) return false;
    }
    return true;
}
void drawString(float x, float y, float z, char *string)
{
    glRasterPos3f(x, y, z);
    //glColor3f(0, 0, 1);
    for (char* c=string; *c != '\0'; c++){
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
    }
}
string integerToString(int n){
    string str="";
    while(n){
        int a=n%10;
        str+=a+'0';
        n/=10;
    }
    reverse(str.begin(), str.end());
    if(str.empty()) return "0";
    return str;
}
void showScore(int hscore, int cscore){
    glColor3f(0, 0.7, 0);
    glRasterPos2i(50, 180);
    string hs="High Score: "+integerToString(hscore);
    for(int i=0; i<hs.size(); i++){
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, hs[i]);
    }

    glRasterPos2i(50, 140);
    string cs="Current Score: "+integerToString(cscore);
    for(int i=0; i<cs.size(); i++){
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, cs[i]);
    }
}
void display(void){
    currentScore=(clock()-start)/CLOCKS_PER_SEC*100.0;
    earth();

    //if(xl<=cx+5.0 and xl>=cx-5.0 and cy-5.0<=-60) exit(0);
    if(isEverythingOkay()){
        if(jumpUp) cy+=1;
        if(cy>=jumpLim) jumpUp=false, jumpDown=true;
        if(jumpDown) cy-=1;
        if(cy<=-70) jumpDown=false;
        circle(cx, cy);
        xl-=1; xr-=1;
        if(xl<-180) xl=185, xr=190;

        showScore(highScore, currentScore);
    }
    else{
        drawString(0, 0, 0, "GAME OVER...");
        highScore=max(highScore, currentScore);
        showScore(highScore, 0);
        start=clock();
    }
    obstacles();
    glColor3f(0, 0, 0);

    glutSwapBuffers();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
void init(void){
    glClearColor(.8, .8, .8, 0);
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-200, 200, -200, 200);
}
static void key(unsigned char key, int x, int y){
    switch (key){
        case 27 :
        case 'q':
            exit(0);
            break;

        case ' ':
            if(!jumpUp and !jumpDown) jumpUp=true;
            break;
        case 'r':
            jumpDown=false; jumpUp=false;
            start=clock();
            restart();
            break;
    }
    glutPostRedisplay();
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(600, 600);
    glutInitWindowPosition(00, 00);
    glutCreateWindow("Flappy Ball Game:");
    init();
    glutKeyboardFunc(key);
    glutDisplayFunc(display);
    Timer(0);
    glutMainLoop();
    return 0;
}
