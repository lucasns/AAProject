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
	if (army.size() >= 6) {
		for (auto &u : army) {
			u.AttackOrder(attackPosition);

		}
	}

}

void CentralAgent::BaseManagement() {
	//Rewrite this!!!!

	bool isBuildingSupply = false;

	if (Broodwar->self()->supplyUsed() + 1 > Broodwar->self()->supplyTotal()) {
		UnitType supply = UnitTypes::Zerg_Overlord;


		int currSupply = 0;

		for (auto u : Broodwar->self()->getUnits()) {
			if (u->isMorphing()) {
				if (u->getBuildType() == supply) {
					isBuildingSupply = true;
					currSupply++;
					break;
				}
			}
		}

		for (auto u : Broodwar->self()->getUnits()) {
			if (u->getType() == supply) {
				currSupply++;
			}

		}

		currSupply *= 16;
		currSupply += 2;


		if (Broodwar->self()->supplyUsed() + 1 > currSupply) {

			Unit supplyBuilder = Broodwar->getClosestUnit(basePos, GetType == supply.whatBuilds().first &&
				(IsIdle || IsGatheringMinerals) &&
				IsOwned);


			if (supplyBuilder) {
				supplyBuilder->train(supply);
			}

		}

	}
	


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
