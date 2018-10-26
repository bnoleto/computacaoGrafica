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

	void reset(){
		this->lista_faces.clear();
		this->lista_pontos.clear();
		centro_massa = Ponto(0,0,0);
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
		if(lista_faces.size() > 1){
			glColor3ub(255, 0, 0);
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
};



#endif /* MESH_H_ */
