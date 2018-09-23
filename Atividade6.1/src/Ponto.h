/*
 * Pixel.h
 *
 *  Created on: 18 de ago de 2018
 *      Author: Bruno Noleto
 */
#ifndef PONTO_H_
#define PONTO_H_
#include <iostream>

using namespace std;


class Ponto{
	private:
		float x,y;

	public:
		Ponto(float x, float y){
			this->x = x;
			this->y = y;
		}

		float get_x(){
			return this->x;
		}

		float get_y(){
			return this->y;
		}

		void set(float x, float y){
			cout << "(" << this->x << ", " << this->y << ") -> ";
			this->x = x;
			this->y = y;
			cout << "(" << this->x << ", " << this->y << ")" << endl;
		}

		bool operator==(const Ponto& outro){
			// in this method, see if t is equal to *this.
			//return outra == *this;
			return this == &outro;
		}

};
#endif /* PONTO_H_ */

