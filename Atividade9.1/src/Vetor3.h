#ifndef VETOR3_H_
#define VETOR3_H_

#include<math.h>
#include "Ponto.h"

class Vetor3 {

	private :
		double x, y, z;
		Ponto *origem = nullptr;

	public :

	Vetor3(double x_dest, double y_dest, double z_dest){
		this->x = x_dest;
		this->y = y_dest;
		this->z = z_dest;
	}

	Vetor3(Ponto *origem){
		this->x = origem->get_x();
		this->y = origem->get_y();
		this->z = origem->get_z();
	}

	double get_x() {
		return this->x;
	}

	double get_y() {
		return this->y;
	}

	double get_z() {
		return this->z;
	}

	Vetor3 clone(){
		return Vetor3(x, y, z);
	}

	double dot(Vetor3 *vec){
	    return ((x*vec->x)+(y*vec->y)+(z*vec->z));
	}

	Vetor3 *produtoVetorial(Vetor3 *vetor2){
		// (vetor1 x vetor2)

		Vetor3 *vetor1 = this;

		double matriz[3][5] = {
				{1, 1, 1, 1, 1},
				{vetor1->get_x(), vetor1->get_y(), vetor1->get_z(), vetor1->get_x(), vetor1->get_y()},
				{vetor2->get_x(), vetor2->get_y(), vetor2->get_z(), vetor2->get_x(), vetor2->get_y()},
		};

		double novo_x = (matriz[0][0]*matriz[1][1]*matriz[2][2]) + (matriz[0][3]*matriz[1][2]*matriz[2][1]);
		double novo_y = (matriz[0][1]*matriz[1][2]*matriz[2][3]) + (matriz[0][4]*matriz[1][3]*matriz[2][2]);
		double novo_z = (matriz[0][2]*matriz[1][3]*matriz[2][4]) + (matriz[0][2]*matriz[1][1]*matriz[2][0]);

		return new Vetor3(novo_x, novo_y, novo_z);
	}

	void normalizar(){
		double maior_valor = abs(this->x);

		if(abs(this->y) > maior_valor){
			maior_valor = abs(this->y);
		}
		if(abs(this->z) > maior_valor){
			maior_valor = abs(this->z);
		}

		this->x /= maior_valor;
		this->y /= maior_valor;
		this->z /= maior_valor;
	}

	void set(double x, double y, double z) {
		this->x = x;
		this->y = y;
		this->z = z;
	}
};

#endif /* VETOR3_H_ */
