#include "operators.hpp"

std::ostream& operator<<(std::ostream& os, const Morceaux& morceau)
{
	return os << morceau.titre << ';' << morceau.album << ';' << morceau.artiste;
}