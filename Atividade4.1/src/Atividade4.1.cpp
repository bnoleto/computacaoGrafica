#include <GL/glut.h>  // GLUT, includes glu.h and gl.h
#include <vector>
#include "Ponto.h"
#include <iostream>
#include <fstream>
#include <Math.h>

using namespace std;

class Funcoes{
	private:

	vector<Ponto*> pontos;
	Ponto* excluido = NULL; //inicial

	public:

	unsigned char point_size = 5;

	void adicionar_ponto(int x, int y){
		pontos.push_back(new Ponto(x,y));
		cout << "Ponto (" << x << ", " << y << ") adicionado!" << endl;
		excluido = NULL;
	}

	void remover_ponto(Ponto* ponto){
		if(pontos.size() == 0){
			cout << "Não há nenhum ponto para remover." << endl;
			return;
		}
		short x = ponto->get_x(), y = ponto->get_y();

		for(unsigned int i = pontos.size()-1; i >= 0; i--){		// começará da posição final para priorizar a remoção do ponto criado por último (caso 2 pontos estejam na mesma posição)
			if(pontos.at(i)->get_x() == x && pontos.at(i)->get_y() == y){
				excluido = pontos.at(i);
				cout << "Ponto (" << x << ", " << y << ") removido." << endl;
				pontos.erase(pontos.begin() + i);	// iterador do elemento inicial + posição i
				break;
			}
		}
	}

	vector<Ponto*>* get_pontos(){
			return &this->pontos;
		}

	float calcularDistancia(Ponto* p1, int x, int y){	// fórmula matemática para calcular a distância entre dois pontos
		float xA = (float)p1->get_x();
		float xB = (float)x;
		float yA = (float)p1->get_y();
		float yB = (float)y;

		return sqrt(pow((xB-xA),2)+pow((yB-yA),2));
	}

	Ponto* pontoMaisProx(int x, int y){ // retornará o índice do ponto mais próximo ao clique
		if(pontos.size() == 0){
			return NULL;
		}

		Ponto* ponto_mais_proximo = pontos.at(0); // inicial

		for(unsigned int i = 0; i< pontos.size(); i++){		// o laço irá comparar as distâncias entre cada ponto do vetor e a posição clicada
			if(calcularDistancia(pontos.at(i), x, y) < calcularDistancia(ponto_mais_proximo, x, y)){
				ponto_mais_proximo = pontos.at(i);
			}
		}
		return ponto_mais_proximo;
	}

	void mostrar_excluido(){
			if(excluido == NULL){
				return;
			}
			glColor3ub(0, 255, 0);
			glBegin(GL_LINES);
				glVertex2i(excluido->get_x()-3, excluido->get_y()-3);
				glVertex2i(excluido->get_x()+3, excluido->get_y()+3);
				glVertex2i(excluido->get_x()+3, excluido->get_y()-3);
				glVertex2i(excluido->get_x()-3, excluido->get_y()+3);
			glEnd();
		}

	void salvar_arquivo(){
		fstream arquivo;

		arquivo.open("saida.dat", ios::out);

		for(unsigned int i = 0; i<pontos.size(); i++){
			arquivo << pontos.at(i)->get_x() << " " << pontos.at(i)->get_y() << ((i == pontos.size()-1) ? "" : "\n");
		}

		arquivo.close();

		cout << "> Arquivo saida.dat salvo!" << endl;
	}

	void abrir_arquivo(){
		fstream arquivo;


		arquivo.open("saida.dat", ios::in);
		if(arquivo.is_open()){

			pontos.clear();		// vai esvaziar a lista de pontos atual para que sejam criados os pontos a serem importados

			string linha;

			while(getline(arquivo,linha)){
				int x, y;

				for(unsigned int i = 0; i< linha.length(); i++){
					if(linha.at(i) == ' '){
						x = stoi(linha.substr(0, i));		// irá copiar e converter para int a substring dos caracteres na 1ª posição até i
						y = stoi(linha.substr(i+1));		// irá copiar e converter para int a substring dos caracteres da i+1 posição até o final
						adicionar_ponto(x, y);
						break;
					}
				}
			}
			arquivo.close();

			cout << "> Arquivo saida.dat importado!" << endl;
		} else {
			cout << "> Não foi possível abrir o arquivo saida.dat!" << endl;
		}

		glutPostRedisplay();
	}
};

Funcoes funcoes; // classe global para as funções do programa

void display() {

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // Set background color to black and opaque
	glClear(GL_COLOR_BUFFER_BIT);         // Clear the color buffer

	glColor3ub(255, 0, 0);
	glBegin(GL_LINES);
	if(funcoes.get_pontos()->size() > 1){
		for(unsigned int i = 1; i<funcoes.get_pontos()->size(); i++){
			glVertex2i(funcoes.get_pontos()->at(i-1)->get_x(), funcoes.get_pontos()->at(i-1)->get_y());
			glVertex2i(funcoes.get_pontos()->at(i)->get_x(), funcoes.get_pontos()->at(i)->get_y());
		}
	}
	glEnd();

	glPointSize(5);
	glBegin(GL_POINTS);
		for(unsigned int i = 0; i<funcoes.get_pontos()->size(); i++){
			glVertex2i(funcoes.get_pontos()->at(i)->get_x(), funcoes.get_pontos()->at(i)->get_y());
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
				funcoes.remover_ponto(funcoes.pontoMaisProx(x, y));
			}
		}
}

void teclado(unsigned char tecla, int x, int y){

	if(toupper(tecla) == 'S'){
		funcoes.salvar_arquivo();
	}
	if(toupper(tecla) == 'O'){
		funcoes.abrir_arquivo();
	}

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
	glutKeyboardFunc(teclado);
	glutDisplayFunc(display); // Register display callback handler for window re-paint
	glutMainLoop();           // Enter the infinitely event-processing loop
	return 0;
}
