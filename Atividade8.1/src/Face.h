#ifndef FACE_H_
#define FACE_H_

#include "Ponto.h"
#include <vector>

using namespace std;

class Face{
	private:

	vector<Ponto*>* lista_pontos;

	public:

	Face(){
		this->lista_pontos = new vector<Ponto*>;
	}

	vector<Ponto*>* get_pontos(){
		return this->lista_pontos;
	}
};



#endif /* FACE_H_ */
