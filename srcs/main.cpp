#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include "check_map.hpp"

#define WIDTH fenetre.getSize().x
#define HEIGHT fenetre.getSize().y
#define VITESSE 0.2
#define ROTATION 1


std::vector <std::string> lignes;									// On crée un tableau de chaîne de charactère.
std::string ligne;													// On crée un chaîne de charactère.						

std::ifstream map("../map.rc");										// On ouvre un flux permettant de lire la map.


sf::Vector2f get_x(std::vector<std::string> const& lignes)
{
	for (unsigned int i = 0; i < lignes.size(); ++i)
	{
		for (unsigned int j = 0; j  < lignes.back().size(); ++j)
		{
			if(lignes[i][j] == 'X')
				return sf::Vector2f(j + 1, i + 1);
		}
	}
	return (sf::Vector2f(-1, -1));
}


struct player
{
	sf::Vector2f dir = sf::Vector2f(0, -1);
	sf::Vector2f pos = get_x(lignes);

	void move()
	{
		pos.x = pos.x + dir.x * VITESSE;
		pos.y = pos.y + dir.y * VITESSE;
	}
	void rot(bool sens)
	{
		float	tmp = dir.x;
		int		degree = ROTATION * ((sens) ? -1 : 1);

		dir.x = dir.x * cos(degree) - dir.y * sin(degree);
		dir.y = tmp * sin(degree) + dir.y * cos(degree);
	}
};

int main(int argc, char** argv)
{
	player joueur;

	
	unsigned int taille_x = 0;
	unsigned int taille_y = 0;

	while(getline(map, ligne))											// Tant qu'on a pas parcouru toutes les lignes de la map
	{
		lignes.push_back(ligne);										// Alors on ajoute dans notre tableau de chaîne de charactère la "ligne" ème ligne.
		taille_y++;
	}
			
	if(!check_map(lignes))												// Puis on effectue une vérification de la map.
		return EXIT_FAILURE;

	taille_x = lignes.back().size();

	sf::RenderWindow fenetre(sf::VideoMode(1600, 900), "RayCasting");	// Création d'une fenêtre de taille 1600 x 900 pixels nommée "RayCasting".
	fenetre.setVerticalSyncEnabled(true);

	sf::Event evenement;												// Création d'un évènement.
	
	std::cout<<taille_x<<std::endl;
	std::cout<<taille_y<<std::endl;


	while(fenetre.isOpen())												// Tant que la fenètre est ouverte
	{
		while(fenetre.pollEvent(evenement))								// Tant qu'un évènement est détecté
		{
			if(evenement.type == sf::Event::Closed)						// Si l'évènement détecté est la fermeture de la fenêtre
				fenetre.close();										// Alors on ferme la fenêtre.
		}

		fenetre.clear(sf::Color::Black);								// Effacement de tout ce qui apparait sur la fenêtre avec la couleur noir.
		fenetre.display();												// Affichage de tout ce qui a été dessiné.
	}

	return 0;															// On ferme le programme à la fin.
}
