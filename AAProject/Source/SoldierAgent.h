#pragma once
#include <BWAPI.h>

class SoldierAgent {

public:

	BWAPI::Unit unit;
	BWAPI::Position targetPosition;
	bool attackCommand;

	SoldierAgent(BWAPI::Unit unit);
	void Update();
	void AttackOrder(BWAPI::Position);
	bool isDead();

};

