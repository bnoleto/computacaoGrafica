/*
 * Teardrop.h
 *
 *  Created on: 15 de set de 2018
 *      Author: Bruno Noleto
 */

#ifndef TEARDROP_H_
#define TEARDROP_H_
#include"Ponto.h"
#include"MatrizTransformacao.h"
#include<math.h>
#include<vector>

class Teardrop {
private:
	vector<Ponto*>* pontos = new vector<Ponto*>;
	Ponto* ponto_inicial = nullptr;
	Ponto* centro_circunferencia = nullptr;
	int raio;
	int angulo;

	float rad(float angulo){	// converterá de graus para radianos
		return angulo*3.141592/180;
	}

	void gerarCircunferencia(){
		int x = centro_circunferencia->get_x();
		int y = centro_circunferencia->get_y();
		for(int i = 0; i<180; i++){
			pontos->push_back(new Ponto(
					x + raio*cos(rad(-1*(90+angulo-i))),
					y + raio*sin(rad(-1*(90+angulo-i)))
			));
		}
	}

public:
	Teardrop(Ponto* p_inicial, int r, int ang) {
		this->ponto_inicial = p_inicial;
		pontos->push_back(ponto_inicial);
		this->raio = r;
		this->angulo = ang;

		int x = this->ponto_inicial->get_x(), y = this->ponto_inicial->get_y();

		// a distância do ponto inicial ao ponto central da circunferência será 4*raio
		this->centro_circunferencia = new Ponto(x + 4*raio*cos(rad(-angulo)), y + 4*raio*sin(rad(-angulo)));

		gerarCircunferencia();

	}

	vector<Ponto*>* get_pontos(){
		return pontos;
	}

	void add_angulo(float incremento,Ponto* centro_massa){

		MatrizTransformacao matriz = MatrizTransformacao(pontos, centro_massa);

		matriz.rotacao(incremento);

	}

	void atualizar(){
		pontos->clear();

		pontos->push_back(ponto_inicial);

		int x = this->ponto_inicial->get_x(), y = this->ponto_inicial->get_y();
		this->centro_circunferencia = new Ponto(x + 4*raio*cos(rad(-angulo)), y + 4*raio*sin(rad(-angulo)));

		gerarCircunferencia();
	}

};

#endif /* TEARDROP_H_ */
