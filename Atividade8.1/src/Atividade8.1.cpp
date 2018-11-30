#include <GL/glut.h>  // GLUT, includes glu.h and gl.h
#include <vector>
#include "MatrizTransformacao.h"
#include "Mesh.h"
#include "Camera.h"
#include <iostream>
#include <fstream>

using namespace std;

class Funcoes{
	private:

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

		if(i<entrada.length()){
			while(i<entrada.length() && !isdigit(entrada.at(i))){
				i++;
			}
			while(i<entrada.length() && isdigit(entrada.at(i))){
				saida+=entrada.at(i);
				i++;
			}
			while(i<entrada.length() && entrada.at(i) != ' '){
				i++;
			}
			*contador = i;

		} else{
			return -1;
		}

		return stoi(saida);
	}

	public:

	Mesh mesh_principal;
	char eixo_rotacao = 'X';
	Camera camera;

	void config_glut(){

			glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB);
			int altura = 720;
			int largura = 480;
			double aspecto = (double)altura/largura;
			glutInitWindowSize(altura, largura);   // Set the window's initial width & height
			glutInitWindowPosition(100, 100); // Position the window's initial top-left corner
			glutCreateWindow("Atividade 8.1"); // Create a window with the given title

			abrir_arquivo("obj/dodge_viper.obj");

			Ponto* p1 = new Ponto(mesh_principal.get_centro_massa()->get_x(),mesh_principal.get_centro_massa()->get_y(),mesh_principal.get_centro_massa()->get_z()+200);

//			camera = new Camera(
//					new Vetor3(p1, new Vetor(p1,1,0,0), new Vetor(p1,0,1,0), new Vetor(p1,0,0,1)),
//					new Ponto(0,0,0),
//					new Vetor(new Ponto(0,10,0), 0,0,0),
//					45,aspecto,0,0);

			camera.set_shape(45, aspecto, 10, 1000);

			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			int tamanho = 100;

			//glOrtho(-tamanho*aspecto, tamanho*aspecto, -tamanho, tamanho, -tamanho*4, tamanho*4);
			gluPerspective(45, aspecto, 0, 1000);

			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			gluLookAt(
					camera.getOlho()->get_x(),
					camera.getOlho()->get_y(),
					camera.getOlho()->get_z(),
					camera.getLook()->get_x(),
					camera.getLook()->get_y(),
					camera.getLook()->get_z(),
					camera.getUp()->get_x(),
					camera.getUp()->get_y(),
					camera.getUp()->get_z()
			);



			camera.setModelViewMatrix();
	}

	void abrir_arquivo(string arquivo_caminho){
		fstream arquivo;

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
								vector<int> valor;

								j=i;

								Face* f1 = new Face();

								while(i < linha.size()){
									int proximo = proximo_int(linha.substr(j), &i);

									if(proximo == -1){
										break;
									} else{
										valor.push_back(proximo);
									}
								}

								for(unsigned int p = 0; p < valor.size(); p++){
									f1->get_pontos()->push_back(lista_pontos->at(valor.at(p)-1));
								}


								lista_faces->push_back(f1);

								break;
						}
					}
				}
			}

			mesh_principal.calcular_centro_massa();

			arquivo.close();

			cout << lista_faces->size() << " faces"<< endl;
			cout << lista_pontos->size() << " pontos"<< endl;

			cout << "> Arquivo " << arquivo_caminho << " importado!" << endl;
		} else {
			cout << "> Não foi possível abrir o arquivo " << arquivo_caminho << endl;
		}

		glutPostRedisplay();
	}
};

Funcoes funcoes; // classe global para as funções do programa

void display() {
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // Set background color to black and opaque

	glClear(GL_COLOR_BUFFER_BIT);         // Clear the color buffer

	funcoes.mesh_principal.imprimir();
//	funcoes.mesh_principal.imprimir_centro_massa();
	funcoes.camera.imprimir_look();

	glutSwapBuffers();
}

void teclado(unsigned char tecla, int x, int y){
	switch(tecla){
		case 'z' : funcoes.camera.slide(0,0,-1); break;
		case 'Z' : funcoes.camera.slide(0,0,1); break;
		case 'p' : funcoes.camera.pitch(-5); break;
		case 'P' : funcoes.camera.pitch(5); break;
		case 'r' : funcoes.camera.roll(-5); break;
		case 'R' : funcoes.camera.roll(5); break;
		case 'y' : funcoes.camera.yaw(-5); break;
		case 'Y' : funcoes.camera.yaw(5); break;
		default: break;
	}

	glutPostRedisplay();

}

/* Main function: GLUT runs as a console application starting at main()  */
int main(int argc, char** argv) {
	glutInit(&argc, argv);                 // Initialize GLUT

	funcoes.config_glut();

	glutKeyboardFunc(teclado);
	glutDisplayFunc(display); // Register display callback handler for window re-paint
	glutMainLoop();           // Enter the infinitely event-processing loop
	return 0;
}
