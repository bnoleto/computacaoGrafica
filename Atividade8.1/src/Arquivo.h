#ifndef ARQUIVO_H_
#define ARQUIVO_H_

#include <fstream>
#include "Mesh.h"

using namespace std;

double proximo_double(string entrada, unsigned int* contador){

	string saida;

	unsigned int i = *contador;

	while(!(isdigit(entrada.at(i)) || entrada.at(i) == '-')){
		i++;
	}
	while(i<entrada.length()){
		if(isdigit(entrada.at(i)) || entrada.at(i) == '-' || entrada.at(i) == '.'){
			saida+=entrada.at(i);
			i++;
		}else{
			while(i<entrada.length() && entrada.at(i) != ' '){
				i++;
			}

			break;
		}
	}

	*contador = i;

	return stod(saida);
}

int proximo_int(string entrada, unsigned int* contador){

	string saida;

	unsigned int i = *contador;

	if(i<entrada.length()){
		while(i<entrada.length() && !isdigit(entrada.at(i))){
			i++;
		}
		while(i<entrada.length() && isdigit(entrada.at(i))){
			saida+=entrada.at(i);
			i++;
		}
		while(i<entrada.length() && entrada.at(i) != ' '){
			i++;
		}
		*contador = i;

	} else{
		return -1;
	}

	return stoi(saida);
}

void abrir_arquivo(string arquivo_caminho, Mesh* mesh_principal){
		fstream arquivo;

		arquivo.open(arquivo_caminho, ios::in);
		if(arquivo.is_open()){

			mesh_principal->reset();

			vector<Ponto*>* lista_pontos = mesh_principal->get_lista_pontos();
			vector<Face*>* lista_faces = mesh_principal->get_lista_faces();

			string linha;

			while(getline(arquivo,linha)){

				int j = 0;

				double x, y, z;

				for(unsigned int i = 0; i< linha.length(); i++){
					if(linha.at(i) == ' '){
						switch(linha.at(i-1)){
							case 'v' :
								j=i;

								x = proximo_double(linha.substr(j), &i);
								y = proximo_double(linha.substr(j), &i);
								z = proximo_double(linha.substr(j), &i);

								lista_pontos->push_back(new Ponto(x,y,z));

								break;
							case 'f' :
								vector<int> valor;

								j=i;

								Face* f1 = new Face();

								while(i < linha.size()){
									int proximo = proximo_int(linha.substr(j), &i);

									if(proximo == -1){
										break;
									} else{
										valor.push_back(proximo);
									}
								}

								for(unsigned int p = 0; p < valor.size(); p++){
									f1->get_pontos()->push_back(lista_pontos->at(valor.at(p)-1));
								}


								lista_faces->push_back(f1);

								break;
						}
					}
				}
			}

			mesh_principal->calcular_centro_massa();

			arquivo.close();

			cout << lista_faces->size() << " faces"<< endl;
			cout << lista_pontos->size() << " pontos"<< endl;

			cout << "> Arquivo " << arquivo_caminho << " importado!" << endl;
		} else {
			cout << "> Não foi possível abrir o arquivo " << arquivo_caminho << endl;
		}

		glutPostRedisplay();
	}

#endif /* ARQUIVO_H_ */
