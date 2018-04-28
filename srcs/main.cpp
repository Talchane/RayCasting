#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <cmath>
#include "check_map.hpp"

#define WIDTH 1600
#define HEIGHT 900
#define VITESSE 0.2
#define ROTATION 1
#define FOV M_PI/2

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
				return sf::Vector2f(j, i);
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

		float norme = sqrt(dir.x*dir.x+dir.y*dir.y);
		dir = dir / norme;
	}
};



float dist_mur(sf::Vector2f const& dir, sf::Vector2f const& pos, std::vector<std::string> lignes)
{
	float dx;
	float dy;
	float x = pos.x;
	float y = pos.y;

	if (fabs(dir.x - pos.x) > fabs(dir.y - pos.y))
	{
		dy = (dir.y - pos.y) / (dir.x - pos.x);
		dx = ((dir.x - pos.x) < 0) ? -1 : 1;
	}
	else
	{
		dx = (dir.x - pos.x) / (dir.y - pos.y);
		dy = ((dir.y - pos.y) < 0) ? -1 : 1;
	}
	while (lignes[(int)y][(int)x] != '1')
	{
		x = x + dx;
		y = y + dy;
	}
	return (sqrt((x-pos.x)*(x-pos.x)+(y-pos.y)*(y-pos.y)));
	/*
	si |x2-x1| >= |y2-y1| alors
		longueur := |x2-x1|
	sinon
		longueur := |y2-y1|
	fin si
	dx := (x2-x1) / longueur
	dy := (y2-y1) / longueur
	x := x1 + 0.5
	y := y1 + 0.5
	i := 1
	tant que i ≤ longueur faire
		setPixel ( E ( x ), E ( y ) )
		x := x + dx
		y := y + dy
		i := i + 1
	fin tant que
	*/
}

void afficher_mur(float const& dist_mur, sf::Image &image, unsigned int colonne, sf::Vector2f vec)
{
	unsigned short int ratio = HEIGHT * 0.5;
	unsigned short int mur_h = (unsigned short int)(ratio / dist_mur);
	unsigned short int rab_cl = (unsigned short int)((HEIGHT - mur_h) / 2);

	std::cout <<"dist mur = "<< dist_mur << std::endl << "colonne = "<< colonne << std::endl << "vec = " << vec.x << " " << vec.y << std::endl << std::endl;

	for (int i = 0; i < rab_cl; ++i)
	{
		image.setPixel(colonne, i, sf::Color::Red);
	}

	for (int i = rab_cl; i < mur_h + rab_cl; ++i)
	{
		image.setPixel(colonne, i, sf::Color::Blue);
	}

	for (int i = mur_h + rab_cl; i < HEIGHT; ++i)
	{
		image.setPixel(colonne, i, sf::Color::Red);
	}


}

void diviser_ray(const player &joueur, sf::Image &image, std::vector<std::string> lignes)
{
	float diff_ray = FOV / WIDTH;

	sf::Vector2f vec;

	vec.x = joueur.dir.x * cos(FOV / 2) - joueur.dir.y * sin(FOV / 2);
	vec.y = joueur.dir.x * sin(FOV / 2) + joueur.dir.y * cos(FOV / 2);

	unsigned int w = 1;

	float test_x;
	float test_y;

	test_x = vec.x;
	test_y = vec.y;

//	afficher_mur(dist_mur(vec, joueur.pos, lignes), image, w, vec);

	for (w = 0; w < WIDTH; ++w)
	{
		vec.x = test_x * cos(-diff_ray) - test_y * sin(-diff_ray);
		vec.y = test_x * sin(-diff_ray) + test_y * cos(-diff_ray);


		test_x = vec.x;
		test_y = vec.y;
		afficher_mur(dist_mur(vec, joueur.pos, lignes), image, w, vec);
	}
}


int main()
{

	unsigned int taille_y = 0;

	while(getline(map, ligne))											// Tant qu'on a pas parcouru toutes les lignes de la map
	{
		lignes.push_back(ligne);										// Alors on ajoute dans notre tableau de chaîne de charactère la "ligne" ème ligne.
		taille_y++;
	}

	if(!check_map(lignes))												// Puis on effectue une vérification de la map.
		return EXIT_FAILURE;

	player joueur;

	//std::cout << joueur.pos.x << std::endl;
	//std::cout << joueur.pos.y << std::endl;

	sf::RenderWindow fenetre(sf::VideoMode(1600, 900), "RayCasting");	// Création d'une fenêtre de taille 1600 x 900 pixels nommée "RayCasting".
	fenetre.setVerticalSyncEnabled(true);

	sf::Event evenement;												// Création d'un évènement.

	sf::Image image;
	image.create(WIDTH, HEIGHT, sf::Color::Black);

	sf::Texture texture;
	sf::Sprite spa;

	while(fenetre.isOpen())												// Tant que la fenètre est ouverte
	{
		diviser_ray(joueur, image, lignes);
		texture.loadFromImage(image);
		spa.setTexture(texture);
		
		while(fenetre.pollEvent(evenement))								// Tant qu'un évènement est détecté
		{
			if(evenement.type == sf::Event::Closed)						// Si l'évènement détecté est la fermeture de la fenêtre
				fenetre.close();										// Alors on ferme la fenêtre.
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
				joueur.move();
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
				joueur.rot(false);
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
				joueur.rot(true);
		}

		fenetre.clear(sf::Color::Black);								// Effacement de tout ce qui apparait sur la fenêtre avec la couleur noir.
		fenetre.draw(spa);
		fenetre.display();												// Affichage de tout ce qui a été dessiné.
	}

	return 0;															// On ferme le programme à la fin.
}
