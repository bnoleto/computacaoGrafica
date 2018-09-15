#include <GL/glut.h>  // GLUT, includes glu.h and gl.h
#include <vector>
#include "Teardrop.h"
#include <iostream>
#include <fstream>
#include <Math.h>

using namespace std;

class Funcoes{
	public:
	Teardrop* teardrop = nullptr;

	void config_glut(){
		glutCreateWindow("Atividade 5.1"); // Create a window with the given title

		gluOrtho2D(0, 500, 500, 0);
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // Set background color to black and opaque

		glColor3ub(255, 0, 0);
	}

};

Funcoes funcoes; // classe global para as funções do programa

void display() {

	glViewport(0, 0, 500, 500);

	glClear(GL_COLOR_BUFFER_BIT);         // Clear the color buffer

	Teardrop* atual = funcoes.teardrop;
	if(funcoes.teardrop != nullptr){
		glBegin(GL_LINE_LOOP);
			for(unsigned int i = 0; i< atual->get_pontos()->size(); i++){
				glVertex2i(atual->get_pontos()->at(i)->get_x(),atual->get_pontos()->at(i)->get_y());
			}
		glEnd();
	}

	glFlush();  // Render now
}

void mouse(int botao, int estado, int x, int y){

	if (estado == GLUT_DOWN){
		if(botao == GLUT_LEFT_BUTTON){
			funcoes.teardrop = new Teardrop(new Ponto(x,y),30,120);
		}
	}
}

/* Main function: GLUT runs as a console application starting at main()  */
int main(int argc, char** argv) {
	glutInit(&argc, argv);                 // Initialize GLUT
	glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
	glutInitWindowSize(500, 500);   // Set the window's initial width & height
	glutInitWindowPosition(100, 100); // Position the window's initial top-left corner
	funcoes.config_glut();
	glutMouseFunc(mouse);
	glutDisplayFunc(display); // Register display callback handler for window re-paint
	glutMainLoop();           // Enter the infinitely event-processing loop
	return 0;
}
