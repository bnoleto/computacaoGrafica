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
			this->x = x;
			this->y = y;
			this->z = z;
		}

		bool operator==(const Ponto& outro){
			return this == &outro;
		}

};
#endif /* PONTO_H_ */

