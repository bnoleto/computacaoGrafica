/*
 * Teardrop.h
 *
 *  Created on: 15 de set de 2018
 *      Author: Bruno Noleto
 */

#ifndef TEARDROP_H_
#define TEARDROP_H_
#include"Ponto.h"
#include<math.h>
#include<vector>

class Teardrop {
private:
	vector<Ponto*>* pontos = new vector<Ponto*>;
	Ponto* ponto_inicial = nullptr;
	Ponto* centro_circunferencia = nullptr;
	int raio;
	int angulo;

public:
	Teardrop(Ponto* p_inicial, int raio, int angulo) {
		this->ponto_inicial = p_inicial;
		pontos->push_back(ponto_inicial);
		this->raio = raio;
		this->angulo = angulo;

		int x = this->ponto_inicial->get_x(), y = this->ponto_inicial->get_y();

		this->centro_circunferencia = new Ponto(x + 4*this->raio*cos(rad(-this->angulo)), y + 4*this->raio*sin(rad(-this->angulo)));

		gerarCircunferencia();

	}

	void gerarCircunferencia(){
		int x = centro_circunferencia->get_x();
		int y = centro_circunferencia->get_y();
		for(int i = 0; i<180; i++){
			pontos->push_back(new Ponto(
					x + this->raio*cos(rad(-1*(90+angulo-i))),
					y + this->raio*sin(rad(-1*(90+angulo-i)))
			));
		}
	}

	float rad(float angulo){	// converterá de graus para radianos
		return angulo*3.141592/180;
	}

	vector<Ponto*>* get_pontos(){
		return this->pontos;
	}

};

#endif /* TEARDROP_H_ */
