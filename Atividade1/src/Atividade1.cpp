#include <windows.h>  // For MS Windows
#include <GL/glut.h>  // GLUT, includes glu.h and gl.h
#include <vector>
#include <iostream>

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

		Pixel(){

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

/* Handler for window-repaint event. Call back when the window first appears and
   whenever the window needs to be re-painted. */
void display() {
   glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color to black and opaque
   glClear(GL_COLOR_BUFFER_BIT);         // Clear the color buffer

/* // Draw a Red 1x1 Square centered at origin
 * glBegin(GL_QUADS);              // Each set of 4 vertices form a quad
 *    glColor3f(1.0f, 0.0f, 0.0f); // Red
 *    glVertex2f(-0.5f, -0.5f);    // x, y
 *    glVertex2f( 0.5f, -0.5f);
 *    glVertex2f( 0.5f,  0.5f);
 *    glVertex2f(-0.5f,  0.5f);
 * glEnd();
 */

   Pixel *p0 = new Pixel(0,0,255,255,255);
   Pixel *p01 = new Pixel(1,1,255,0,255);
   Pixel *p1 = new Pixel(5,5,255,0,255);
   Pixel *p2 = new Pixel(6,5,0,0,255);
   Pixel *p3 = new Pixel(7,5,0,255,0);
   Pixel *p4 = new Pixel(6,6,255,255,0);
   Pixel *p5 = new Pixel(7,6,0,255,255);

   glPointSize(pointSize);
   glBegin(GL_POINTS);
   	p0->imprimir_pixel();
   	p01->imprimir_pixel();
	p1->imprimir_pixel();
	p2->imprimir_pixel();
	p3->imprimir_pixel();
	p4->imprimir_pixel();
	p5->imprimir_pixel();
   glEnd();

   glFlush();  // Render now
}

void changeSize(int w, int h) {

	offsetY = (double)-(sizeY-h)/h;
}

/* Main function: GLUT runs as a console application starting at main()  */
int main(int argc, char** argv) {
   glutInit(&argc, argv);                 // Initialize GLUT
   glutInitWindowSize(sizeX, sizeY);   // Set the window's initial width & height
   glutInitWindowPosition(100, 100); // Position the window's initial top-left corner
   glutCreateWindow("Atividade 1"); // Create a window with the given title
   glutDisplayFunc(display); // Register display callback handler for window re-paint
   glutReshapeFunc(changeSize);
   glutMainLoop();           // Enter the infinitely event-processing loop
   return 0;
}
