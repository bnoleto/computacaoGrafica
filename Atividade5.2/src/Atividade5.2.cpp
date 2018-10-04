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
	Ponto* ponto_central = new Ponto(250,250);
	float rotacionando = 0;
	int contador = 0;

	void config_glut(){
		glutCreateWindow("Atividade 5.2"); // Create a window with the given title

		gluOrtho2D(0, 500, 500, 0);
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // Set background color to black and opaque

		glColor3ub(255, 0, 0);

		novo_cjto_teardrop(1, 30);
	}

	float rad(float angulo){	// converterá de graus para radianos
		return angulo*3.141592/180;
	}

	void novo_cjto_teardrop(int tipo, int dist_centro){

		lista_teardrop->clear();

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
				lista_teardrop->push_back(new Teardrop(p_inicial_teardrop,30,angulo+90));	// irá rotacionar o teardrop no ângulo atual + 90 graus em relação ao seu ponto inicial
			}
			else{
				lista_teardrop->push_back(new Teardrop(p_inicial_teardrop,30,angulo));
			}
		}
	}


	void rotacionar(float angulo){

		for(unsigned int i = 0; i<lista_teardrop->size(); i++){

			lista_teardrop->at(i)->add_angulo(angulo,ponto_central);

		}
	}

};

Funcoes funcoes; // classe global para as funções do programa

void display() {

	if(funcoes.rotacionando != 0){
		funcoes.rotacionar(funcoes.rotacionando);
		glutPostRedisplay();
	}

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
/*
	cout << estado << " / " << GLUT_DOWN << endl;
	cout << funcoes.mouse_pressionado << endl;
*/


	if (estado == GLUT_DOWN){

		/*
		funcoes.ponto_central = new Ponto(x,y);
		if(botao == GLUT_LEFT_BUTTON){
			funcoes.novo_cjto_teardrop(0, 0);

		}
		if(botao == GLUT_RIGHT_BUTTON){
			funcoes.novo_cjto_teardrop(1, 30);
		}*/
		if(botao == GLUT_LEFT_BUTTON){
			funcoes.rotacionando = -0.1;
		}
		if(botao == GLUT_RIGHT_BUTTON){
			funcoes.rotacionando = 0.1;
		}
	}
	else{
		funcoes.rotacionando = 0;
	}
	glutPostRedisplay();
}

void teclado(unsigned char tecla, int x, int y){
	/*if  (tecla == 'r'){
		funcoes.rotacionar(-1);
	}
	if  (tecla == 'R'){
		funcoes.rotacionar(1);
	}*/
	/*
	if(toupper(tecla) == 'R'){
		funcoes.rotacionando = !funcoes.rotacionando;
	}
	glutPostRedisplay();*/
}

/* Main function: GLUT runs as a console application starting at main()  */
int main(int argc, char** argv) {
	glutInit(&argc, argv);                 // Initialize GLUT
	glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
	glutInitWindowSize(500, 500);   // Set the window's initial width & height
	glutInitWindowPosition(100, 100); // Position the window's initial top-left corner
	funcoes.config_glut();
	glutMouseFunc(mouse);
	glutKeyboardFunc(teclado);
	glutDisplayFunc(display); // Register display callback handler for window re-paint
	glutMainLoop();           // Enter the infinitely event-processing loop
	return 0;
}
