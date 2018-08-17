#include <windows.h>  // For MS Windows
#include <GL/glut.h>  // GLUT, includes glu.h and gl.h
#include <vector>
#include <iostream>

using namespace std;

int sizeX=500,sizeY=500;
int pointSize = sizeX*sizeY/5000;
double offsetY = 0;

class Pixel{
	private:
		unsigned short x,y;
		unsigned char r;
		unsigned char g;
		unsigned char b;

	public:
		Pixel(unsigned short x, unsigned short y, unsigned char r,unsigned char g,unsigned char b){
			this->x = x;
			this->y = y;
			this->r = r;
			this->g = g;
			this->b = b;
		}

		void set_posicao(unsigned short x, unsigned short y){
			this->x = x;
			this->y = y;
		}

		void set_cor(unsigned char r,unsigned char g,unsigned char b){
			this->r = r;
			this->g = g;
			this->b = b;
		}

		void imprimir_pixel(){
			double xa = (double)x/sizeX;
			double ya = -(double)y/sizeY;
			glColor3f((double)r/255, (double)g/255, (double)b/255); // Red
			glVertex2d(((xa*2*pointSize)-1)+((double)pointSize/sizeX), ((ya*2*pointSize)+1-((double)pointSize/sizeY))+offsetY);

		}
};

vector<Pixel> pixels;

void inicializar_pixels(){
	pixels.push_back(Pixel(0,0,255,255,255));
	pixels.push_back(Pixel(1,1,255,0,255));
	pixels.push_back(Pixel(5,5,255,0,255));
	pixels.push_back(Pixel(6,5,0,0,255));
	pixels.push_back(Pixel(7,5,0,255,0));
	pixels.push_back(Pixel(6,6,255,255,0));
	pixels.push_back(Pixel(7,6,0,255,255));
}

/* Handler for window-repaint event. Call back when the window first appears and
   whenever the window needs to be re-painted. */
void display() {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color to black and opaque
	glClear(GL_COLOR_BUFFER_BIT);         // Clear the color buffer

	glPointSize(pointSize);
	glBegin(GL_POINTS);

	for(unsigned int i = 0; i<pixels.size(); i++){
		pixels.at(i).imprimir_pixel();
	}
	glEnd();

	glFlush();  // Render now
}

void changeSize(int w, int h) {

	offsetY = (double)-(sizeY-h)/h;
}

/* Main function: GLUT runs as a console application starting at main()  */
int main(int argc, char** argv) {
	inicializar_pixels();

	glutInit(&argc, argv);                 // Initialize GLUT
	glutInitWindowSize(sizeX, sizeY);   // Set the window's initial width & height
	glutInitWindowPosition(100, 100); // Position the window's initial top-left corner
	glutCreateWindow("Atividade 1"); // Create a window with the given title
	glutDisplayFunc(display); // Register display callback handler for window re-paint
	glutReshapeFunc(changeSize);
	glutMainLoop();           // Enter the infinitely event-processing loop
	return 0;
}
