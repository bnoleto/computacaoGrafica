#ifndef VETOR3_H_
#define VETOR3_H_

#include "Ponto.h"
#include "Vetor.h"

class Vetor3{

private:
	Ponto *origem;
	Vetor *u, *v, *n;

public:

	Vetor3(Ponto* origem, Vetor *u, Vetor *v, Vetor *n){
		this->origem = origem;
		this->u = u;
		this->v = v;
		this->n = n;
	}

	Ponto* get_origem(){
		return this->origem;
	}

	Vetor* get_u(){
		return this->u;
	}

	Vetor* get_v(){
		return this->v;
	}

	Vetor* get_n(){
		return this->n;
	}
};




#endif /* VETOR3_H_ */
