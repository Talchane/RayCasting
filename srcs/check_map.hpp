#ifndef CHECK_MAP_HPP
#define CHECK_MAP_HPP


bool check_map(std::vector<std::string> const& carte)
{
	int lettre_x = 0;

	for (unsigned int i = 0; i < carte.size(); ++i)												// Tant qu'on a pas parcouru toutes les lignes de la map
	{
		for (unsigned int j = 0; j < carte[0].size(); ++j)										// Tant qu'on a pas parcoure tout les caractères de la ligne
		{
			if(carte[i][j] == 'X')																// Si une des lettres de la map est un 'X'
				lettre_x++;																		// Alors le nombre de X augmente de '1'.
										
			if (carte[i][j] != 'X' &&															// Si une des lettres de la map n'est ni un 'X'
				carte[i][j] != '1' && 															// Ni un '1'
				carte[i][j] != '0')																// Ni un '0'
			{
				std::cout << "Il y à un '" << carte[i][j] << "' dans la map... " << std::endl;	// Alors on précise quel lettre est étrange
				std::cout << "ligne : " << i+1 << " !" << std::endl;							// Puis on indique quel ligne
				std::cout << "caractère : " << j+1 << " !" << std::endl;						// Puis quel charactère de la ligne
				return false;																	// Et puis il y a une erreur.
			}
		}
	}

	if(lettre_x == 0)																			// Si il n'y a aucun 'X' dans la map
	{
		std::cout << "Il dois y avoir au minimum 1 'X' dans la map." << std::endl;				// Alors on indique le problème
		return false;																			// Puis il y a une erreur.      
	}

	if(lettre_x >= 2)																			// Si il y à plus de 1 'X' dans la map
	{		
		std::cout << "Il ne dois pas y avoir plus de 1 'X' dans la map." << std::endl;			// Alors on indique le problème																										// Puis il y a une erreur.     
		return false;																			// Puis il y a une erreur.
	}

	for (unsigned int i = 0; i < carte.size(); ++i)												// Tant qu'on a pas parcouru toutes les lignes de la map
	{
		if(i >= 1)																				// Si on en est au moin à la 2 ème ligne
		{
			if(carte[i].size() != carte[i-1].size())											// Si cette ligne n'est pas aussi grande que la ligne d'avant 
			{
				std::cout << "La map n'est pas rectangulaire." << std::endl;					// Alors on précise que la carte n'est pas rectangulaire
				std::cout << "Ligne : " << i << " !" << std::endl;								// Puis on indique la ligne
				return false;																	// Puis il y a une erreur
			}
		}
	}

	unsigned int taille;
	taille = carte.back().size();																// Taille est égal à la taille de la dernière ligne de la map.

	for (unsigned int i = 0; i < taille; ++i)													// Tant qu'on a pas parcouru tout les caractères de la dernière ligne de la map
	{
		if(carte.back()[i] != '1')																// Si un des charactère de la dernière ligne de la map est différent du charactère '1' 
		{
			std::cout << "Il y a un trou au sud de la map." << std::endl;						// Alors on indique qu'il y a un trou au sud de la map
			std::cout << "charactère : " << i << " !" << std::endl;								// Puis on indique quel charactère de la ligne
			return false;																		// Puis il y a une erreur.
		}

		if(carte[0][i] != '1')																	// Si un des charactère de la première ligne de la map est différent du charactère '1'
		{
			std::cout << "Il y a un trou au nord de la map." << std::endl;						// Alors on indique qu'il y a un trou au nord de la map
			std::cout << "charactère : " << i << " !" << std::endl;								// Puis on indique quel charactère de la ligne
			return false;																		// Puis il y a une erreur.
		}
	}

	for (unsigned int i = 0; i < carte.size(); ++i)											
	{
		if(carte[i][0] != '1') 																	// Si le premier charactère de la i ème ligne est différent du charactère '1'
		{
			std::cout << "Il manque un mur à l'ouest de la carte." << std::endl;				// Alors on indique qu'il y a un trou à l'ouest de la map
			std::cout << "ligne numéro : " << i + 1 << "." << std::endl;						// Puis on indique quel ligne
			return false;																		// Puis il y a une erreur.
		}

		if(carte[i][carte[i].size()-1] != '1')													// Si Le dernier charactère de la i ème ligne est différent du charactère '1'
		{
			std::cout << "Il manque un mur à l'est de la carte." << std::endl;					// Alors on indique qu'il y a un trou à l'est de la map 
			std::cout << "ligne numéro : "<< i + 1 << "." << std::endl;							// Puis on indique quel ligne
			return false;																		// Puis il y a une erreur.
		} 
	}

	std::cout << "Aucune erreur trouvé." << std::endl;											// Si aucune erreur n'a été trouvé, alors on le dit
	return true;																				// Puis on sort de la fonction sans erreur.
}

#endif