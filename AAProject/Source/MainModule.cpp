#include "MainModule.h"


using namespace BWAPI;
using namespace Filter;
using namespace std;


//Central Agent attr -------------------------------------------------------------

//Units

Unitset scouts;
Unitset workers;
Unitset buildings;
Unitset army;


//Map Information
Position basePos = Position(-1, -1);
Position enemyBase = Position(-1, -1);


//Base Information



bool pool = false;
vector<BWAPI::UnitType> buildOrder;

int numWorkers = 4;



//Army Information
Position waitPos;
Position attackPosition;
bool attackCommand = false;



//-------------------------------------------------------------------



void MainModule::onStart() {
	Broodwar << "The map is " << Broodwar->mapName() << "!" << std::endl;

	Broodwar->enableFlag(Flag::UserInput);
	
	Broodwar->setCommandOptimizationLevel(2);

	for (auto l : Broodwar->getStartLocations()) {
		if (!Broodwar->isExplored(l)) {
			enemyBase = (Position)l;
		} else {
			basePos = (Position)l;
		}
	}

	buildOrder.insert(buildOrder.begin(), UnitTypes::Zerg_Spawning_Pool);


}

void MainModule::onEnd(bool isWinner) {
	if (isWinner) {
	}
}

void MainModule::onFrame() {


	//Debug messages

	Broodwar->drawTextScreen(20, 20, "N workers: %d", workers.size());
	Broodwar->drawTextScreen(20, 40, "N army: %d", army.size());
	Broodwar->drawTextScreen(20, 60, "N scouts: %d", scouts.size());
	
	Broodwar->drawTextScreen(200, 0, "FPS: %d", Broodwar->getFPS());
	Broodwar->drawTextScreen(200, 20, "Average FPS: %f", Broodwar->getAverageFPS());
	Broodwar->drawTextScreen(200, 40, "N frames: %d", Broodwar->getFrameCount());
	
	//

	CentralAgent();

	for (auto &u : Broodwar->self()->getUnits()) {
		if (u->getType().isWorker()) {
			WorkerAgent(u);

		} else if (u->getType() == UnitTypes::Zerg_Zergling) {
			SoldierAgent(u);
		}
	}


}

void MainModule::onSendText(std::string text) {
}

void MainModule::onReceiveText(BWAPI::Player player, std::string text) {
}

void MainModule::onPlayerLeft(BWAPI::Player player) {
}

void MainModule::onNukeDetect(BWAPI::Position target) {
}

void MainModule::onUnitDiscover(BWAPI::Unit unit) {
}

void MainModule::onUnitEvade(BWAPI::Unit unit) {
}

void MainModule::onUnitShow(BWAPI::Unit unit) {
}

void MainModule::onUnitHide(BWAPI::Unit unit) {
}

void MainModule::onUnitCreate(BWAPI::Unit unit) {
}

void MainModule::onUnitDestroy(BWAPI::Unit unit) {
}

void MainModule::onUnitMorph(BWAPI::Unit unit) {
}

void MainModule::onUnitRenegade(BWAPI::Unit unit) {
}

void MainModule::onSaveGame(std::string gameName) {
}

void MainModule::onUnitComplete(BWAPI::Unit unit) {
	if (IsAlly(unit) && unit->getType().isWorker()) {
		workers.insert(unit);
	} else if (IsBuilding(unit)) {
		buildings.insert(unit);
	} else if (unit->getType() == UnitTypes::Zerg_Zergling) {
		army.insert(unit);
	}
}



//Agent Actions

void WorkerAgent(BWAPI::Unit unit) {
	if (unit->getClosestUnit(IsEnemy, 100)) {
		unit->attack(unit->getClosestUnit(IsEnemy, 100));
	}

	if (unit->isIdle()) {
		if (unit->isCarryingGas() || unit->isCarryingMinerals()) {
			unit->returnCargo();

		} else if (!unit->getPowerUp()) {
			if (!unit->gather(unit->getClosestUnit(IsMineralField || IsRefinery))) {
				Broodwar << Broodwar->getLastError() << std::endl;
			}

		}
	}
}


void SoldierAgent(BWAPI::Unit unit) {
	
	if (attackCommand) {
		if (unit->getClosestUnit(IsEnemy && CanAttack && !IsWorker, 1000)) {
			unit->attack(unit->getClosestUnit(IsEnemy && CanAttack && !IsWorker, 1000));
		} else if (unit->getClosestUnit(IsEnemy && CanAttack, 1000)) {
			unit->attack(unit->getClosestUnit(IsEnemy && CanAttack, 1000));

		} else if (unit->getClosestUnit(IsEnemy, 1000)) {
			unit->attack(unit->getClosestUnit(IsEnemy, 1000));

		} else {
			unit->attack(attackPosition);
		}

	} else {
		unit->attack(unit->getClosestUnit(IsEnemy, 2000));
	}
}


void CentralAgent() {
	ArmyManagement();
	BaseManagement();
}

void ArmyManagement() {
	attackPosition = enemyBase;
	if (army.size() >= 6) {
		attackCommand = true;
	} else {
		attackCommand = false;
	}
}

void BaseManagement() {
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

			if (u->isIdle() && workers.size() + n <= numWorkers && u->getClosestUnit(IsOwned && GetType == UnitTypes::Zerg_Spawning_Pool)) {
				u->train(u->getType().getRace().getWorker());
			}
		

		}
	}

}
