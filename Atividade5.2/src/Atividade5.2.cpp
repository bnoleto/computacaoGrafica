#include <GL/glut.h>  // GLUT, includes glu.h and gl.h
#include <vector>
#include "Teardrop.h"
#include <iostream>
#include <fstream>
#include <math.h>

using namespace std;

class Funcoes{
	public:
	vector<Teardrop*>* lista_teardrop = new vector<Teardrop*>;

	void config_glut(){
		glutCreateWindow("Atividade 5.2"); // Create a window with the given title

		gluOrtho2D(0, 500, 500, 0);
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // Set background color to black and opaque

		glColor3ub(255, 0, 0);
	}

	float rad(float angulo){	// converterá de graus para radianos
		return angulo*3.141592/180;
	}

	void novo_cjto_teardrop(int tipo, Ponto* ponto_central){

		lista_teardrop->clear();

		if(tipo == 1){
			int x = ponto_central->get_x();
			int y = ponto_central->get_y();
			for(int i = 0; i<8; i++){	// laço para 8 teardrops

				float angulo = i*45;

				Ponto* p_inicial_teardrop = new Ponto(	// irá se afastar 30 pixels do ponto central
						x + 30*cos(rad(angulo)),
						y - 30*sin(rad(angulo))
				);

				lista_teardrop->push_back(new Teardrop(p_inicial_teardrop,30,angulo+90));	// irá rotacionar o teardrop no ângulo atual + 90 graus em relação ao seu ponto inicial
			}
		}
		else{
			int x = ponto_central->get_x();
			int y = ponto_central->get_y();
			for(int i = 0; i<8; i++){

				float angulo = i*45;

				Ponto* p_inicial_teardrop = new Ponto(
						x + cos(rad(angulo)),
						y - sin(rad(angulo))
				);

				lista_teardrop->push_back(new Teardrop(p_inicial_teardrop,30,angulo));
			}
		}
	}

};

Funcoes funcoes; // classe global para as funções do programa

void display() {

	glViewport(0, 0, 500, 500);

	glClear(GL_COLOR_BUFFER_BIT);         // Clear the color buffer

	for(unsigned int j = 0; j< funcoes.lista_teardrop->size(); j++){
		Teardrop* atual = funcoes.lista_teardrop->at(j);
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
			funcoes.novo_cjto_teardrop(0, new Ponto(x,y));

		}
		if(botao == GLUT_RIGHT_BUTTON){
			funcoes.novo_cjto_teardrop(1, new Ponto(x,y));
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
