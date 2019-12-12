#include <stdlib.h>
#include <stdio.h>

#define DELIMITEUR '@'

char lire_octet(FILE *fichier, uint8_t *octet) {
	char nombre = 8;
	int resultat;
       
	resultat = fgetc(fichier);
	if (resultat == EOF)
		/* Bug, la fin devrait être marquée par une directive */
		assert(0);
	/* Decodage du protocole de trames */
	if (resultat == DELIMITEUR) {
		resultat = fgetc(fichier);
		if (resultat == EOF)
			/* Bug, une directive fait deux caractères */
			assert(0);
		if (resultat == DELIMITEUR) {
			*octet = resultat;
		} else {
			resultat -= '0';
			if ((resultat >= 0) && (resultat <= 8)) {
				nombre = resultat;
				resultat = fgetc(fichier);
				if (resultat == EOF)
					/* Bug, il manque les données de fin */
					assert(0);
				*octet = resultat;
			} else
				/* Bug si resultat n'est pas entre 1 et 8 */
				assert(0);
		}
	} else
		*octet = resultat;
	return nombre;
}


int main(int argc, char* argv[]){
    
    return 0;
}