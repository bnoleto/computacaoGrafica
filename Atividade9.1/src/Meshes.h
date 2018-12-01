/*
 * Meshes.h
 *
 *  Created on: 30 de nov de 2018
 *      Author: Bruno Noleto
 */

#ifndef MESHES_H_
#define MESHES_H_

class Meshes{

public:
	desenhar_chao(){
		glPushMatrix();
			glColor3ub(1,0.5,0);
			glTranslated(0,0.0,0);

			glutWireCube(1);

		glPopMatrix();
	}

};



#endif /* MESHES_H_ */
