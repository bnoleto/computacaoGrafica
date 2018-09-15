#include <GL/glut.h>  // GLUT, includes glu.h and gl.h
#include <vector>
#include <iostream>
#include <fstream>
#include "Ponto.h"
#include <math.h>

using namespace std;

class Funcoes{
	public:
	vector<Ponto*>* lista_pontos = new vector<Ponto*>;

	void config_glut(){
		glutCreateWindow("Atividade 5.3"); // Create a window with the given title

		gluOrtho2D(0, 0, 1, 1);
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // Set background color to black and opaque
	}

	void funcao_matematica(){
		for(int i_x = -1000; i_x< 1000; i_x++){

			float x = (float)i_x/1000;
			float y = x-100*(pow(x,5));		// f(x) = x-100*(x^5)
			//float y = pow(x,3);		// f(x) = x³
			lista_pontos->push_back(new Ponto(x,y));
		}
	}
};

Funcoes funcoes; // classe global para as funções do programa

void display() {

	glViewport(0, 0, 500, 500);

	glClear(GL_COLOR_BUFFER_BIT);         // Clear the color buffer


	// irá desenhar os eixos, cada grade corresponde a 0.1
	glColor3ub(50, 50, 50);
	glBegin(GL_LINES);
		glVertex2f(-1,0);
		glVertex2f(1,0);
		glVertex2f(0,-1);
		glVertex2f(0,1);
	for(int i = -10; i < 10; i++){
		glVertex2f((float)i/10,-0.01);
		glVertex2f((float)i/10,0.01);
		glVertex2f(-0.01,(float)i/10);
		glVertex2f(0.01,(float)i/10);
	}
	glEnd();

	// desenho da função
	glColor3ub(255, 0, 0);
	glBegin(GL_LINE_STRIP);
	for(unsigned int j = 0; j< funcoes.lista_pontos->size(); j++){
		Ponto* atual = funcoes.lista_pontos->at(j);
		cout << atual->get_x() << ", " << atual->get_y() << endl;
			glVertex2f(atual->get_x(),atual->get_y());
		}
	glEnd();
	glFlush();  // Render now
}


/* Main function: GLUT runs as a console application starting at main()  */
int main(int argc, char** argv) {
	glutInit(&argc, argv);                 // Initialize GLUT
	glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
	glutInitWindowSize(500, 500);   // Set the window's initial width & height
	glutInitWindowPosition(100, 100); // Position the window's initial top-left corner
	funcoes.config_glut();
	funcoes.funcao_matematica();
	glutDisplayFunc(display); // Register display callback handler for window re-paint
	glutMainLoop();           // Enter the infinitely event-processing loop
	return 0;
}
