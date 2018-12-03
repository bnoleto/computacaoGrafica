#include <GL/glut.h>  // GLUT, includes glu.h and gl.h
#include "Camera.h"
#include "Ponto.h"
#include "Objetos.h"

using namespace std;

class Funcoes{
	public:

	Camera* camera = nullptr;
	int x_inicial, y_inicial;
	int botao_mouse = 0;
	double sens_camera = 0.5;
	Objetos objetos = Objetos();

	void config_glut(){

			glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB);
			int altura = 480;
			int largura = 720;
			double aspecto = (double)largura/altura;
			glutInitWindowSize(largura, altura);   // Set the window's initial width & height
			glutInitWindowPosition(100, 100); // Position the window's initial top-left corner
			glutCreateWindow("Atividade 9.1"); // Create a window with the given title

			camera = new Camera(
					new Ponto(0,0,200),	 // eye
					new Ponto(0,0,0),	 // look
					new Vetor3(0,0,1),	 // up
					new Vetor3(1,0,0),	 // u
					new Vetor3(0,1,0),	 // v
					new Vetor3(0,0,1),	 // n
					45,aspecto,10,1000); // outros params

			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();

			gluPerspective(45, aspecto, 0, 1000);

			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			gluLookAt(
					camera->getOlho()->get_x(),
					camera->getOlho()->get_y(),
					camera->getOlho()->get_z(),
					camera->getLook()->get_x(),
					camera->getLook()->get_y(),
					camera->getLook()->get_z(),
					camera->getUp()->get_x(),
					camera->getUp()->get_y(),
					camera->getUp()->get_z()
			);

			camera->setModelViewMatrix();
	}
};

Funcoes funcoes; // classe global para as funções do programa

void display() {
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // Set background color to black and opaque

	glClear(GL_COLOR_BUFFER_BIT);         // Clear the color buffer

	glColor3ub(255,255,255);

	funcoes.objetos.desenhar_letra_t();
	funcoes.objetos.desenhar_func_matematica();

	glutSwapBuffers();
}

void teclado(unsigned char tecla, int x, int y){
	switch(tecla){
		case 'z' : funcoes.camera->slide(0,0,-5); break;
		case 'Z' : funcoes.camera->slide(0,0,5); break;
		case 'p' : funcoes.camera->pitch(-5); break;
		case 'P' : funcoes.camera->pitch(5); break;
		case 'r' : funcoes.camera->roll(-5); break;
		case 'R' : funcoes.camera->roll(5); break;
		case 'y' : funcoes.camera->yaw(-5); break;
		case 'Y' : funcoes.camera->yaw(5); break;
		default: break;
	}

	glutPostRedisplay();

}

void mouse(int button, int state, int x, int y) {

	funcoes.botao_mouse = button;

	funcoes.x_inicial = x;
	funcoes.y_inicial = y;
}

void motion(int x, int y) {



	//cout << funcoes.x_inicial << " / " << funcoes.y_inicial << endl;

	if(funcoes.botao_mouse == 2){
		if(y < funcoes.y_inicial){
			funcoes.camera->pitch(-funcoes.sens_camera);
		} else if(y > funcoes.y_inicial){
			funcoes.camera->pitch(funcoes.sens_camera);
		}
	} else{
		if(y < funcoes.y_inicial){
			funcoes.camera->slide(0,0,-funcoes.sens_camera*2);
		} else if(y > funcoes.y_inicial){
			funcoes.camera->slide(0,0,funcoes.sens_camera*2);
		}
	}

	if(x < funcoes.x_inicial){
		funcoes.camera->yaw(-funcoes.sens_camera);
	} else if(x > funcoes.x_inicial){
		funcoes.camera->yaw(funcoes.sens_camera);
	}

	funcoes.x_inicial = x;
	funcoes.y_inicial = y;

	glutPostRedisplay();
}

/* Main function: GLUT runs as a console application starting at main()  */
int main(int argc, char** argv) {
	glutInit(&argc, argv);                 // Initialize GLUT

	funcoes.config_glut();

	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutKeyboardFunc(teclado);
	glutDisplayFunc(display); // Register display callback handler for window re-paint
	glutMainLoop();           // Enter the infinitely event-processing loop
	return 0;
}
