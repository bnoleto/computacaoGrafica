#include <GL/glut.h>  // GLUT, includes glu.h and gl.h
#include <vector>
#include "Ponto.h"
#include <iostream>
#include <fstream>
#include <Math.h>

using namespace std;

class MatrizTransformacao{
	private:

	float matriz[3][3];
	vector<Ponto*>* lista_pontos = nullptr;
	Ponto centro_massa = Ponto(0,0);

	float rad(float angulo){	// converterá de graus para radianos
		return angulo*3.141592/180;
	}

	void reset(){
		for(int i = 0; i<3; i++){
			for(int j = 0; j<3; j++){
				if(i == j){
					matriz[i][j] = 1.0;
				}
				else{
					matriz[i][j] = 0.0;
				}
			}
		}

	}

	void calcular_centro_massa(){


		float menorX,menorY,maiorX,maiorY;

		if(lista_pontos->size() > 0){
			menorX = lista_pontos->at(0)->get_x();
			menorY = lista_pontos->at(0)->get_y();
			maiorX = lista_pontos->at(0)->get_x();
			maiorY = lista_pontos->at(0)->get_y();
		}
		else{
			return;
		}

		for(unsigned int i = 0; i<lista_pontos->size(); i++){
			if(lista_pontos->at(i)->get_x() > maiorX){
				maiorX = lista_pontos->at(i)->get_x();
			}
			if(lista_pontos->at(i)->get_x() < menorX){
				menorX = lista_pontos->at(i)->get_x();
			}
			if(lista_pontos->at(i)->get_y() > maiorY){
				maiorY = lista_pontos->at(i)->get_y();
			}
			if(lista_pontos->at(i)->get_y() < menorY){
				menorY = lista_pontos->at(i)->get_y();
			}
		}
		this->centro_massa = Ponto((menorX+maiorX/2),(menorY+maiorY/2));
	}

	void aplicar_transformacao(){
		for(unsigned int i = 0; i <= lista_pontos->size(); i++){	// para cada ponto do conjunto
			Ponto* ponto_atual = nullptr;

			if(i ==lista_pontos->size()){	// ajustará o centro de massa no final do laço
				ponto_atual = &centro_massa;
			}
			else{
				ponto_atual = lista_pontos->at(i);
			}

			//cout << ponto_atual->get_x() << endl;

			float matriz_mult[3] = {ponto_atual->get_x(),ponto_atual->get_y(), 1};

			matriz_mult[0] -= centro_massa.get_x();
			matriz_mult[1] -= centro_massa.get_y();

			float x_final = 0.0;
			for(int i_matriz = 0; i_matriz < 3; i_matriz++){
				x_final+= matriz[0][i_matriz] * matriz_mult[i_matriz];
			}

			float y_final = 0.0;
			for(int i_matriz = 0; i_matriz < 3; i_matriz++){
				y_final+= matriz[1][i_matriz] * matriz_mult[i_matriz];
			}

			x_final += centro_massa.get_x();
			y_final += centro_massa.get_y();

			ponto_atual->set(x_final, y_final);

		}
		reset();
	}

	public:

	MatrizTransformacao(vector<Ponto*>* pontos){
		this->lista_pontos = pontos;
		reset();
		calcular_centro_massa();
	}

	void translacao(float delta_x, float delta_y){
		matriz[0][2] = delta_x;
		matriz[1][2] = delta_y;
		aplicar_transformacao();
	}

	void escala(float multiplicador){
		matriz[0][0] = multiplicador;
		matriz[1][1] = multiplicador;
		aplicar_transformacao();
	}

	void rotacao(float angulo){
		matriz[0][0] = cos(rad(angulo));
		matriz[0][1] = -sin(rad(angulo));
		matriz[1][0] = sin(rad(angulo));
		matriz[1][1] = cos(rad(angulo));
		aplicar_transformacao();
	}

	void atualizar_matriz(){
		reset();
		calcular_centro_massa();
	}

};

class Funcoes{
	private:

	vector<Ponto*> pontos;
	Ponto* excluido = nullptr; //inicial
	MatrizTransformacao matriz = MatrizTransformacao(&pontos);

	public:

	unsigned char point_size = 5;

	MatrizTransformacao* matriz_transformacao(){
		return &this->matriz;
	}

	void adicionar_ponto(float x, float y){
		pontos.push_back(new Ponto(x,y));
		cout << "Ponto (" << x << ", " << y << ") adicionado!" << endl;
		excluido = nullptr;
		matriz = MatrizTransformacao(&pontos);
	}

	void remover_ponto(Ponto* ponto){
		if(pontos.size() == 0){
			cout << "Não há nenhum ponto para remover." << endl;
			return;
		}
		//short x = ponto->get_x(), y = ponto->get_y();

		for(unsigned int i = pontos.size()-1; i >= 0; i--){		// começará da posição final para priorizar a remoção do ponto criado por último (caso 2 pontos estejam na mesma posição)
			//if(pontos.at(i)->get_x() == x && pontos.at(i)->get_y() == y){
				excluido = pontos.at(i);
//				cout << "Ponto (" << x << ", " << y << ") removido." << endl;
				pontos.erase(pontos.begin() + i);	// iterador do elemento inicial + posição i
				break;
			//}
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
				float x, y;

				for(unsigned int i = 0; i< linha.length(); i++){
					if(linha.at(i) == ' '){
						x = stof(linha.substr(0, i));		// irá copiar e converter para int a substring dos caracteres na 1ª posição até i
						y = stof(linha.substr(i+1));		// irá copiar e converter para int a substring dos caracteres da i+1 posição até o final
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

		matriz = MatrizTransformacao(&pontos);

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

	switch(tecla){
		case 'e' : funcoes.matriz_transformacao()->escala(0.9); break;
		case 'E' : funcoes.matriz_transformacao()->escala(1.1); break;
		case 'r' : funcoes.matriz_transformacao()->rotacao(-5); break;
		case 'R' : funcoes.matriz_transformacao()->rotacao(5); break;
		default : break;
	}

	glutPostRedisplay();

}

void setas(int tecla, int x, int y){

	switch(tecla){
		case GLUT_KEY_UP : funcoes.matriz_transformacao()->translacao(0, -5); break;
		case GLUT_KEY_DOWN : funcoes.matriz_transformacao()->translacao(0, 5); break;
		case GLUT_KEY_LEFT : funcoes.matriz_transformacao()->translacao(-5, 0); break;
		case GLUT_KEY_RIGHT : funcoes.matriz_transformacao()->translacao(5, 0); break;
		default : break;
	}

	glutPostRedisplay();

}

/* Main function: GLUT runs as a console application starting at main()  */
int main(int argc, char** argv) {
	glutInit(&argc, argv);                 // Initialize GLUT
	glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
	glutInitWindowSize(500, 500);   // Set the window's initial width & height
	glutInitWindowPosition(100, 100); // Position the window's initial top-left corner
	glutCreateWindow("Atividade 6.1"); // Create a window with the given title
	gluOrtho2D(0, 500, 500, 0);
	glutMouseFunc(mouse);
	glutKeyboardFunc(teclado);
	glutSpecialFunc(setas);
	glutDisplayFunc(display); // Register display callback handler for window re-paint
	glutMainLoop();           // Enter the infinitely event-processing loop
	return 0;
}
