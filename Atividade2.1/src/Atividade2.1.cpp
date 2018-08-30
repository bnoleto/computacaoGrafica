#include <GL/glut.h>  // GLUT, includes glu.h and gl.h
#include <vector>
#include "Ponto.h"
#include <iostream>
#include <Math.h>

using namespace std;

class Funcoes{
	private:

	vector<Ponto> pontos;
	Ponto excluido = Ponto(-1,-1); //inicial

	public:

	void adicionar_ponto(int x, int y){
		pontos.push_back(Ponto(x,y));
		cout << "Ponto (" << x << ", " << y << ") adicionado!" << endl;
		excluido = Ponto(-1,-1);
	}

	void remover_ponto(Ponto ponto){
		short x = ponto.get_x(), y = ponto.get_y();

		for(unsigned int i = pontos.size()-1; i >= 0; i--){		// começará da posição final para priorizar a remoção do ponto criado por último
			if(pontos.at(i).get_x() == x && pontos.at(i).get_y() == y){
				excluido = pontos.at(i);
				cout << "Ponto (" << x << ", " << y << ") removido." << endl;
				pontos.erase(pontos.begin() + i);	// iterador do elemento inicial + posição i
				break;
			}
		}
	}

	vector<Ponto> get_pontos(){
		return this->pontos;
	}

	float calcularDistancia(Ponto p1, int x, int y){	// fórmula matemática para calcular a distância entre dois pontos
		float xA = (float)p1.get_x();
		float xB = (float)x;
		float yA = (float)p1.get_y();
		float yB = (float)y;

		return sqrt(pow((xB-xA),2)+pow((yB-yA),2));
	}

	Ponto pontoMaisProx(int x, int y){ // retornará o índice do ponto mais próximo ao clique
		float menorDistancia;
		int indiceMenorDistancia;

		vector<Ponto> pontos = get_pontos();

		for(unsigned int i = 0; i< pontos.size(); i++){		// o laço irá comparar as distâncias entre cada ponto do vetor e a posição clicada
			if(i==0){				// ação a ser realizada caso tenha apenas 1 ponto declarado no vetor
				menorDistancia = calcularDistancia(pontos.at(i), x, y);
				indiceMenorDistancia = i;
			}
			else{					// ação a ser realizada caso tenha 2 ou mais pontos declarados no vetor
				if(calcularDistancia(pontos.at(i), x, y) < menorDistancia){
					menorDistancia = calcularDistancia(pontos.at(i), x, y);
					indiceMenorDistancia = i;
				}
			}
		}
		cout << "(distância " << menorDistancia << ") ";
		return pontos.at(indiceMenorDistancia);
	}

	void mostrar_excluido(){
		if(excluido.get_x() == -1){
			return;
		}
		glBegin(GL_LINES);
			glVertex2i(excluido.get_x()-3, excluido.get_y()-3);
			glVertex2i(excluido.get_x()+3, excluido.get_y()+3);
		glEnd();

		glBegin(GL_LINES);
			glVertex2i(excluido.get_x()+3, excluido.get_y()-3);
			glVertex2i(excluido.get_x()-3, excluido.get_y()+3);
		glEnd();
	}
};

Funcoes funcoes; // variável global para as funções do programa

void display() {

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // Set background color to black and opaque
	glClear(GL_COLOR_BUFFER_BIT);         // Clear the color buffer

	glColor3ub(255, 0, 0);
	glBegin(GL_LINES);
	if(funcoes.get_pontos().size() > 1){
		for(unsigned int i = 1; i<funcoes.get_pontos().size(); i++){
			glVertex2i(funcoes.get_pontos().at(i-1).get_x(), funcoes.get_pontos().at(i-1).get_y());
			glVertex2i(funcoes.get_pontos().at(i).get_x(), funcoes.get_pontos().at(i).get_y());
		}
	}
	glEnd();

	glPointSize(5);
	glBegin(GL_POINTS);
		for(unsigned int i = 0; i<funcoes.get_pontos().size(); i++){
			glVertex2i(funcoes.get_pontos().at(i).get_x(), funcoes.get_pontos().at(i).get_y());
		}
	glEnd();

	// algoritmo para mostrar a posição do último ponto excluído
	funcoes.mostrar_excluido();

	glFlush();  // Render now
}

void mouse(int botao, int estado, int x, int y){

	if (estado == GLUT_DOWN){
		if(botao == GLUT_LEFT_BUTTON){
			funcoes.adicionar_ponto(x,y);
		}
		if(botao == GLUT_RIGHT_BUTTON){
			if(funcoes.get_pontos().size() == 0){
				cout << "Não há nenhum ponto para remover." << endl;
				return;
			}
			funcoes.remover_ponto(funcoes.pontoMaisProx(x, y));
		}
	}
}

/* Main function: GLUT runs as a console application starting at main()  */
int main(int argc, char** argv) {
	glutInit(&argc, argv);                 // Initialize GLUT
	glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
	glutInitWindowSize(500, 500);   // Set the window's initial width & height
	glutInitWindowPosition(100, 100); // Position the window's initial top-left corner
	glutCreateWindow("Atividade 2.1"); // Create a window with the given title
	gluOrtho2D(0, 500, 500, 0);
	glutMouseFunc(mouse);
	glutDisplayFunc(display); // Register display callback handler for window re-paint
	glutMainLoop();           // Enter the infinitely event-processing loop
	return 0;
}
