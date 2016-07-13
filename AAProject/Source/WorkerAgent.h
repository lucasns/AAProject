#pragma once
#include <BWAPI.h>

class WorkerAgent {

public:
	
	BWAPI::Unit unit;
	BWAPI::Position targetPosition;
	bool attackCommand;

	WorkerAgent(BWAPI::Unit unit);
	void Update();
	bool isDead();
	void AttackOrder(BWAPI::Position);

};

