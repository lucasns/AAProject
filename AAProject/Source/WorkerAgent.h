#pragma once
#include <BWAPI.h>

class WorkerAgent {

public:
	
	BWAPI::Unit unit;
	BWAPI::Position targetPosition;
	bool attackCommand;
	bool isExplorer;

	WorkerAgent(BWAPI::Unit unit);
	void Update();
	bool isDead();
	void AttackOrder(BWAPI::Position);
	void explore();

};

