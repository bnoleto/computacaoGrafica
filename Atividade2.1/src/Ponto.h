/*
 * Pixel.h
 *
 *  Created on: 18 de ago de 2018
 *      Author: Bruno Noleto
 */
#ifndef PONTO_H_
#define PONTO_H_

class Ponto{
	private:
		short x,y;

	public:
		Ponto(short x, short y){
			this->x = x;
			this->y = y;
		}

		short get_x(){
			return this->x;
		}

		short get_y(){
			return this->y;
		}

};
#endif /* PONTO_H_ */

