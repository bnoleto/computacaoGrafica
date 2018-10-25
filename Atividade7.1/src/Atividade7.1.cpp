#include <GL/glut.h>  // GLUT, includes glu.h and gl.h
#include <vector>
#include "Ponto.h"
#include "Face.h"
#include <iostream>
#include <fstream>
#include <Math.h>
#include <iomanip>

using namespace std;

class MatrizTransformacao{
	private:

	float tamanho_figura = 1;

	double matriz[4][4];
	vector<Face*>* lista_faces = nullptr;
	char eixo_atual = 'X';

	double rad(double angulo){	// converterá de graus para radianos
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

	void aplicar_transformacao(){
		for(unsigned int j = 0; j<lista_faces->size(); j++){

			vector<Ponto*>* lista_pontos = lista_faces->at(j)->get_pontos();

			for(unsigned int i = 0; i < lista_pontos->size(); i++){	// para cada ponto do conjunto
				Ponto* ponto_atual = lista_pontos->at(i);

				double matriz_mult[4] = {ponto_atual->get_x(),ponto_atual->get_y(), ponto_atual->get_z(), 1};

				matriz_mult[0] -= centro_massa.get_x();
				matriz_mult[1] -= centro_massa.get_y();
				matriz_mult[2] -= centro_massa.get_z();


				double x_final = 0;
				double y_final = 0;
				double z_final = 0;
				for(int i_matriz = 0; i_matriz < 4; i_matriz++){
					x_final+= matriz[0][i_matriz] * matriz_mult[i_matriz];
					y_final+= matriz[1][i_matriz] * matriz_mult[i_matriz];
					z_final+= matriz[2][i_matriz] * matriz_mult[i_matriz];
				}

				x_final += centro_massa.get_x();
				y_final += centro_massa.get_y();
				z_final += centro_massa.get_z();

				ponto_atual->set(x_final, y_final, z_final);

			}
		}
		reset();
	}

	public:
	Ponto centro_massa = Ponto(0,0,0);

	MatrizTransformacao(vector<Face*>* faces){
		this->lista_faces = faces;
		reset();
	}

	void set_eixo(char entrada){
		if (entrada == 'X' || entrada == 'Y' || entrada == 'Z'){
			cout << "O eixo agora é " << entrada << "!" << endl;
			this->eixo_atual = entrada;
		}
	}

	void translacao(double delta_x, double delta_y, double delta_z){
		matriz[0][3] = delta_x;
		matriz[1][3] = delta_y;
		matriz[2][3] = delta_z;
		aplicar_transformacao();
	}

	void escala(double multiplicador){
		if(!(tamanho_figura < 0.01 && multiplicador < 1)){	// condição para impedir de perder os pontos ao deixar a imagem muito pequena
			tamanho_figura *= multiplicador;
			matriz[0][0] = multiplicador;
			matriz[1][1] = multiplicador;
			matriz[2][2] = multiplicador;
			aplicar_transformacao();
		}
	}

	void rotacao(double angulo){
		switch(eixo_atual){
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

			default : break;
		}

		aplicar_transformacao();
	}

	void atualizar_matriz(){
		reset();
	}

};

class Funcoes{
	private:

	char eixo_rotacao = 'X';
	vector<Face*> faces;
	MatrizTransformacao matriz = MatrizTransformacao(&faces);

	public:

	unsigned char point_size = 5;

	MatrizTransformacao* matriz_transformacao(){
		return &this->matriz;
	}

	vector<Face*>* get_faces(){
		return &this->faces;
	}

	double proximo_double(string entrada, unsigned int* contador){

		string saida;

		unsigned int i = *contador;

		while(!(isdigit(entrada.at(i)) || entrada.at(i) == '-')){
			i++;
		}
		while(i<entrada.length()){
			if(isdigit(entrada.at(i)) || entrada.at(i) == '-' || entrada.at(i) == '.'){
				saida+=entrada.at(i);
				i++;
			}else{
				while(i<entrada.length() && entrada.at(i) != ' '){
					i++;
				}

				break;
			}
		}

		*contador = i;

		return stod(saida);
	}

	int proximo_int(string entrada, unsigned int* contador){

		string saida;

		unsigned int i = *contador;

		while(!isdigit(entrada.at(i))){
			i++;
		}
		while(i<entrada.length()){
			if(isdigit(entrada.at(i))){
				saida+=entrada.at(i);
				i++;
			}else{
				while(i<entrada.length() && entrada.at(i) != ' '){
					i++;
				}

				break;
			}
		}

		*contador = i;

		return stoi(saida);
	}

	void abrir_arquivo(){
		fstream arquivo;

		vector<Ponto*> lista_pontos;

		string arquivo_caminho = "obj/Caveira.obj";

		arquivo.open(arquivo_caminho, ios::in);
		if(arquivo.is_open()){

			faces.clear();		// vai esvaziar a lista de pontos atual para que sejam criados os pontos a serem importados

			string linha;

			while(getline(arquivo,linha)){

				int j = 0;

				double x, y, z;

				for(unsigned int i = 0; i< linha.length(); i++){
					if(linha.at(i) == ' '){
						switch(linha.at(i-1)){
							case 'v' :
								j=i;

								x = proximo_double(linha.substr(j), &i);
								y = proximo_double(linha.substr(j), &i);
								z = proximo_double(linha.substr(j), &i);

								lista_pontos.push_back(new Ponto(x,y,z));

								break;
							case 'f' :
								int valor[3];

								j=i;

								Face* f1 = new Face();
								valor[0] = proximo_int(linha.substr(j), &i);
								valor[1] = proximo_int(linha.substr(j), &i);
								valor[2] = proximo_int(linha.substr(j), &i);

								for(int p = 0; p < 3; p++){
									f1->get_pontos()->push_back(lista_pontos.at(valor[p]-1));
								}

								this->faces.push_back(f1);

								break;
						}
					}
				}
			}
			arquivo.close();

			cout << faces.size() << " faces"<< endl;
			cout << faces.size()*3 << " pontos"<< endl;

			cout << "> Arquivo " << arquivo_caminho << " importado!" << endl;
		} else {
			cout << "> Não foi possível abrir o arquivo saida.dat!" << endl;
		}

		matriz = MatrizTransformacao(&faces);

		glutPostRedisplay();
	}
};

Funcoes funcoes; // classe global para as funções do programa

void display() {
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // Set background color to black and opaque

	glClear(GL_COLOR_BUFFER_BIT);         // Clear the color buffer

	glColor3ub(255, 0, 0);
	if(funcoes.get_faces()->size() > 1){
		for(unsigned int i = 0; i<funcoes.get_faces()->size(); i++){
			glBegin(GL_LINE_LOOP);

			Face* face_atual = funcoes.get_faces()->at(i);

			for(unsigned int j = 0; j<face_atual->get_pontos()->size(); j++){
				glVertex3d(face_atual->get_pontos()->at(j)->get_x(), face_atual->get_pontos()->at(j)->get_y(), face_atual->get_pontos()->at(j)->get_z());
			}

			glEnd();
		}
	}

	glutSwapBuffers();
}

void teclado(unsigned char tecla, int x, int y){

	if(toupper(tecla) == 'O'){
		funcoes.abrir_arquivo();
	}

	funcoes.matriz_transformacao()->set_eixo(toupper(tecla));

	switch(tecla){
		case 'e' : funcoes.matriz_transformacao()->escala(0.9); break;
		case 'E' : funcoes.matriz_transformacao()->escala(1.1); break;
		case 'r' : funcoes.matriz_transformacao()->rotacao(-1); break;
		case 'R' : funcoes.matriz_transformacao()->rotacao(1); break;
		default : break;
	}

	glutPostRedisplay();

}

void setas(int tecla, int x, int y){

	switch(tecla){
		case GLUT_KEY_PAGE_UP : funcoes.matriz_transformacao()->translacao(0,0,5); break;
		case GLUT_KEY_PAGE_DOWN : funcoes.matriz_transformacao()->translacao(0,0,-5); break;
		case GLUT_KEY_UP : funcoes.matriz_transformacao()->translacao(0, 5, 0); break;
		case GLUT_KEY_DOWN : funcoes.matriz_transformacao()->translacao(0, -5, 0); break;
		case GLUT_KEY_LEFT : funcoes.matriz_transformacao()->translacao(-5, 0, 0); break;
		case GLUT_KEY_RIGHT : funcoes.matriz_transformacao()->translacao(5, 0, 0); break;
		default : break;
	}

	glutPostRedisplay();

}

/* Main function: GLUT runs as a console application starting at main()  */
int main(int argc, char** argv) {
	glutInit(&argc, argv);                 // Initialize GLUT
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB);
	glutInitWindowSize(500, 500);   // Set the window's initial width & height
	//glViewport(0, 0, 500, 500);
	glutInitWindowPosition(100, 100); // Position the window's initial top-left corner
	glutCreateWindow("Atividade 7.1"); // Create a window with the given title

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	int tamanho = 50;
	glOrtho(-tamanho, tamanho, -tamanho, tamanho, -tamanho, tamanho);
	glMatrixMode(GL_MODELVIEW);

	glutKeyboardFunc(teclado);
	glutSpecialFunc(setas);
	glutDisplayFunc(display); // Register display callback handler for window re-paint
	glutMainLoop();           // Enter the infinitely event-processing loop
	return 0;
}
