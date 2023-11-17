#include "GameEngine.h"
#include "Configuration.hpp"

int main()
{
	Configuration::initialize();
	GameEngine game = GameEngine();
	return 0;
}