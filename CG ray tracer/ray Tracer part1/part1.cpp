#include <math.h>

#include <stdlib.h>

#ifdef WIN32

#include <windows.h>

#endif

#include <GL/glut.h>



struct RGBType {

    float r;

    float g;

    float b;

};



void Init( ) {



    glViewport(0, 0, 600, 600);

    glMatrixMode(GL_MODELVIEW);

    glLoadIdentity( );

    glMatrixMode(GL_PROJECTION);

    glLoadIdentity( );

    gluOrtho2D(0., 600., 0., 600.);



}



void onDisplay( ) {



    glClearColor(1, 0, 0, 0);

    glClear(GL_COLOR_BUFFER_BIT);

    RGBType *pixels = new RGBType[600*600];



        for (int i=0; i< 400*400; i++) {



            pixels[i].r=0.2;

            pixels[i].g=0.7;

            pixels[i].b=0.7;



        }





    glDrawPixels(600,600,GL_RGB,GL_FLOAT,pixels);

    glutSwapBuffers();

    delete[] pixels;



}



int main(int argc, char * argv[]) {

    glutInit(&argc, argv);

    glutInitWindowSize(600, 600);

    glutInitWindowPosition(100, 100);

    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("practise image");

    Init();

    glutDisplayFunc(onDisplay);

    glutMainLoop();

}
