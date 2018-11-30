#ifndef MATRIZTRANSFORMACAO_H_
#define MATRIZTRANSFORMACAO_H_

#include<vector>
#include<Math.h>
#include"Face.h"

class MatrizTransformacao{
	private:

	float tamanho_figura = 1;

	double matriz[4][4];
	vector<Ponto*>* lista_pontos = nullptr;
	Ponto *centro_massa = nullptr;
	char eixo_atual = 'X';

	double rad(double angulo){	// converterá de graus para radianos
		return angulo*3.141592/180;
	}

	void reset(){
		for(int i = 0; i<4; i++){
			for(int j = 0; j<4; j++){
				if(i == j){
					matriz[i][j] = 1.0;
				}
				else{
					matriz[i][j] = 0.0;
				}
			}
		}
	}

	void funcao_transformacao(Ponto *ponto_atual){
		double matriz_mult[4] = {ponto_atual->get_x(),ponto_atual->get_y(), ponto_atual->get_z(), 1};

		matriz_mult[0] -= this->centro_massa->get_x();
		matriz_mult[1] -= this->centro_massa->get_y();
		matriz_mult[2] -= this->centro_massa->get_z();

		double x_final = 0;
		double y_final = 0;
		double z_final = 0;
		for(int i_matriz = 0; i_matriz < 4; i_matriz++){
			x_final+= matriz[0][i_matriz] * matriz_mult[i_matriz];
			y_final+= matriz[1][i_matriz] * matriz_mult[i_matriz];
			z_final+= matriz[2][i_matriz] * matriz_mult[i_matriz];
		}

		x_final += this->centro_massa->get_x();
		y_final += this->centro_massa->get_y();
		z_final += this->centro_massa->get_z();

		ponto_atual->set(x_final, y_final, z_final);
	}

	void aplicar_transformacao(){

		for(unsigned int i = 0; i < lista_pontos->size(); i++){	// para cada ponto do conjunto
			funcao_transformacao(lista_pontos->at(i));
		}

		funcao_transformacao(this->centro_massa);
		reset();
	}

	public:

	MatrizTransformacao(vector<Ponto*>* pontos, Ponto* centro_massa){
		this->lista_pontos = pontos;
		this->centro_massa = centro_massa;
		reset();
	}

	void set_eixo(char entrada){
		if (entrada == 'X' || entrada == 'Y' || entrada == 'Z'){
			cout << "O eixo agora é " << entrada << "!" << endl;
			this->eixo_atual = entrada;
		}
	}

	void translacao(double delta_x, double delta_y, double delta_z){
		matriz[0][3] = delta_x;
		matriz[1][3] = delta_y;
		matriz[2][3] = delta_z;

		aplicar_transformacao();
	}

	void escala(double multiplicador){
		if(!(tamanho_figura < 0.01 && multiplicador < 1)){	// condição para impedir de perder os pontos ao deixar a imagem muito pequena
			tamanho_figura *= multiplicador;
			matriz[0][0] = multiplicador;
			matriz[1][1] = multiplicador;
			matriz[2][2] = multiplicador;
			aplicar_transformacao();
		}
	}

	void rotacao(double angulo){
		switch(eixo_atual){
			case 'X' :
				matriz[1][1] = cos(rad(angulo));
				matriz[1][2] = -sin(rad(angulo));
				matriz[2][1] = sin(rad(angulo));
				matriz[2][2] = cos(rad(angulo));
				break;
			case 'Y' :
				matriz[0][0] = cos(rad(angulo));
				matriz[2][0] = -sin(rad(angulo));
				matriz[0][2] = sin(rad(angulo));
				matriz[2][2] = cos(rad(angulo));
				break;
			case 'Z' :
				matriz[0][0] = cos(rad(angulo));
				matriz[0][1] = -sin(rad(angulo));
				matriz[1][0] = sin(rad(angulo));
				matriz[1][1] = cos(rad(angulo));
				break;
		}

		aplicar_transformacao();
	}

};

#endif /* MATRIZTRANSFORMACAO_H_ */
