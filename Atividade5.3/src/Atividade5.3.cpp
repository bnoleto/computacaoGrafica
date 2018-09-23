#include <GL/glut.h>  // GLUT, includes glu.h and gl.h
#include <vector>
#include <iostream>
#include <fstream>
#include "Ponto.h"
#include <math.h>

using namespace std;

class Funcoes{
	private:
	unsigned int dominio;

	public:
	vector<Ponto*>* lista_pontos = new vector<Ponto*>;

	void config_glut(){
		glutCreateWindow("Atividade 5.3"); // Create a window with the given title

		gluOrtho2D(0, 0, 1, 1);
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // Set background color to black and opaque
	}

	void funcao_matematica(unsigned int dominio){
		this->dominio = dominio;

		lista_pontos->clear();

		for(int i_x = -250; i_x <= 250; i_x++){

			float x = (float)i_x*this->dominio/250;		// domínio = -dominio <= x <= dominio
			//float y = x-100*(pow(x,5));									// f(x) = x-100*(x^5)
			//float y = pow(x,3);											// f(x) = x^3
			float y = pow(pow(x,2)+pow(x,2)-1,3)-(pow(x,2)*pow(x,3));		// f(x) = (x^2+x^2-1)^3-x^2*x^3

			lista_pontos->push_back(new Ponto(x,y));
			cout << x << ", " << y << endl;
		}
	}

	unsigned int get_dominio(){
		return this->dominio;
	}

	void set_dominio(unsigned int valor){
		this->dominio = valor;
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
			glVertex2f(atual->get_x()/funcoes.get_dominio(),atual->get_y()/funcoes.get_dominio());
		}
	glEnd();
	glFlush();  // Render now
}

void teclado (int tecla, int x, int y){
	if(tecla == GLUT_KEY_UP){
		funcoes.set_dominio(funcoes.get_dominio()*1.1+1);
	}
	if(tecla == GLUT_KEY_DOWN && funcoes.get_dominio()*0.9 >= 1){
		funcoes.set_dominio(funcoes.get_dominio()*0.9);
	}
	funcoes.funcao_matematica(funcoes.get_dominio());

	glutPostRedisplay();

}

/* Main function: GLUT runs as a console application starting at main()  */
int main(int argc, char** argv) {
	glutInit(&argc, argv);                 // Initialize GLUT
	glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
	glutInitWindowSize(500, 500);   // Set the window's initial width & height
	glutInitWindowPosition(100, 100); // Position the window's initial top-left corner
	funcoes.config_glut();
	funcoes.funcao_matematica(1);
	glutDisplayFunc(display); // Register display callback handler for window re-paint
	glutSpecialFunc(teclado);
	glutMainLoop();           // Enter the infinitely event-processing loop
	return 0;
}
