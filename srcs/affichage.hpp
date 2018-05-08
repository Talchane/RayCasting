#ifndef AFFICHAGE_HPP
#define AFFICHAGE_HPP

float dist_mur(sf::Vector2f const& dir, sf::Vector2f const& pos, std::vector<std::string> lignes)
{
	//which box of the map we're in
	int mapX = (int)pos.x;
	int mapY = (int)pos.y;
	//length of ray from current position to next x or y-side
	float sideDistX;
	float sideDistY;
	//length of ray from one x or y-side to next x or y-side
	float deltaDistX = fabs(1 / dir.x);
	float deltaDistY = fabs(1 / dir.y);
	//what direction to step in x or y-direction (either +1 or -1)
	int stepX;
	int stepY;
	int side; //was a NS or a EW wall hit?
	//calculate step and initial sideDist
	if (dir.x < 0)
	{
		stepX = -1;
		sideDistX = (pos.x - mapX) * deltaDistX;
	}
	else
	{
		stepX = 1;
		sideDistX = (mapX + 1.0 - pos.x) * deltaDistX;
	}
	if (dir.y < 0)
	{
		stepY = -1;
		sideDistY = (pos.y - mapY) * deltaDistY;
	}
	else
	{
		stepY = 1;
		sideDistY = (mapY + 1.0 - pos.y) * deltaDistY;
	}
	while (lignes[mapY][mapX] != '1')
	{
		if (sideDistX < sideDistY)
		{
			sideDistX = sideDistX + deltaDistX;
			mapX = mapX + stepX;
			side = 0;
		}
		else
		{
			sideDistY = sideDistY + deltaDistY;
			mapY = mapY + stepY;
			side = 1;
		}
	}
	if (!side)
		return ((mapX - pos.x + (1 - stepX) / 2) / dir.x);
	else
		return ((mapY - pos.y + (1 - stepY) / 2) / dir.y);
}

void afficher_mur(float const& dist_mur, sf::Image &image, unsigned int const& colonne, bool const& accroupi)
{
	unsigned short int ratio = HEIGHT * 0.5;
	unsigned short int mur_h = (unsigned short int)(ratio / dist_mur);
	unsigned short int rab_cl = (unsigned short int)((HEIGHT - mur_h) / 2);

	if(accroupi == true)
	{
		rab_cl = rab_cl - 40;
	}


	for (int i = 0; i < rab_cl && i < HEIGHT; ++i)
	{
		image.setPixel(colonne, i, sf::Color(80, 80, 80));
	}

	for (int i = rab_cl; i < mur_h + rab_cl && i < HEIGHT; ++i)
	{
		image.setPixel(colonne, i, sf::Color(20, 20, 20));

	}

	if (mur_h + rab_cl >= HEIGHT)
	{
		for (int a = 0; a < HEIGHT; ++a)
			image.setPixel(colonne, a, sf::Color(20, 20, 20));
	}

	else
	{
		for (int i = mur_h + rab_cl; i < HEIGHT; ++i)
		{
			image.setPixel(colonne, i, sf::Color(40, 40, 40));
		}
	}


}

void diviser_ray(const player &joueur, sf::Image &image, std::vector<std::string> lignes, bool accroupi)
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


	for (w = 0; w < WIDTH; ++w)
	{
		vec.x = test_x * cos(-diff_ray) - test_y * sin(-diff_ray);
		vec.y = test_x * sin(-diff_ray) + test_y * cos(-diff_ray);


		test_x = vec.x;
		test_y = vec.y;
		afficher_mur(dist_mur(vec, joueur.pos, lignes), image, w, accroupi);
	}
}

#endif