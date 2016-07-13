#include "WorkerAgent.h"

using namespace BWAPI;
using namespace Filter;


WorkerAgent::WorkerAgent(Unit unit) {
	this->unit = unit;
}

void WorkerAgent::Update() {
	if (unit == NULL || !unit->exists()) {
		return;
	}


	if (unit->getClosestUnit(IsEnemy, 100)) {
		unit->attack(unit->getClosestUnit(IsEnemy, 100));

	}

	if (unit->isIdle()) {
		if (unit->isCarryingGas() || unit->isCarryingMinerals()) {
			unit->returnCargo();

		} else {
			unit->gather(unit->getClosestUnit(IsMineralField || IsRefinery));
		
		}
	}
}

bool WorkerAgent::isDead() {
	return (this->unit == NULL || !this->unit->exists());
}