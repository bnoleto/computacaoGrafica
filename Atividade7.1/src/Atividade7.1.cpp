#include <GL/glut.h>  // GLUT, includes glu.h and gl.h
#include <vector>
#include "Ponto.h"
#include "Face.h"
#include <iostream>
#include <fstream>
#include <Math.h>

using namespace std;

class MatrizTransformacao{
	private:

	float tamanho_figura = 1;

	float matriz[4][4];
	//vector<Ponto*>* lista_pontos = nullptr;
	vector<Face*>* lista_faces = nullptr;

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

	void aplicar_transformacao(){
		for(unsigned int j = 0; j<lista_faces->size(); j++){

			vector<Ponto*>* lista_pontos = lista_faces->at(j)->get_pontos();

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

				double matriz_mult[4] = {ponto_atual->get_x(),ponto_atual->get_y(), ponto_atual->get_z(), 1};

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
		}
		reset();
	}

	public:
	Ponto centro_massa = Ponto(250,250,250);
	Ponto* removido = nullptr;

	MatrizTransformacao(vector<Face*>* faces){
		this->lista_faces = faces;
		reset();
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
	}

};

class Funcoes{
	private:

	char eixo_rotacao = 'x';
	//vector<Ponto*> pontos;
	vector<Face*> faces;
	Ponto* excluido = nullptr; //inicial
	MatrizTransformacao matriz = MatrizTransformacao(&faces);

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

	void adicionar_ponto(double x, double y, double z, Face* face){
		face->get_pontos()->push_back(new Ponto(x,y,z));
		cout << "Ponto (" << x << ", " << y << ", " << z << ") adicionado!" << endl;
		excluido = nullptr;
		matriz.atualizar_matriz();
	}

	vector<Face*>* get_faces(){
		return &this->faces;
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
/*
	void salvar_arquivo(){
		fstream arquivo;

		arquivo.open("saida.dat", ios::out);

		for(unsigned int i = 0; i<pontos.size(); i++){
			arquivo << pontos.at(i)->get_x() << " " << pontos.at(i)->get_y() << ((i == pontos.size()-1) ? "" : "\n");
		}

		arquivo.close();

		cout << "> Arquivo saida.dat salvo!" << endl;
	}
*/
	void abrir_arquivo(){
		fstream arquivo;

		vector<Ponto*> lista_pontos;

		string arquivo_caminho = "obj/Peixe.obj";

		arquivo.open(arquivo_caminho, ios::in);
		if(arquivo.is_open()){

			faces.clear();		// vai esvaziar a lista de pontos atual para que sejam criados os pontos a serem importados

			string linha;

			while(getline(arquivo,linha)){

				int j = 0;

				float x, y, z;

				for(unsigned int i = 0; i< linha.length(); i++){
					if(linha.at(i) == ' '){
						switch(linha.at(i-1)){
							case 'v' :
								j=i;
								j++;
								while(linha.at(j) != ' '){
									j++;
								}

								x = stod(linha.substr(i+1, j-1));
								cout << x << " ";

								i=j;
								j++;
								while(linha.at(j) != ' '){
									j++;
								}
								y = stod(linha.substr(i+1, j-1));
								cout << y << " ";

								i=j;

								z = stod(linha.substr(i+1));
								cout << z << endl;

								lista_pontos.push_back(new Ponto(x,y,z));

								break;
							case 'f' :
								int valor[3];
								Face f1 = Face();
								j=i;
								j++;
								while(linha.at(j) != ' '){
									j++;
								}

								valor[0] = (stoi(linha.substr(i+1, j-1))) - 1;
								cout << valor[0] << " ";

								i=j;
								j++;
								while(linha.at(j) != ' '){
									j++;
								}
								valor[1] = (stoi(linha.substr(i+1, j-1))) - 1;
								cout << valor[1] << " ";

								i=j;

								valor[2] = (stoi(linha.substr(i+1))) - 1;
								cout << valor[2] << endl;

								for(int p = 0; p < 3; p++){
									f1.get_pontos()->push_back(lista_pontos.at(valor[p]));
								}

								faces.push_back(&f1);

								break;
						}
					}
				}
			}
			arquivo.close();

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
	glBegin(GL_LINES);
	if(funcoes.get_faces()->size() > 1){
		for(unsigned int i = 1; i<funcoes.get_faces()->size(); i++){
			Face* face_atual = funcoes.get_faces()->at(i);

			for(unsigned int j = 1; j<face_atual->get_pontos()->size(); j++){
				glVertex3d(face_atual->get_pontos()->at(j-1)->get_x(), face_atual->get_pontos()->at(j-1)->get_y(), face_atual->get_pontos()->at(j-1)->get_z());
				glVertex3d(face_atual->get_pontos()->at(j)->get_x(), face_atual->get_pontos()->at(j)->get_y(), face_atual->get_pontos()->at(j)->get_z());
			}
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
