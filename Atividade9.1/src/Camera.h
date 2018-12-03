#ifndef CAMERA_H_
#define CAMERA_H_

#include "Vetor3.h"
#include "Ponto.h"
#include <math.h>

class Camera{
	private:

	Ponto *look;
	Ponto *olho;
	Vetor3 *up;
	Vetor3 *u, *v, *n;

	double angulo_visao, aspecto, nearDist, farDist;

	public:

	Camera(Ponto* olho, Ponto* look,Vetor3* up, Vetor3* u,Vetor3* v,Vetor3* n, double angulo, double aspecto, double nearDist, double farDist){
		this->olho = olho;
		this->look = look;
		this->up = up;
		this->u = u;
		this->v = v;
		this->n = n;
		this->angulo_visao = angulo;
		this->aspecto = aspecto;
		this->nearDist = nearDist;
		this->farDist = farDist;
	}

	Ponto* getOlho(){
		return olho;
	}

	Ponto* getLook(){
		return this->look;
	}

	Vetor3* getUp(){
		return up;
	}

	void set_shape(double v_angle, double asp, double nr, double fr){
		angulo_visao = v_angle;
		aspecto = asp;
		nearDist = nr;
		farDist = fr;

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(angulo_visao, aspecto, nearDist, farDist);
		glMatrixMode(GL_MODELVIEW);
	}

	void atualizarCamera(){
		gluLookAt(
				olho->get_x(),
				olho->get_y(),
				olho->get_z(),
				look->get_x(),
				look->get_y(),
				look->get_z(),
				up->get_x(),
				up->get_y(),
				up->get_z()
		);
	}

	void set(){

		n = new Vetor3(
				olho->get_x() - look->get_x(),
				olho->get_y() - look->get_y(),
				olho->get_z() - look->get_z());
		u = up->produtoVetorial(n);

		n->normalizar();
		u->normalizar();

		v = n->produtoVetorial(u);
	}

	void setModelViewMatrix(){

		Vetor3 *eVec = new Vetor3(olho);

		double m[16];

	    m[0] = u->get_x(); m[4] = u->get_y(); m[8] = u->get_z(); m[12] = -eVec->dot(u);
	    m[1] = v->get_x(); m[5] = v->get_y(); m[9] = v->get_z(); m[13] = -eVec->dot(v);
	    m[2] = n->get_x(); m[6] = n->get_y(); m[10] = n->get_z(); m[14] = -eVec->dot(n);
	    m[3] = 0; m[7] = 0; m[11] = 0; m[15] = 1.0;
	    glMatrixMode(GL_MODELVIEW);
	    glLoadMatrixd(m);
	}

	void slide(double delU, double delV, double delN){

		olho->set(
				olho->get_x() + delU * u->get_x() + delV * v->get_x() + delN * n->get_x(),
				olho->get_y() + delU * u->get_y() + delV * v->get_y() + delN * n->get_y(),
				olho->get_z() + delU * u->get_z() + delV * v->get_z() + delN * n->get_z()
				);

		setModelViewMatrix();
	}

	void roll(double angulo){

		double pi = 3.14159265;

		double cs = cos(pi/180.0*angulo);
		double sn = sin(pi/180.0*angulo);

		Vetor3 t = u->clone();

		u->set(
			cs*t.get_x() - sn*v->get_x(),
			cs*t.get_y() - sn*v->get_y(),
			cs*t.get_z() - sn*v->get_z()
		);

		v->set(
			sn*t.get_x() + cs*v->get_x(),
			sn*t.get_y() + cs*v->get_y(),
			sn*t.get_z() + cs*v->get_z()
		);

		setModelViewMatrix();
	}

	void pitch(double angulo){

		double pi = 3.14159265;

		double cs = cos(pi/180.0*angulo);
		double sn = sin(pi/180.0*angulo);

		Vetor3 t = v->clone();

		v->set(
			cs*t.get_x() - sn*n->get_x(),
			cs*t.get_y() - sn*n->get_y(),
			cs*t.get_z() - sn*n->get_z()
		);

		n->set(
			sn*t.get_x() + cs*n->get_x(),
			sn*t.get_y() + cs*n->get_y(),
			sn*t.get_z() + cs*n->get_z()
		);

		setModelViewMatrix();
	}

	void yaw(double angulo){

		double pi = 3.14159265;

		double cs = cos(pi/180.0*angulo);
		double sn = sin(pi/180.0*angulo);

		Vetor3 t = n->clone();

		n->set(
			cs*t.get_x() - sn*u->get_x(),
			cs*t.get_y() - sn*u->get_y(),
			cs*t.get_z() - sn*u->get_z()
		);

		u->set(
			sn*t.get_x() + cs*u->get_x(),
			sn*t.get_y() + cs*u->get_y(),
			sn*t.get_z() + cs*u->get_z()
		);

		setModelViewMatrix();
	}

	void imprimir_look(){


		glColor3ub(255, 0, 0);
		glBegin(GL_QUADS);

			glVertex3d( look->get_x(), look->get_y()+0.5,look->get_z()+0.5);
			glVertex3d( look->get_x(), look->get_y()-0.5,look->get_z()-0.5);
			glVertex3d( look->get_x(), look->get_y()-0.5,look->get_z()-0.5);
			glVertex3d( look->get_x(), look->get_y()+0.5,look->get_z()+0.5);

			glVertex3d( look->get_x()+8, look->get_y()+0.5,look->get_z()+0.5);
			glVertex3d( look->get_x()+8, look->get_y()-0.5,look->get_z()-0.5);
			glVertex3d( look->get_x()+8, look->get_y()-0.5,look->get_z()-0.5);
			glVertex3d( look->get_x()+8, look->get_y()+0.5,look->get_z()+0.5);

			glVertex3d( look->get_x(), look->get_y()+0.5,look->get_z()-0.5);
			glVertex3d( look->get_x()+8, look->get_y()+0.5,look->get_z()-0.5);
			glVertex3d( look->get_x()+8, look->get_y()-0.5,look->get_z()-0.5);
			glVertex3d( look->get_x(), look->get_y()-0.5,look->get_z()-0.5);

			glVertex3d( look->get_x(), look->get_y()+0.5,look->get_z()+0.5);
			glVertex3d( look->get_x()+8, look->get_y()+0.5,look->get_z()+0.5);
			glVertex3d( look->get_x()+8, look->get_y()-0.5,look->get_z()+0.5);
			glVertex3d( look->get_x(), look->get_y()-0.5,look->get_z()+0.5);

			glVertex3d( look->get_x(), look->get_y()+0.5,look->get_z()-0.5);
			glVertex3d( look->get_x()+8, look->get_y()+0.5,look->get_z()-0.5);
			glVertex3d( look->get_x()+8, look->get_y()+0.5,look->get_z()+0.5);
			glVertex3d( look->get_x(), look->get_y()+0.5,look->get_z()+0.5);

			glVertex3d( look->get_x(), look->get_y()-0.5,look->get_z()-0.5);
			glVertex3d( look->get_x()+8, look->get_y()-0.5,look->get_z()-0.5);
			glVertex3d( look->get_x()+8, look->get_y()-0.5,look->get_z()+0.5);
			glVertex3d( look->get_x(), look->get_y()-0.5,look->get_z()+0.5);

		glEnd();

		glColor3ub(0, 255, 0);
		glBegin(GL_QUADS);
			glVertex3d( look->get_x()+0.5,look->get_y(),   look->get_z()+0.5);
			glVertex3d( look->get_x()-0.5,look->get_y(),   look->get_z()-0.5);
			glVertex3d( look->get_x()-0.5,look->get_y(),   look->get_z()-0.5);
			glVertex3d( look->get_x()+0.5,look->get_y(),   look->get_z()+0.5);

			glVertex3d( look->get_x()+0.5,look->get_y()+8, look->get_z()+0.5);
			glVertex3d( look->get_x()-0.5,look->get_y()+8, look->get_z()-0.5);
			glVertex3d( look->get_x()-0.5,look->get_y()+8, look->get_z()-0.5);
			glVertex3d( look->get_x()+0.5,look->get_y()+8, look->get_z()+0.5);

			glVertex3d( look->get_x()+0.5,look->get_y(),   look->get_z()-0.5);
			glVertex3d( look->get_x()+0.5,look->get_y()+8, look->get_z()-0.5);
			glVertex3d( look->get_x()-0.5,look->get_y()+8, look->get_z()-0.5);
			glVertex3d( look->get_x()-0.5,look->get_y(),   look->get_z()-0.5);

			glVertex3d( look->get_x()+0.5,look->get_y(),   look->get_z()+0.5);
			glVertex3d( look->get_x()+0.5,look->get_y()+8, look->get_z()+0.5);
			glVertex3d( look->get_x()-0.5,look->get_y()+8, look->get_z()+0.5);
			glVertex3d( look->get_x()-0.5,look->get_y(),   look->get_z()+0.5);

			glVertex3d( look->get_x()+0.5,look->get_y(),   look->get_z()-0.5);
			glVertex3d( look->get_x()+0.5,look->get_y()+8, look->get_z()-0.5);
			glVertex3d( look->get_x()+0.5,look->get_y()+8, look->get_z()+0.5);
			glVertex3d( look->get_x()+0.5,look->get_y(),   look->get_z()+0.5);

			glVertex3d( look->get_x()-0.5,look->get_y(),   look->get_z()-0.5);
			glVertex3d( look->get_x()-0.5,look->get_y()+8, look->get_z()-0.5);
			glVertex3d( look->get_x()-0.5,look->get_y()+8, look->get_z()+0.5);
			glVertex3d( look->get_x()-0.5,look->get_y(),   look->get_z()+0.5);
		glEnd();

		glColor3ub(50, 50, 255);
		glBegin(GL_QUADS);
			glVertex3d( look->get_x()+0.5,look->get_y()+0.5, look->get_z());
			glVertex3d( look->get_x()-0.5,look->get_y()-0.5, look->get_z());
			glVertex3d( look->get_x()-0.5,look->get_y()-0.5, look->get_z());
			glVertex3d( look->get_x()+0.5,look->get_y()+0.5, look->get_z());

			glVertex3d( look->get_x()+0.5,look->get_y()+0.5, look->get_z()+8);
			glVertex3d( look->get_x()-0.5,look->get_y()-0.5, look->get_z()+8);
			glVertex3d( look->get_x()-0.5,look->get_y()-0.5, look->get_z()+8);
			glVertex3d( look->get_x()+0.5,look->get_y()+0.5, look->get_z()+8);

			glVertex3d( look->get_x()+0.5,look->get_y()-0.5, look->get_z());
			glVertex3d( look->get_x()+0.5,look->get_y()-0.5, look->get_z()+8);
			glVertex3d( look->get_x()-0.5,look->get_y()-0.5, look->get_z()+8);
			glVertex3d( look->get_x()-0.5,look->get_y()-0.5, look->get_z());

			glVertex3d( look->get_x()+0.5,look->get_y()+0.5, look->get_z());
			glVertex3d( look->get_x()+0.5,look->get_y()+0.5, look->get_z()+8);
			glVertex3d( look->get_x()-0.5,look->get_y()+0.5, look->get_z()+8);
			glVertex3d( look->get_x()-0.5,look->get_y()+0.5, look->get_z());

			glVertex3d( look->get_x()+0.5,look->get_y()-0.5, look->get_z());
			glVertex3d( look->get_x()+0.5,look->get_y()-0.5, look->get_z()+8);
			glVertex3d( look->get_x()+0.5,look->get_y()+0.5, look->get_z()+8);
			glVertex3d( look->get_x()+0.5,look->get_y()+0.5, look->get_z());

			glVertex3d( look->get_x()-0.5,look->get_y()-0.5, look->get_z());
			glVertex3d( look->get_x()-0.5,look->get_y()-0.5, look->get_z()+8);
			glVertex3d( look->get_x()-0.5,look->get_y()+0.5, look->get_z()+8);
			glVertex3d( look->get_x()-0.5,look->get_y()+0.5, look->get_z());
		glEnd();
	}
};

#endif /* CAMERA_H_ */

