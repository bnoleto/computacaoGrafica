#ifndef CAMERA_H_
#define CAMERA_H_

#include "Vetor3.h"

class Camera{
	private:

	Ponto *look;
	Vetor3 *olho;
	Vetor *up;

	double angulo_visao, aspecto, nearDist, farDist;

	public:

	Camera(Vetor3* olho, Ponto* look,Vetor* up, double angulo, double aspecto, double nearDist, double farDist){
		this->olho = olho;
		this->look = look;
		this->up = up;
		this->angulo_visao = angulo;
		this->aspecto = aspecto;
		this->nearDist = nearDist;
		this->farDist = farDist;
	}

	Ponto* getOlho(){
		return olho->get_origem();
	}

	Ponto* getLook(){
		return this->look;
	}

	Ponto* getUp(){
		return this->up->get_origem();
	}

	void atualizarCamera(){
		gluLookAt(
				olho->get_origem()->get_x(),
				olho->get_origem()->get_y(),
				olho->get_origem()->get_z(),
				look->get_x(),
				look->get_y(),
				look->get_z(),
				up->get_x(),
				up->get_y(),
				up->get_z()
		);
	}

	void set(){
		Vetor *u = olho->get_u();
		Vetor *v = olho->get_v();
		Vetor *n = olho->get_n();

		n = new Vetor(olho->get_origem(),
				olho->get_origem()->get_x() - look->get_x(),
				olho->get_origem()->get_y() - look->get_y(),
				olho->get_origem()->get_z() - look->get_z());
		u = up->produtoVetorial(n);

		n->normalizar();
		u->normalizar();

		v = n->produtoVetorial(u);
	}

	void setModelViewMatrix(){

		Vetor *eVec = new Vetor(olho->get_origem(), olho->get_origem()->get_x(), olho->get_origem()->get_y(), olho->get_origem()->get_z());

		float m[16];

		Vetor *u = olho->get_u();
		Vetor *v = olho->get_v();
		Vetor *n = olho->get_n();

	    m[0] = u->get_x(); m[4] = u->get_y(); m[8] = u->get_z(); m[12] = -eVec->get_origem()->get_x();
	    m[1] = v->get_x(); m[5] = v->get_y(); m[9] = v->get_z(); m[13] = -eVec->get_origem()->get_y();
	    m[2] = n->get_x(); m[6] = n->get_y(); m[10] = n->get_z(); m[14] = -eVec->get_origem()->get_z();
	    m[3] = 0; m[7] = 0; m[11] = 0; m[15] = 1.0;
	    glMatrixMode(GL_MODELVIEW);
	    glLoadMatrixf(m);
	}

	void slide(double delU, double delV, double delN){

		Vetor *u = olho->get_u();
		Vetor *v = olho->get_v();
		Vetor *n = olho->get_n();

		olho->get_origem()->set(
				olho->get_origem()->get_x() + delU * u->get_x() + delV * v->get_x() + delN * n->get_x(),
				olho->get_origem()->get_y() + delU * u->get_y() + delV * v->get_y() + delN * n->get_y(),
				olho->get_origem()->get_z() + delU * u->get_z() + delV * v->get_z() + delN * n->get_z()
				);

		setModelViewMatrix();
	}
};

#endif /* CAMERA_H_ */
