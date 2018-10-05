#ifndef MATRIZTRANSFORMACAO_H_
#define MATRIZTRANSFORMACAO_H_
#include<math.h>
#include<vector>


class MatrizTransformacao{
	private:

	float tamanho_figura = 1;

	float matriz[3][3];
	vector<Ponto*>* lista_pontos = nullptr;
	Ponto* centro_massa = nullptr;


	float rad(float angulo){	// converterá de graus para radianos
		return angulo*3.141592/180;
	}

	void reset(){
		for(int i = 0; i<3; i++){
			for(int j = 0; j<3; j++){
				if(i == j){
					matriz[i][j] = 1.0;
				}
				else{
					matriz[i][j] = 0.0;
				}
			}
		}

	}

	void aplicar_transformacao(){
		for(unsigned int i = 0; i < lista_pontos->size(); i++){	// para cada ponto do conjunto
			Ponto* ponto_atual = lista_pontos->at(i);

	/*		if(i == lista_pontos->size()){	// ajustará o centro de massa no final do laço
				ponto_atual = &centro_massa;
			}
			else{
				ponto_atual = lista_pontos->at(i);
			}*/

			float matriz_mult[3] = {ponto_atual->get_x(),ponto_atual->get_y(), 1};

			matriz_mult[0] -= centro_massa->get_x();
			matriz_mult[1] -= centro_massa->get_y();

			float x_final = 0.0;
			for(int i_matriz = 0; i_matriz < 3; i_matriz++){
				x_final+= matriz[0][i_matriz] * matriz_mult[i_matriz];
			}

			float y_final = 0.0;
			for(int i_matriz = 0; i_matriz < 3; i_matriz++){
				y_final+= matriz[1][i_matriz] * matriz_mult[i_matriz];
			}

			x_final += centro_massa->get_x();
			y_final += centro_massa->get_y();

			ponto_atual->set(x_final, y_final);

		}
		reset();
	}

	public:


	MatrizTransformacao(vector<Ponto*>* pontos, Ponto* centro_massa){
		this->centro_massa = centro_massa;
		this->lista_pontos = pontos;
		reset();
	}

	void translacao(float delta_x, float delta_y){
		matriz[0][2] = delta_x;
		matriz[1][2] = delta_y;
		aplicar_transformacao();
	}

	void escala(float multiplicador){
		if(!(tamanho_figura < 0.01 && multiplicador < 1)){	// condição para impedir de perder os pontos ao deixar a imagem muito pequena
			tamanho_figura *= multiplicador;
			matriz[0][0] = multiplicador;
			matriz[1][1] = multiplicador;
			aplicar_transformacao();
		}
	}

	void rotacao(float angulo){
		matriz[0][0] = cos(rad(angulo));
		matriz[0][1] = -sin(rad(angulo));
		matriz[1][0] = sin(rad(angulo));
		matriz[1][1] = cos(rad(angulo));
		aplicar_transformacao();
	}

	void atualizar_matriz(){
		reset();
	}

};

#endif /* MATRIZTRANSFORMACAO_H_ */
