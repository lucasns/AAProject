#pragma once
#include <BWAPI.h>

class WorkerAgent {

public:
	
	BWAPI::Unit unit;

	WorkerAgent(BWAPI::Unit unit);
	void Update();

};

