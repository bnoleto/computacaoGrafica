#ifndef VETOR_H_
#define VETOR_H_

#include<math.h>
#include "Ponto.h"

class Vetor {

	private :

		double x, y, z;
		Ponto *origem;

	public :

	Vetor(Ponto *origem, double x_dest, double y_dest, double z_dest){
		this->x = x_dest;
		this->y = y_dest;
		this->z = z_dest;
		this->origem = origem;
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

	Vetor clone(){
		Ponto *clone = new Ponto(origem->get_x(), origem->get_y(), origem->get_z());
		return Vetor(clone, x, y, z);
	}

	Ponto* get_origem(){
		return this->origem;
	}

	Vetor *produtoVetorial(Vetor *vetor2){
		// (vetor1 x vetor2)

		Vetor *vetor1 = this;

		double matriz[3][5] = {
				{1, 1, 1, 1, 1},
				{vetor1->get_x(), vetor1->get_y(), vetor1->get_z(), vetor1->get_x(), vetor1->get_y()},
				{vetor2->get_x(), vetor2->get_y(), vetor2->get_z(), vetor2->get_x(), vetor2->get_y()},
		};

		double novo_x = (matriz[0][0]*matriz[1][1]*matriz[2][2]) + (matriz[0][3]*matriz[1][2]*matriz[2][1]);
		double novo_y = (matriz[0][1]*matriz[1][2]*matriz[2][3]) + (matriz[0][4]*matriz[1][3]*matriz[2][2]);
		double novo_z = (matriz[0][2]*matriz[1][3]*matriz[2][4]) + (matriz[0][2]*matriz[1][1]*matriz[2][0]);

		return new Vetor(new Ponto(novo_x, novo_y, novo_z), x, y, z);
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

#endif /* VETOR_H_ */
