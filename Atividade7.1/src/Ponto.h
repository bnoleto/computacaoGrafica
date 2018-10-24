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
		double x,y,z;

	public:
		Ponto(double x, double y, double z){
			this->x = x;
			this->y = y;
			this->z = z;
		}

		double get_x(){
			return this->x;
		}

		double get_y(){
			return this->y;
		}

		double get_z(){
			return this->z;
		}

		void set(double x, double y, double z){
			cout << "(" << this->x << ", " << this->y << ", " << this->z << ") -> ";
			this->x = x;
			this->y = y;
			this->z = z;
			cout << "(" << this->x << ", " << this->y << ", " << this->z << ")" << endl;
		}

		bool operator==(const Ponto& outro){
			// in this method, see if t is equal to *this.
			//return outra == *this;
			return this == &outro;
		}

};
#endif /* PONTO_H_ */

