#pragma once

#include <string>
#include <vector>

struct Morceaux
{
	std::string titre;
	std::string album;
	std::string artiste;
};

using Discographie = std::vector<Morceaux>;
