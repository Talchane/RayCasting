#ifndef DEPLACEMENTS_HPP
#define DEPLACEMENTS_HPP 

deplacements:
{
	if ((sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) && 		// Si on appuis sur shift
		(!sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)))		// Mais qu'on appuie pas sur control
	{
		VITESSE = 3;												// Alors on augmente la vitesse.
	}

	if(!sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))			// Si on n'appuie pas sur control
		accroupi = false;											// Alors on est debout.

	if ((sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) && 	// Si on appuie sur Control
		(!sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)))		// Et que on appuie pas sur shift
	{
		accroupi = true;											// Alors on est accroupi
		VITESSE = 0.7;												// Et notre vitesse dimminue.
	}

	if ((!sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) && 	// Si on n'appuie pas sur control
		(!sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)))		// Et que on appuie pas sur shift
	{
		VITESSE = 1.7;												// Alors on diminue la vitesse.
	}

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))			// Si on appuie sur échape
		fenetre.close();											// Alors on ferme la fenêtre.

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Z))					// Si on appuie sur Z
		joueur.move(1, VITESSE, temps, lignes);						// Alors fait avancer le joueur.

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))					// Si on appuie sur S
		joueur.move(2, VITESSE, temps, lignes);						// Alors fait reculer le joueur.

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Q))					// Si on appuie sur Q
		joueur.move(3, VITESSE, temps, lignes);						// Alors fait aller à gauche le joueur.

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))					// Si on appuie sur D
		joueur.move(4, VITESSE, temps, lignes);						// Alors fait aller à droite le joueur.
}

#endif