#include <iostream>
#include "helperFunctions.h"
#include "GameEngine.h"

using namespace std;

int main()
{
	hideCursor();
	GameEngine Engine;
	Engine.play();
}

//Use arrows and space to play