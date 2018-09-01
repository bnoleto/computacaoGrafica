#include <GL/glut.h>  // GLUT, includes glu.h and gl.h
#include <vector>
#include "Ponto.h"
#include <iostream>
#include <Math.h>

using namespace std;

class Funcoes{
	private:

	vector<Ponto*> pontos;
	Ponto* dummy = new Ponto(-10,-10); //ponto dummy para não ocorrer exceção caso não tenha nenhum ponto
	Ponto* excluido = dummy; //inicial
	Ponto* atual = excluido;

	public:

	unsigned char point_size = 5;

	void adicionar_ponto(int x, int y){
		pontos.push_back(new Ponto(x,y));
		cout << "Ponto (" << x << ", " << y << ") adicionado!" << endl;
		excluido = dummy;
		atual = pontos.back();
	}

	void remover_ponto(Ponto ponto){
		short x = ponto.get_x(), y = ponto.get_y();

		for(unsigned int i = pontos.size()-1; i >= 0; i--){		// começará da posição final para priorizar a remoção do ponto criado por último
			if(pontos.at(i)->get_x() == x && pontos.at(i)->get_y() == y){
				excluido = pontos.at(i);
				cout << "Ponto (" << x << ", " << y << ") removido." << endl;
				pontos.erase(pontos.begin() + i);	// iterador do elemento inicial + posição i
				break;
			}
		}
	}

	vector<Ponto*> get_pontos(){
		return this->pontos;
	}

	vector<Ponto*>* get_pontos_ref(){
			return &this->pontos;
		}

	Ponto* get_ponto_atual(){
		return atual;
	}

	void set_ponto_atual(Ponto* ponto){

		atual = ponto;
		cout << "O ponto atual agora é (" << atual->get_x() << ", " << atual->get_y() << ")" << endl;
	}

	float calcularDistancia(Ponto p1, int x, int y){	// fórmula matemática para calcular a distância entre dois pontos
		float xA = (float)p1.get_x();
		float xB = (float)x;
		float yA = (float)p1.get_y();
		float yB = (float)y;

		return sqrt(pow((xB-xA),2)+pow((yB-yA),2));
	}

	Ponto* pontoMaisProx(int x, int y){ // retornará o índice do ponto mais próximo ao clique
		float menorDistancia;
		int indiceMenorDistancia;

		vector<Ponto*>* pontos = get_pontos_ref();

/*		//teste
		string result = (this->pontos == *pontos) ? "*são o msm objeto" : "*não são o mesmo";
		cout << result << endl;
*/
		for(unsigned int i = 0; i< pontos->size(); i++){		// o laço irá comparar as distâncias entre cada ponto do vetor e a posição clicada
			if(i==0){				// ação a ser realizada caso tenha apenas 1 ponto declarado no vetor
				menorDistancia = calcularDistancia(*pontos->at(i), x, y);
				indiceMenorDistancia = i;
			}
			else{					// ação a ser realizada caso tenha 2 ou mais pontos declarados no vetor
				if(calcularDistancia(*pontos->at(i), x, y) < menorDistancia){
					menorDistancia = calcularDistancia(*pontos->at(i), x, y);
					indiceMenorDistancia = i;
				}
			}
		}

		if(pontos->size() == 0){
			return dummy;
		}
		return pontos->at(indiceMenorDistancia);
	}

	bool atual_eh_dummy(){
		return atual == dummy;
	}

	Ponto* get_dummy(){
		return this->dummy;
	}

	short limitar(short valor, short incremento){
		if (valor+incremento < 0 || valor+incremento >= 500){
			return valor;
		}
		return valor + incremento;
	}
};

Funcoes funcoes; // classe global para as funções do programa

void display() {

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // Set background color to black and opaque
	glClear(GL_COLOR_BUFFER_BIT);         // Clear the color buffer

	glColor3ub(255, 0, 0);
	glPointSize(funcoes.point_size);
	glBegin(GL_POINTS);
		for(unsigned int i = 0; i<funcoes.get_pontos().size(); i++){
			if(!(funcoes.get_pontos().at(i) == funcoes.get_ponto_atual())){
				glVertex2i(funcoes.get_pontos().at(i)->get_x(), funcoes.get_pontos().at(i)->get_y());
			}
		}
		glColor3ub(0, 255, 0);
		glVertex2i(funcoes.get_ponto_atual()->get_x(), funcoes.get_ponto_atual()->get_y());

	glEnd();

	glFlush();  // Render now
}

void mouse(int botao, int estado, int x, int y){

	if (estado == GLUT_DOWN){
		if(botao == GLUT_LEFT_BUTTON){
			funcoes.adicionar_ponto(((x + 4)/funcoes.point_size) * funcoes.point_size,
					((y + 4)/funcoes.point_size) * funcoes.point_size);
						// expressão matemática para arredondar os valores para múltiplos de point_size
		}
		if(botao == GLUT_RIGHT_BUTTON){
			if(funcoes.pontoMaisProx(x, y) == funcoes.get_dummy()){	// vai garantir que o ponto dummy não vai ser selecionado
				return;
			}
			funcoes.set_ponto_atual(funcoes.pontoMaisProx(x, y));
		}
	}
}

void teclado(int tecla, int mouseX, int mouseY){
	short x_atual = funcoes.get_ponto_atual()->get_x();
	short y_atual = funcoes.get_ponto_atual()->get_y();

	if(funcoes.atual_eh_dummy()){	// vai garantir que o ponto dummy não vai ser movimentado
		return;
	}

	if(tecla == GLUT_KEY_UP){
		funcoes.get_ponto_atual()->set(x_atual, funcoes.limitar(y_atual,-funcoes.point_size));
	}
	if(tecla == GLUT_KEY_DOWN){
		funcoes.get_ponto_atual()->set(x_atual, funcoes.limitar(y_atual,funcoes.point_size));
	}
	if(tecla == GLUT_KEY_LEFT){
		funcoes.get_ponto_atual()->set(funcoes.limitar(x_atual,-funcoes.point_size), y_atual);
	}
	if(tecla == GLUT_KEY_RIGHT){
		funcoes.get_ponto_atual()->set(funcoes.limitar(x_atual,funcoes.point_size), y_atual);
	}

	glutPostRedisplay();
}

/* Main function: GLUT runs as a console application starting at main()  */
int main(int argc, char** argv) {
	glutInit(&argc, argv);                 // Initialize GLUT
	glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
	glutInitWindowSize(500, 500);   // Set the window's initial width & height
	glutInitWindowPosition(100, 100); // Position the window's initial top-left corner
	glutCreateWindow("Atividade 4.1"); // Create a window with the given title
	gluOrtho2D(0, 500, 500, 0);
	glutMouseFunc(mouse);
	glutSpecialFunc(teclado);
	glutDisplayFunc(display); // Register display callback handler for window re-paint
	glutMainLoop();           // Enter the infinitely event-processing loop
	return 0;
}
