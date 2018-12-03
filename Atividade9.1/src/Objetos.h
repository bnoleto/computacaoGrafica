#ifndef OBJETOS_H_
#define OBJETOS_H_

#include "Ponto.h"
#include "Mesh.h"

class Objetos{

private:

	Ponto *posicao_atual = nullptr;
	Mesh *obj_matematico = nullptr;

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

	void funcao_matematica(unsigned int dominio){

		obj_matematico = new Mesh();

		obj_matematico->reset();

		for(int z = 0; z < 360; z++){
			for(int i_x = -250; i_x <= 250; i_x++){

				float x = (float)i_x*dominio/250;		// domínio = -dominio <= x <= dominio
				//float y = x-100*(pow(x,5));									// f(x) = x-100*(x^5)
				//float y = pow(x,3);											// f(x) = x^3
				float y = pow(pow(x,2)+pow(x,2)-1,3)-(pow(x,2)*pow(x,3));		// f(x) = (x^2+x^2-1)^3-x^2*x^3

				obj_matematico->get_lista_pontos()->push_back(new Ponto(x,y,z));
			}
		}
	}


public:

	Objetos(){
		funcao_matematica(5);
	}

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

	void desenhar_func_matematica(){
		glColor3ub(255, 0, 0);
		glBegin(GL_LINE_STRIP);
		for(unsigned int j = 0; j< obj_matematico->get_lista_pontos()->size(); j++){
			Ponto* atual = obj_matematico->get_lista_pontos.at(j);
				glVertex2f(atual->get_x()/5,atual->get_y()/5);
			}
		glEnd();

	}

};



#endif /* OBJETOS_H_ */
