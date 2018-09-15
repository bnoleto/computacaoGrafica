#include <GL/glut.h>  // GLUT, includes glu.h and gl.h
#include <vector>
#include "Ponto.h"
#include <fstream>
#include <Math.h>

using namespace std;

class Funcoes{
	private:

	vector<vector<Ponto*>*> nucleos;

	public:

	void adicionar_ponto_nucleo(vector<Ponto*>* nucleo,int x, int y){
		nucleo->push_back(new Ponto(x,y));
		cout << "Ponto (" << x << ", " << y << ") adicionado! [" << nucleo->size() << "]" << endl;
	}

	vector<vector<Ponto*>*>* get_nucleos(){
		return &this->nucleos;
	}

	void abrir_arquivo(){
		fstream arquivo;


		arquivo.open("dino.dat", ios::in);
		if(arquivo.is_open()){

			string str_nucleos;
			getline(arquivo,str_nucleos);
			int qtd_nucleos = stoi(str_nucleos);

			for(int nucleo = 0; nucleo < qtd_nucleos; nucleo++){
				vector<Ponto*>* novo = new vector<Ponto*>;
				nucleos.push_back(novo);
				string str_linhas;
				getline(arquivo,str_nucleos);
				int qtd_linhas = stoi(str_nucleos);

				cout << "+ Núcleo " << nucleo+1 << " (" << qtd_linhas <<" pontos)" << "\n";
				for(int linha_atual = 0; linha_atual < qtd_linhas; linha_atual++){
					string linha;
					getline(arquivo,linha);

					int x, y;

					for(unsigned int i = 0; i< linha.length(); i++){
						if(linha.at(i) == ' '){
							x = stoi(linha.substr(0, i));		// irá copiar e converter para int a substring dos caracteres na 1ª posição até i
							y = stoi(linha.substr(i+1));		// irá copiar e converter para int a substring dos caracteres da i+1 posição até o final
							adicionar_ponto_nucleo(novo,x, y);
							break;
						}
					}
				}
			}

			arquivo.close();

			cout << "> Arquivo dino.dat importado!" << endl;
		} else {
			cout << "> Não foi possível abrir o arquivo dino.dat!" << endl;
		}

		glutPostRedisplay();
	}
};

Funcoes funcoes; // classe global para as funções do programa

void display() {

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // Set background color to black and opaque
	glClear(GL_COLOR_BUFFER_BIT);         // Clear the color buffer

	glColor3ub(255, 0, 0);

	for(unsigned int i = 0; i<funcoes.get_nucleos()->size(); i++){

			glBegin(GL_LINE_STRIP);
				for(unsigned int j = 0; j < funcoes.get_nucleos()->at(i)->size(); j++){
					glVertex2i(funcoes.get_nucleos()->at(i)->at(j)->get_x(), funcoes.get_nucleos()->at(i)->at(j)->get_y());
				}
			glEnd();


	}
	glFlush();  // Render now
}

void teclado(unsigned char tecla, int x, int y){

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
	glutCreateWindow("Atividade 4.2"); // Create a window with the given title
	gluOrtho2D(0, 650, 0, 650);
	glutKeyboardFunc(teclado);
	glutDisplayFunc(display); // Register display callback handler for window re-paint
	glutMainLoop();           // Enter the infinitely event-processing loop
	return 0;
}
