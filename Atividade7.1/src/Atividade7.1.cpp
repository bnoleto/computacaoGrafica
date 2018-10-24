#include <GL/glut.h>  // GLUT, includes glu.h and gl.h
#include <vector>
#include "Ponto.h"
#include <iostream>
#include <fstream>
#include <Math.h>

using namespace std;

class MatrizTransformacao{
	private:

	float tamanho_figura = 1;

	float matriz[4][4];
	vector<Ponto*>* lista_pontos = nullptr;

	float rad(float angulo){	// converterá de graus para radianos
		return angulo*3.141592/180;
	}

	void reset(){
		for(int i = 0; i<4; i++){
			for(int j = 0; j<4; j++){
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


		float mediaX = 0, mediaY = 0;

		for(unsigned int i = 0; i<lista_pontos->size(); i++){
			mediaX+=lista_pontos->at(i)->get_x();
			mediaY+=lista_pontos->at(i)->get_y();
		}

		mediaX /= lista_pontos->size();
		mediaY /= lista_pontos->size();

		this->centro_massa = Ponto(mediaX,mediaY);
	}
	void aplicar_transformacao(){
		for(unsigned int i = 0; i <= lista_pontos->size()+1; i++){	// para cada ponto do conjunto
			Ponto* ponto_atual = nullptr;

			if(i == lista_pontos->size()){	// ajustará o centro de massa no final do laço
				ponto_atual = &centro_massa;
			}
			else if(i == lista_pontos->size()+1){	// ajustará o centro de massa no final do laço
				if(removido == nullptr){
					break;
				}
				ponto_atual = removido;
			}
			else{
				ponto_atual = lista_pontos->at(i);
			}

			float matriz_mult[4] = {ponto_atual->get_x(),ponto_atual->get_y(), ponto_atual->get_z(), 1};

			matriz_mult[0] -= centro_massa.get_x();
			matriz_mult[1] -= centro_massa.get_y();
			matriz_mult[2] -= centro_massa.get_z();

			float x_final = 0.0;
			for(int i_matriz = 0; i_matriz < 4; i_matriz++){
				x_final+= matriz[0][i_matriz] * matriz_mult[i_matriz];
			}

			float y_final = 0.0;
			for(int i_matriz = 0; i_matriz < 4; i_matriz++){
				y_final+= matriz[1][i_matriz] * matriz_mult[i_matriz];
			}

			float z_final = 0.0;
			for(int i_matriz = 0; i_matriz < 4; i_matriz++){
				z_final+= matriz[1][i_matriz] * matriz_mult[i_matriz];
			}

			x_final += centro_massa.get_x();
			y_final += centro_massa.get_y();
			z_final += centro_massa.get_z();

			ponto_atual->set(x_final, y_final, z_final);

		}
		reset();
	}

	public:
	Ponto centro_massa = Ponto(250,250,250);
	Ponto* removido = nullptr;

	MatrizTransformacao(vector<Ponto*>* pontos){
		this->lista_pontos = pontos;
		reset();
		calcular_centro_massa();
	}

	void translacao(float delta_x, float delta_y, float delta_z){
		matriz[0][3] = delta_x;
		matriz[1][3] = delta_y;
		matriz[2][3] = delta_z;
		aplicar_transformacao();
	}

	void escala(float multiplicador){
		if(!(tamanho_figura < 0.01 && multiplicador < 1)){	// condição para impedir de perder os pontos ao deixar a imagem muito pequena
			tamanho_figura *= multiplicador;
			matriz[0][0] = multiplicador;
			matriz[1][1] = multiplicador;
			matriz[2][2] = multiplicador;
			aplicar_transformacao();
		}
	}

	void rotacao(float angulo, char eixo){
		switch(toupper(eixo)){
			case 'X' :
				matriz[1][1] = cos(rad(angulo));
				matriz[1][2] = -sin(rad(angulo));
				matriz[2][1] = sin(rad(angulo));
				matriz[2][2] = cos(rad(angulo));
				break;
			case 'Y' :
				matriz[0][0] = cos(rad(angulo));
				matriz[2][0] = -sin(rad(angulo));
				matriz[0][2] = sin(rad(angulo));
				matriz[2][2] = cos(rad(angulo));
				break;
			case 'Z' :
				matriz[0][0] = cos(rad(angulo));
				matriz[0][1] = -sin(rad(angulo));
				matriz[1][0] = sin(rad(angulo));
				matriz[1][1] = cos(rad(angulo));
				break;
			default:
				break;
		}

		aplicar_transformacao();
	}

	void atualizar_matriz(){
		reset();
		calcular_centro_massa();
	}

};

class Funcoes{
	private:

	char eixo_rotacao = 'x';
	vector<Ponto*> pontos;
	Ponto* excluido = nullptr; //inicial
	MatrizTransformacao matriz = MatrizTransformacao(&pontos);

	public:

	unsigned char point_size = 5;

	MatrizTransformacao* matriz_transformacao(){
		return &this->matriz;
	}

	void set_eixo_rotacao(char entrada){
		if (entrada == 'X' || entrada == 'Y' || entrada == 'Z'){
			this->eixo_rotacao = entrada;
		}
	}

	char get_eixo_rotacao(){
		return this->eixo_rotacao;
	}

	void adicionar_ponto(double x, double y, double z){
		pontos.push_back(new Ponto(x,y,z));
		cout << "Ponto (" << x << ", " << y << ", " << z << ") adicionado!" << endl;
		excluido = nullptr;
		matriz.removido = nullptr;
		matriz.atualizar_matriz();
	}

	vector<Ponto*>* get_pontos(){
			return &this->pontos;
		}

	void mostrar_centro(){
		Ponto* excluido = &matriz.centro_massa;

		glColor3ub(0, 255, 255);
		glBegin(GL_LINE_LOOP);
			glVertex2i(excluido->get_x()-2, excluido->get_y());
			glVertex2i(excluido->get_x(), excluido->get_y()+2);
			glVertex2i(excluido->get_x()+2, excluido->get_y());
			glVertex2i(excluido->get_x(), excluido->get_y()-2);
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
				float x, y, z;

				for(unsigned int i = 0; i< linha.length(); i++){
					if(linha.at(i) == ' '){
						x = stod(linha.substr(0, i));		// irá copiar e converter para int a substring dos caracteres na 1ª posição até i
						y = stod(linha.substr(i+1));		// irá copiar e converter para int a substring dos caracteres da i+1 posição até o final
						z = stod(linha.substr(i+1));		// irá copiar e converter para int a substring dos caracteres da i+1 posição até o final
						adicionar_ponto(x, y, z);
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
			glVertex3d(funcoes.get_pontos()->at(i-1)->get_x(), funcoes.get_pontos()->at(i-1)->get_y(),funcoes.get_pontos()->at(i-1)->get_z());
			glVertex3d(funcoes.get_pontos()->at(i)->get_x(), funcoes.get_pontos()->at(i)->get_y(), funcoes.get_pontos()->at(i)->get_z());
		}
	}
	glEnd();
/*
	glPointSize(5);
	glBegin(GL_POINTS);
		for(unsigned int i = 0; i<funcoes.get_pontos()->size(); i++){
			glVertex3d(funcoes.get_pontos()->at(i)->get_x(), funcoes.get_pontos()->at(i)->get_y());
		}
	glEnd();
*/

	funcoes.mostrar_centro();

	glFlush();  // Render now
}

void teclado(unsigned char tecla, int x, int y){

	if(toupper(tecla) == 'O'){
		funcoes.abrir_arquivo();
	}

	funcoes.set_eixo_rotacao(toupper(tecla));

	switch(tecla){
		case 'e' : funcoes.matriz_transformacao()->escala(0.9); break;
		case 'E' : funcoes.matriz_transformacao()->escala(1.1); break;
		case 'r' : funcoes.matriz_transformacao()->rotacao(-5,funcoes.get_eixo_rotacao()); break;
		case 'R' : funcoes.matriz_transformacao()->rotacao(5,funcoes.get_eixo_rotacao()); break;
		default : break;
	}

	glutPostRedisplay();

}

void setas(int tecla, int x, int y){

	switch(tecla){
		case GLUT_KEY_PAGE_UP : funcoes.matriz_transformacao()->translacao(0,0,5); break;
		case GLUT_KEY_PAGE_DOWN : funcoes.matriz_transformacao()->translacao(0,0,-5); break;
		case GLUT_KEY_UP : funcoes.matriz_transformacao()->translacao(0, -5, 0); break;
		case GLUT_KEY_DOWN : funcoes.matriz_transformacao()->translacao(0, 5, 0); break;
		case GLUT_KEY_LEFT : funcoes.matriz_transformacao()->translacao(-5, 0, 0); break;
		case GLUT_KEY_RIGHT : funcoes.matriz_transformacao()->translacao(5, 0, 0); break;
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
	glutCreateWindow("Atividade 7.1"); // Create a window with the given title
	gluOrtho2D(0, 500, 500, 0);
	glutKeyboardFunc(teclado);
	glutSpecialFunc(setas);
	glutDisplayFunc(display); // Register display callback handler for window re-paint
	glutMainLoop();           // Enter the infinitely event-processing loop
	return 0;
}
