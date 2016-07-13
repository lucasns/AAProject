#pragma once
#include <BWAPI.h>
#include <Windows.h>
#include <vector>
#include "WorkerAgent.h"
#include "SoldierAgent.h"
#include "CentralAgent.h"


static bool THREADED = true;
static bool DEBUG_MODE = FALSE;

DWORD WINAPI threadWorkerAgent(LPVOID param);
DWORD WINAPI threadSoldierAgent(LPVOID param);
DWORD WINAPI threadCentralAgent(LPVOID param);

class MainModule : public BWAPI::AIModule {

public:

  virtual void onStart();
  virtual void onEnd(bool isWinner);
  virtual void onFrame();
  virtual void onSendText(std::string text);
  virtual void onReceiveText(BWAPI::Player player, std::string text);
  virtual void onPlayerLeft(BWAPI::Player player);
  virtual void onNukeDetect(BWAPI::Position target);
  virtual void onUnitDiscover(BWAPI::Unit unit);
  virtual void onUnitEvade(BWAPI::Unit unit);
  virtual void onUnitShow(BWAPI::Unit unit);
  virtual void onUnitHide(BWAPI::Unit unit);
  virtual void onUnitCreate(BWAPI::Unit unit);
  virtual void onUnitDestroy(BWAPI::Unit unit);
  virtual void onUnitMorph(BWAPI::Unit unit);
  virtual void onUnitRenegade(BWAPI::Unit unit);
  virtual void onSaveGame(std::string gameName);
  virtual void onUnitComplete(BWAPI::Unit unit);


 
 
};

void WorkerAgentAI(BWAPI::Unit unit);
void SoldierAgentAI(BWAPI::Unit unit);
void CentralAgentAI();

void ArmyManagement();
void BaseManagement();
