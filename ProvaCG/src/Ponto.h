/*
 * Pixel.h
 *
 *  Created on: 18 de ago de 2018
 *      Author: Bruno Noleto
 */
#ifndef PONTO_H_
#define PONTO_H_
#include <iostream>
#include <math.h>

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
//			cout << "(" << this->x << ", " << this->y << ") -> ";
			this->x = x;
			this->y = y;
//			cout << "(" << this->x << ", " << this->y << ")" << endl;
		}

		bool operator==(const Ponto& outro){
			// in this method, see if t is equal to *this.
			//return outra == *this;
			return this == &outro;
		}

		float distancia_para(Ponto* p1){
			float xA = (float)p1->get_x();
			float xB = (float)x;
			float yA = (float)p1->get_y();
			float yB = (float)y;

//			cout << sqrt(pow((xB-xA),2)+pow((yB-yA),2)) << endl;

			return sqrt(pow((xB-xA),2)+pow((yB-yA),2));
		}

};
#endif /* PONTO_H_ */

