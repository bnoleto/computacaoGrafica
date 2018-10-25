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
	MatrizTransformacao matriz = MatrizTransformacao(&lista_faces, &lista_pontos, &centro_massa);

	public:

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
};



#endif /* MESH_H_ */
