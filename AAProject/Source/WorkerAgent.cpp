#include "WorkerAgent.h"

using namespace BWAPI;
using namespace Filter;


WorkerAgent::WorkerAgent(Unit unit) {
	this->unit = unit;
	this->attackCommand = false;
	this->targetPosition = Position(-1, -1);
	this->isExplorer = false;
}

void WorkerAgent::Update() {
	if (unit == NULL || !unit->exists()) {
		return;
	}

	

	if (!attackCommand) {
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

	} else { //Worker attack mode
		if (unit->getClosestUnit(IsEnemy && CanAttack && !IsWorker, 1000)) {
			unit->attack(unit->getClosestUnit(IsEnemy && CanAttack && !IsWorker, 1000));

		} else if (unit->getClosestUnit(IsEnemy && CanAttack, 1000)) {
			unit->attack(unit->getClosestUnit(IsEnemy && CanAttack, 1000));

		} else if (unit->getClosestUnit(IsEnemy, 1000)) {
			unit->attack(unit->getClosestUnit(IsEnemy, 1000));

		} else {
			unit->attack(targetPosition);
		}
	}
}

void WorkerAgent::AttackOrder(Position targetPosition) {
	this->attackCommand = true;
	this->targetPosition = targetPosition;

}


bool WorkerAgent::isDead() {
	return (this->unit == NULL || !this->unit->exists());
}