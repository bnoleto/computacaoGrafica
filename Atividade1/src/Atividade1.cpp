#include <windows.h>  // For MS Windows
#include <GL/glut.h>  // GLUT, includes glu.h and gl.h
#include <vector>
#include "Pixel.h"
#include <iostream>

using namespace std;

vector<Ponto> pixels;


/* Handler for window-repaint event. Call back when the window first appears and
   whenever the window needs to be re-painted. */
void display() {
	glClearColor(1.0f, (float)214/255, (float)185/255, 1.0f); // Set background color to black and opaque
	glClear(GL_COLOR_BUFFER_BIT);         // Clear the color buffer

	glBegin(GL_LINES);	// gradiente do fundo
	int cont = 0;
		for(unsigned int i = tamanhoJanelaY/2; i<tamanhoJanelaY; i++){
			glColor3f(1.0, (double)(214-cont)/255, (double)(185-cont)/255);
			glVertex2i(0, i);
			glVertex2i(tamanhoJanelaX, i);
			if(i%3 == 0)
				cont++;
		}
	glEnd();

	glPointSize(tamanhoPixel);
	glBegin(GL_POINTS);
		for(unsigned int i = 0; i<pixels.size(); i++){
			pixels.at(i).imprimir_pixel();
		}
	glEnd();

	glFlush();  // Render now
}

/* Main function: GLUT runs as a console application starting at main()  */
int main(int argc, char** argv) {
	inicializar_pixels(pixels);

	glutInit(&argc, argv);                 // Initialize GLUT
	glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
	glutInitWindowSize(tamanhoJanelaX, tamanhoJanelaY);   // Set the window's initial width & height
	glutInitWindowPosition(100, 100); // Position the window's initial top-left corner
	glutCreateWindow("Atividade 1"); // Create a window with the given title
	gluOrtho2D(0, tamanhoJanelaX, tamanhoJanelaY, 0);
	glutDisplayFunc(display); // Register display callback handler for window re-paint
	glutMainLoop();           // Enter the infinitely event-processing loop
	return 0;
}
