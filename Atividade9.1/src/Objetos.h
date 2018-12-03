#ifndef OBJETOS_H_
#define OBJETOS_H_

#include "Ponto.h"

class Objetos{

private:

	Ponto *posicao_atual = nullptr;

	void novo_hexaedro(double tamanho_x,double tamanho_y,double tamanho_z){

		Ponto ponto_criacao = posicao_atual->clone();
		glPushMatrix();
			glScaled(tamanho_x,tamanho_y,tamanho_z);
			glTranslated(ponto_criacao.get_x(), ponto_criacao.get_y(), ponto_criacao.get_z());
			glutWireCube(1);
		glPopMatrix();
	}

	void nova_esfera(double raio){
		Ponto ponto_criacao = posicao_atual->clone();
		glPushMatrix();
			glTranslated(ponto_criacao.get_x(), ponto_criacao.get_y(), ponto_criacao.get_z());
			glutWireSphere(raio, 16, 16);
		glPopMatrix();
	}

	void rotacionar(double x, double y, double z){

		glRotated(x, 1, 0, 0);
		glRotated(y, 0, 1, 0);
		glRotated(z, 0, 0, 1);
	}


public:

	void desenhar_letra_t(){

		posicao_atual = new Ponto(0,0,0);

		glPushMatrix();
			rotacionar(20,10,45);
			novo_hexaedro(10, 50, 10);
			posicao_atual->set(posicao_atual->get_x(), posicao_atual->get_y()+2, posicao_atual->get_z());
			novo_hexaedro(50, 10, 10);
			posicao_atual->set(posicao_atual->get_x(), posicao_atual->get_y()+28, posicao_atual->get_z());
			nova_esfera(5);
		glPopMatrix();
	}

};



#endif /* OBJETOS_H_ */
