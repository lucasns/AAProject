#pragma once
#include <BWAPI.h>
#include <vector>
#include "SoldierAgent.h"
#include "WorkerAgent.h"

class CentralAgent {

private:
	void checkSupply();
	BWAPI::Unit & getBuilder();


public:


	//Units

	BWAPI::Unitset scouts;
	BWAPI::Unitset buildings;

	std::vector<WorkerAgent*> workers;
	std::vector<SoldierAgent*> army;


	//Map Information
	BWAPI::Position basePos;
	BWAPI::Position enemyBase;


	//Base Information

	bool pool = false;
	int numWorkers;


	//Army Information
	BWAPI::Position waitPos;
	BWAPI::Position attackPosition;


	CentralAgent();
	void Update();
	void ArmyManagement();
	void BaseManagement();
	void addWorker(WorkerAgent * worker);
	void removeWorker(WorkerAgent * worker);
	void addSoldier(SoldierAgent * soldier);
	void removeSoldier(SoldierAgent * soldier);

	

	
};

