#include <GL/glut.h>  // GLUT, includes glu.h and gl.h
#include <vector>
#include "Teardrop.h"
#include <iostream>
#include <fstream>
#include <math.h>

using namespace std;

class Funcoes{
	public:
	vector<vector<Teardrop*>*>* lista_cjtos = new vector<vector<Teardrop*>*>;
	Ponto* ponto_central = nullptr;
	vector<Teardrop*>* conjunto_corrente = nullptr;

	void config_glut(){
		glutCreateWindow("Prova de Computação Gráfica"); // Create a window with the given title

		gluOrtho2D(0, 800, 600, 0);
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // Set background color to black and opaque

		glColor3ub(255, 0, 0);

	}

	float rad(float angulo){	// converterá de graus para radianos
		return angulo*3.141592/180;
	}

	void novo_cjto_teardrop(int tipo, int dist_centro){

		vector<Teardrop*>* conjunto_teardrop = new vector<Teardrop*>;

		int x = ponto_central->get_x();
		int y = ponto_central->get_y();
		int qtd_teardrops = 8;
		for(int i = 0; i<qtd_teardrops; i++){	// laço para 8 teardrops

			float angulo = i*(360/qtd_teardrops);

			Ponto* p_inicial_teardrop = new Ponto(	// irá se afastar 30 pixels do ponto central
					x + dist_centro*cos(rad(angulo)),
					y - dist_centro*sin(rad(angulo))
			);

			if(tipo == 1){
				conjunto_teardrop->push_back(new Teardrop(p_inicial_teardrop,30,angulo+90));	// irá rotacionar o teardrop no ângulo atual + 90 graus em relação ao seu ponto inicial
			}
			else{
				conjunto_teardrop->push_back(new Teardrop(p_inicial_teardrop,30,angulo));
			}
		}

		conjunto_corrente = conjunto_teardrop;
		lista_cjtos->push_back(conjunto_teardrop);
	}


	void rotacionar(float angulo, vector<Teardrop*>* cjto_atual){

		for(unsigned int i = 0; i<cjto_atual->size(); i++){

			cjto_atual->at(i)->add_angulo(angulo,ponto_central);

		}
		glutPostRedisplay();
	}

};

Funcoes funcoes; // classe global para as funções do programa

void display() {

	if(funcoes.lista_cjtos->size() > 0){
		funcoes.rotacionar(0.1, funcoes.conjunto_corrente);
	}

	glViewport(0, 0, 800, 600);

	glClear(GL_COLOR_BUFFER_BIT);         // Clear the color buffer


	for(unsigned int k = 0; k< funcoes.lista_cjtos->size(); k++){ // lista de conjuntos de teardrops

		vector<Teardrop*>* conjunto_atual = funcoes.lista_cjtos->at(k);

		for(unsigned int j = 0; j< conjunto_atual->size(); j++){ // lista de teardrops de um conjunto
			Teardrop* atual = conjunto_atual->at(j);
			glBegin(GL_LINE_LOOP);
				for(unsigned int i = 0; i< atual->get_pontos()->size(); i++){ // lista de pontos de um teardrop
					glVertex2i(atual->get_pontos()->at(i)->get_x(),atual->get_pontos()->at(i)->get_y());
				}
			glEnd();
		}
	}

	glFlush();  // Render now
}

void mouse(int botao, int estado, int x, int y){

	if (estado == GLUT_DOWN){


		funcoes.ponto_central = new Ponto(x,y);
		if(botao == GLUT_LEFT_BUTTON){
			funcoes.novo_cjto_teardrop(0, 0);

		}
		if(botao == GLUT_RIGHT_BUTTON){
			funcoes.novo_cjto_teardrop(1, 30);
		}
	}
	glutPostRedisplay();
}

/* Main function: GLUT runs as a console application starting at main()  */
int main(int argc, char** argv) {
	glutInit(&argc, argv);                 // Initialize GLUT
	glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
	glutInitWindowSize(800, 600);   // Set the window's initial width & height
	glutInitWindowPosition(100, 100); // Position the window's initial top-left corner
	funcoes.config_glut();
	glutMouseFunc(mouse);
	glutDisplayFunc(display); // Register display callback handler for window re-paint
	glutMainLoop();           // Enter the infinitely event-processing loop
	return 0;
}
