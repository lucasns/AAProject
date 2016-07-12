#include "MainModule.h"


using namespace BWAPI;
using namespace Filter;
using namespace std;






void MainModule::onStart() {
	Broodwar << "The map is " << Broodwar->mapName() << "!" << std::endl;

	Broodwar->enableFlag(Flag::UserInput);
	
	Broodwar->setCommandOptimizationLevel(2);

	Unit a;

	for (auto u : Broodwar->getAllUnits()) {
		a = u;
		break;
	}

	centralAgent = CentralAgent();


}

void MainModule::onEnd(bool isWinner) {
	if (isWinner) {
	}
}

void MainModule::onFrame() {


	//Debug messages

	Broodwar->drawTextScreen(20, 20, "N workers: %d", centralAgent.workers.size());
	Broodwar->drawTextScreen(20, 40, "N army: %d", centralAgent.army.size());
	Broodwar->drawTextScreen(20, 60, "N buildings: %d", centralAgent.buildings.size());
	
	Broodwar->drawTextScreen(200, 0, "FPS: %d", Broodwar->getFPS());
	Broodwar->drawTextScreen(200, 20, "Average FPS: %f", Broodwar->getAverageFPS());
	Broodwar->drawTextScreen(200, 40, "N frames: %d", Broodwar->getFrameCount());
	
	//

	if (Broodwar->getFrameCount() % 10 == 0) {
		
		centralAgent.Update();


		for (auto u : centralAgent.workers) {
			u.Update();
		}

		
		for (auto u : centralAgent.army) {
			u.Update();
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
	if (IsAlly(unit) && unit->getType().isWorker()) {
		int pos = 0;
		for (auto w : centralAgent.workers) {
			if (w.unit == unit) {
				centralAgent.workers.erase(centralAgent.workers.begin() + pos);
				break;
			}
			pos++;
		}


	}
}

void MainModule::onUnitMorph(BWAPI::Unit unit) {
	//Remove a Drone morphing into a building from workers
	if (IsAlly(unit) && IsBuilding(unit)) {
		
		int pos = 0;
		for (auto u : centralAgent.workers) {
			if (u.unit == unit) {
				centralAgent.workers.erase(centralAgent.workers.begin() + pos);
				break;
			}
			pos++;
		}
	} else if (IsAlly(unit) && unit->getBuildType() == UnitTypes::Zerg_Overlord) {
		

	}
	
}

void MainModule::onUnitRenegade(BWAPI::Unit unit) {
}

void MainModule::onSaveGame(std::string gameName) {
}

void MainModule::onUnitComplete(BWAPI::Unit unit) {
	if (IsAlly(unit) && unit->getType().isWorker()) {
		centralAgent.workers.push_back(WorkerAgent(unit));

	} else if (IsBuilding(unit) && IsOwned(unit)) {
		centralAgent.buildings.insert(unit);

	} else if (unit->getType() == UnitTypes::Zerg_Zergling) {
		centralAgent.army.push_back(SoldierAgent(unit));
		
	}
}

