#include "SoldierAgent.h"
#include "MainModule.h"

using namespace BWAPI;
using namespace Filter;


SoldierAgent::SoldierAgent(Unit unit) {
	this->unit = unit;
	this->attackCommand = false;
	this->targetPosition = Position(-1, -1);
	
	
}

void SoldierAgent::Update() {
	if (unit == NULL || !unit->exists()) {
		return;
	}
	
	if (attackCommand) {
		if (unit->getClosestUnit(IsEnemy && CanAttack && !IsWorker, 1000)) {
			unit->attack(unit->getClosestUnit(IsEnemy && CanAttack && !IsWorker, 1000));

		} else if (unit->getClosestUnit(IsEnemy && CanAttack, 1000)) {
			unit->attack(unit->getClosestUnit(IsEnemy && CanAttack, 1000));

		} else if (unit->getClosestUnit(IsEnemy, 1000)) {
			unit->attack(unit->getClosestUnit(IsEnemy, 1000));

		} else {
			unit->attack(targetPosition);
		}

	} else {
		unit->attack(unit->getClosestUnit(IsEnemy, 500));
	}
}

void SoldierAgent::AttackOrder(Position targetPosition) {
	this->attackCommand = true;
	this->targetPosition = targetPosition;
	

}

void SoldierAgent::RetreatOrder(Position targetPosition) {
	this->attackCommand = false;
	this->targetPosition = targetPosition;


}

bool SoldierAgent::isDead() {
	return (this->unit == NULL || !this->unit->exists());
}
