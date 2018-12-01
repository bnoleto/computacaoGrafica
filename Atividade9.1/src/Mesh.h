#ifndef MESH_H_
#define MESH_H_

#include<vector>
#include "Face.h"
#include "MatrizTransformacao.h"

class Mesh{
	private:
	vector<Face*> lista_faces;
	vector<Ponto*> lista_pontos;
	Ponto centro_massa = Ponto(0,0,0);
	MatrizTransformacao matriz = MatrizTransformacao(&lista_pontos, &centro_massa);

	public:

	void calcular_centro_massa(){
		double menor_x = lista_pontos.at(0)->get_x();
		double maior_x = lista_pontos.at(0)->get_x();
		double menor_y = lista_pontos.at(0)->get_y();
		double maior_y = lista_pontos.at(0)->get_y();
		double menor_z = lista_pontos.at(0)->get_z();
		double maior_z = lista_pontos.at(0)->get_z();

		for(unsigned int i = 1; i<lista_pontos.size(); i++){
			Ponto* ponto_atual = lista_pontos.at(i);

			if(ponto_atual->get_x() < menor_x){
				menor_x = ponto_atual->get_x();
			}

			if(ponto_atual->get_y() < menor_y){
				menor_y = ponto_atual->get_y();
			}
			if(ponto_atual->get_z() < menor_z){
				menor_z = ponto_atual->get_z();
			}

			if(ponto_atual->get_x() > maior_x){
				maior_x = ponto_atual->get_x();
			}
			if(ponto_atual->get_y() > maior_y){
				maior_y = ponto_atual->get_y();
			}
			if(ponto_atual->get_z() > maior_z){
				maior_z = ponto_atual->get_z();
			}
		}

		double media_x = (menor_x+maior_x)/2;
		double media_y = (menor_y+maior_y)/2;
		double media_z = (menor_z+maior_z)/2;

		this->centro_massa = Ponto (media_x,media_y,media_z);
	}

	void reset(){
		this->lista_faces.clear();
		this->lista_pontos.clear();
	}

	vector<Ponto*>* get_lista_pontos(){
		return &this->lista_pontos;
	}

	Ponto* get_centro_massa(){
		return &this->centro_massa;
	}

	vector<Face*>* get_lista_faces(){
		return &this->lista_faces;
	}

	MatrizTransformacao* get_matriz_transformacao(){
		return &this->matriz;
	}

	void imprimir(){
		glLineWidth(1);
		if(lista_faces.size() > 1){
			glColor3ub(255, 153, 0);
			for(unsigned int i = 0; i<lista_faces.size(); i++){
				Face* face_atual = lista_faces.at(i);

				glBegin(GL_LINE_LOOP);

				for(unsigned int j = 0; j<face_atual->get_pontos()->size(); j++){
					glVertex3d(face_atual->get_pontos()->at(j)->get_x(), face_atual->get_pontos()->at(j)->get_y(), face_atual->get_pontos()->at(j)->get_z());
				}

				glEnd();
			}
		}
	}

	void imprimir_centro_massa(){

				glColor3ub(0, 255, 255);
				glBegin(GL_LINE_LOOP);
					glVertex3d(centro_massa.get_x(), centro_massa.get_y()+2, centro_massa.get_z());
					glVertex3d(centro_massa.get_x()+2, centro_massa.get_y(), centro_massa.get_z());
					glVertex3d(centro_massa.get_x(), centro_massa.get_y()-2, centro_massa.get_z());
					glVertex3d(centro_massa.get_x()-2, centro_massa.get_y(), centro_massa.get_z());
				glEnd();
				glBegin(GL_LINES);
					glVertex3d(centro_massa.get_x()-2, centro_massa.get_y(), centro_massa.get_z());
					glVertex3d(centro_massa.get_x()+2, centro_massa.get_y(), centro_massa.get_z());
					glVertex3d(centro_massa.get_x(), centro_massa.get_y()-2, centro_massa.get_z());
					glVertex3d(centro_massa.get_x(), centro_massa.get_y()+2, centro_massa.get_z());
				glEnd();


		}
};



#endif /* MESH_H_ */
