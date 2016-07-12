#include "CentralAgent.h"

using namespace BWAPI;
using namespace Filter;


CentralAgent::CentralAgent() {
	this->basePos = Position(-1, -1);
	this->enemyBase = Position(-1, -1);
	
	this->pool = false;
	this->numWorkers = 4;

	this->buildOrder.insert(buildOrder.begin(), UnitTypes::Zerg_Spawning_Pool);


	
}


Unit & CentralAgent::getBuilder() {
	//TODO: Get from workers vector
	Unit builder = Broodwar->getClosestUnit(basePos, GetType == UnitTypes::Zerg_Drone &&
		(IsIdle || IsGatheringMinerals) &&
		IsOwned);

	return builder;
}

void CentralAgent::checkSupply() {
	if (Broodwar->self()->supplyUsed() + 1 > Broodwar->self()->supplyTotal()) {
		UnitType supply = UnitTypes::Zerg_Overlord;
		bool isBuildingSupply = false;

		for (auto u : Broodwar->self()->getUnits()) {
			if (u->isMorphing()) {
				if (u->getBuildType() == supply) {
					isBuildingSupply = true;

				}
			}
		}

		
		if (!isBuildingSupply) {
			Unit supplyBuilder = getBuilder();

			if (supplyBuilder) {
				supplyBuilder->train(supply);
			}

		}
	}
}

void CentralAgent::Update() {
	if (basePos == Position(-1, -1)) {
		for (auto l : Broodwar->getStartLocations()) {
			if (!Broodwar->isExplored(l)) {
				this->enemyBase = (Position)l;
			} else {
				this->basePos = (Position)l;
			}
		}
	}


	this->ArmyManagement();
	this->BaseManagement();
}



void CentralAgent::ArmyManagement() {
	attackPosition = enemyBase;
	if (army.size() >= 20) {
		for (auto &u : army) {
			u.AttackOrder(attackPosition);

		}
	}

}

void CentralAgent::BaseManagement() {
	//Rewrite this!!!!
		
	checkSupply();
	
	UnitType nextBuilding = buildOrder.at(0);
	Unit builder;

	
	if (Broodwar->self()->minerals() >= (nextBuilding.mineralPrice()) && !pool) {

		Unit builder = Broodwar->getClosestUnit(basePos, GetType == nextBuilding.whatBuilds().first &&
			(IsIdle || IsGatheringMinerals) &&
			IsOwned);

		
		if (builder) {
			TilePosition targetBuildLocation = Broodwar->getBuildLocation(nextBuilding, builder->getTilePosition());
			builder->build(nextBuilding, targetBuildLocation);
		}

		pool = true;

	}
	
	for (auto &u : buildings) {

		if (u->getType().isResourceDepot()) {

			if (basePos.x == -1) {
				basePos = u->getPosition();
			}


			if (u->isIdle() && pool) {
				u->train(UnitTypes::Zerg_Zergling);

			}


			int n = 0;

			for (auto u : Broodwar->self()->getUnits()) {
				if (u->isMorphing()) {
					if (u->getBuildType() == UnitTypes::Zerg_Drone) {
						n++;
						break;
					}
				}
			}

			bool sp = false;

			if (u->isIdle() && workers.size() + n < numWorkers && u->getClosestUnit(IsOwned && GetType == UnitTypes::Zerg_Spawning_Pool)) {
				u->train(u->getType().getRace().getWorker());
			}


		}
	}
	
}
