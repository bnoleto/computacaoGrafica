#include <GL/glut.h>  // GLUT, includes glu.h and gl.h
#include <vector>
#include "Ponto.h"
#include "Face.h"
#include "MatrizTransformacao.h"
#include "Mesh.h"
#include <iostream>
#include <fstream>
#include <iomanip>

using namespace std;

class Funcoes{
	public:

	Mesh mesh_principal;
	char eixo_rotacao = 'X';

//	unsigned char point_size = 5;
/*
	MatrizTransformacao* matriz_transformacao(){
		return &this->matriz;
	}

	vector<Face*>* get_faces(){
		return &this->lista_faces;
	}
*/
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

		string arquivo_caminho = "obj/dodge_viper.obj";

		arquivo.open(arquivo_caminho, ios::in);
		if(arquivo.is_open()){

			mesh_principal.reset();

			vector<Ponto*>* lista_pontos = mesh_principal.get_lista_pontos();
			vector<Face*>* lista_faces = mesh_principal.get_lista_faces();

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

								lista_pontos->push_back(new Ponto(x,y,z));

								break;
							case 'f' :
								int valor[3];

								j=i;

								Face* f1 = new Face();
								valor[0] = proximo_int(linha.substr(j), &i);
								valor[1] = proximo_int(linha.substr(j), &i);
								valor[2] = proximo_int(linha.substr(j), &i);

								for(int p = 0; p < 3; p++){
									f1->get_pontos()->push_back(lista_pontos->at(valor[p]-1));
								}

								lista_faces->push_back(f1);

								break;
						}
					}
				}
			}
			arquivo.close();

			cout << lista_faces->size() << " faces"<< endl;
			cout << lista_faces->size()*3 << " pontos"<< endl;

			cout << "> Arquivo " << arquivo_caminho << " importado!" << endl;
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

	Mesh* mesh_atual = &funcoes.mesh_principal;

	glColor3ub(255, 0, 0);
	if(mesh_atual->get_lista_faces()->size() > 1){
		for(unsigned int i = 0; i<mesh_atual->get_lista_faces()->size(); i++){
			Face* face_atual = mesh_atual->get_lista_faces()->at(i);

			glBegin(GL_LINE_STRIP);

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

	MatrizTransformacao* matriz = funcoes.mesh_principal.get_matriz_transformacao();

	matriz->set_eixo(toupper(tecla));

	switch(tecla){
		case 'e' : matriz->escala(0.9); break;
		case 'E' : matriz->escala(1.1); break;
		case 'r' : matriz->rotacao(-1); break;
		case 'R' : matriz->rotacao(1); break;
	}

	glutPostRedisplay();

}

void setas(int tecla, int x, int y){

	MatrizTransformacao* matriz = funcoes.mesh_principal.get_matriz_transformacao();

	switch(tecla){
		case GLUT_KEY_PAGE_UP : matriz->translacao(0,0,5); break;
		case GLUT_KEY_PAGE_DOWN : matriz->translacao(0,0,-5); break;
		case GLUT_KEY_UP : matriz->translacao(0, 5, 0); break;
		case GLUT_KEY_DOWN : matriz->translacao(0, -5, 0); break;
		case GLUT_KEY_LEFT : matriz->translacao(-5, 0, 0); break;
		case GLUT_KEY_RIGHT : matriz->translacao(5, 0, 0); break;
		default : break;
	}

	glutPostRedisplay();

}

/* Main function: GLUT runs as a console application starting at main()  */
int main(int argc, char** argv) {
	glutInit(&argc, argv);                 // Initialize GLUT
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB);

	int altura = 720;
	int largura = 480;
	double aspecto = (double)altura/largura;
	glutInitWindowSize(altura, largura);   // Set the window's initial width & height
	glutInitWindowPosition(100, 100); // Position the window's initial top-left corner
	glutCreateWindow("Atividade 7.1"); // Create a window with the given title

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	int tamanho = 100;

	glOrtho(-tamanho*aspecto, tamanho*aspecto, -tamanho, tamanho, -tamanho*4, tamanho*4);

	glutKeyboardFunc(teclado);
	glutSpecialFunc(setas);
	glutDisplayFunc(display); // Register display callback handler for window re-paint
	glutMainLoop();           // Enter the infinitely event-processing loop
	return 0;
}
