#include "MainModule.h"


using namespace BWAPI;
using namespace Filter;
using namespace std;


static bool GameOver;
HANDLE ghMutex;
bool test = false;

CentralAgent centralAgent;

void MainModule::onStart() {
	GameOver = false;

	ghMutex = CreateMutex(
		NULL,              // default security attributes
		FALSE,             // initially not owned
		NULL);             // unnamed mutex

	Broodwar << "The map is " << Broodwar->mapName() << "!" << std::endl;

	Broodwar->enableFlag(Flag::UserInput);
	
	Broodwar->setCommandOptimizationLevel(2);


	centralAgent = CentralAgent();

	CreateThread(NULL, 0, threadCentralAgent, NULL, 0, NULL);


}

void MainModule::onEnd(bool isWinner) {
	GameOver = true;
	if (isWinner) {
	}

	Sleep(100);
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
	if (!THREADED) {

		if (Broodwar->getFrameCount() % 10 == 0) {

			//centralAgent.Update();


			for (auto u : centralAgent.workers) {
				//u.Update();
			}


			for (auto u : centralAgent.army) {
				//u.Update();
			}


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
	DWORD dwWaitResult;

	dwWaitResult = WaitForSingleObject(
		ghMutex,
		100);


	if (IsAlly(unit) && unit->getType().isWorker()) {
		int pos = 0;
		for (auto w : centralAgent.workers) {
			if (w->unit == unit) {
				centralAgent.workers.erase(centralAgent.workers.begin() + pos);
				break;
			}
			pos++;
		}


	} else if (IsAlly(unit) && unit->getType() == UnitTypes::Zerg_Zergling) {
		int pos = 0;
		for (auto w : centralAgent.army) {
			if (w->unit == unit) {
				centralAgent.army.erase(centralAgent.army.begin() + pos);
				break;
			}
			pos++;
		}
	}


	ReleaseMutex(ghMutex);

}

void MainModule::onUnitMorph(BWAPI::Unit unit) {

	DWORD dwWaitResult;

	dwWaitResult = WaitForSingleObject(
		ghMutex,
		100);

	//Remove a Drone morphing into a building from workers
	if (IsAlly(unit) && IsBuilding(unit)) {
		
		int pos = 0;
		for (auto u : centralAgent.workers) {
			if (u->unit == unit) {
				centralAgent.workers.erase(centralAgent.workers.begin() + pos);
				break;
			}
			pos++;
		}
	}

	ReleaseMutex(ghMutex);
	
}

void MainModule::onUnitRenegade(BWAPI::Unit unit) {
}

void MainModule::onSaveGame(std::string gameName) {
}

void MainModule::onUnitComplete(BWAPI::Unit unit) {
	DWORD dwWaitResult;

	dwWaitResult = WaitForSingleObject(
		ghMutex,
		100);

	if (IsAlly(unit) && unit->getType().isWorker()) {
		CreateThread(NULL, 0, threadWorkerAgent, (LPVOID)unit, 0, NULL);
	
	} else if (IsBuilding(unit) && IsOwned(unit)) {
		centralAgent.buildings.insert(unit);

	} else if (unit->getType() == UnitTypes::Zerg_Zergling) {
		CreateThread(NULL, 0, threadSoldierAgent, (LPVOID)unit, 0, NULL);
		
	}

	ReleaseMutex(ghMutex);

}

DWORD WINAPI threadWorkerAgent(LPVOID param) {
	BWAPI::Unit unit = static_cast<BWAPI::Unit>(param);
	
	WorkerAgent * agent = new WorkerAgent(unit);

	DWORD dwWaitResult;

	dwWaitResult = WaitForSingleObject(
		ghMutex,
		100);

	centralAgent.workers.push_back(agent);

	if (!ReleaseMutex(ghMutex)) {

	}

	while (true) {

		dwWaitResult = WaitForSingleObject(
			ghMutex,
			100);

		if (GameOver || agent->unit == NULL || !agent->unit->exists()) {
			ReleaseMutex(ghMutex);
			return 0;
		}
		
		if (dwWaitResult == WAIT_OBJECT_0 || dwWaitResult == WAIT_ABANDONED) {
			agent->Update();
			if (!ReleaseMutex(ghMutex)) {
				
			}
		}
		
		Sleep(20);
	}
	
}


DWORD WINAPI threadSoldierAgent(LPVOID param) {
	BWAPI::Unit unit = static_cast<BWAPI::Unit>(param);

	SoldierAgent *agent = new SoldierAgent(unit);


	DWORD dwWaitResult;

	dwWaitResult = WaitForSingleObject(
		ghMutex,
		100);

	centralAgent.army.push_back(agent);

	if (!ReleaseMutex(ghMutex)) {

	}

	while (true) {

		dwWaitResult = WaitForSingleObject(
			ghMutex,
			100);

		if (GameOver || agent->unit == NULL || !agent->unit->exists()) {
			ReleaseMutex(ghMutex);
			return 0;
		}

		if (dwWaitResult == WAIT_OBJECT_0 || dwWaitResult == WAIT_ABANDONED) {
			agent->Update();
			if (!ReleaseMutex(ghMutex)) {

			}
		}

		Sleep(20);
	}

}

DWORD WINAPI threadCentralAgent(LPVOID param) {
	
	DWORD dwWaitResult;

	while (true) {

		dwWaitResult = WaitForSingleObject(
			ghMutex,
			100);

		if (GameOver) {
			ReleaseMutex(ghMutex);
			return 0;
		}
		
		if (dwWaitResult == WAIT_OBJECT_0 || dwWaitResult == WAIT_ABANDONED) {
			centralAgent.Update();
			if (!ReleaseMutex(ghMutex)) {
				
			}
		}
		Sleep(50);
	}

}