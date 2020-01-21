#include "component_mesh_renderer.h"
#include "component_npc_shipdriver.h"
#include "component_powerup.h"
#include "component_track.h"
#include "component_player_shipdriver.h"
#include "Entity.h"
#include "ship.h"

void BuildShipPlayer(Entity *ent) {
  *ent = Entity();
  ent->SetName("player ship");
  // TODO: cleanup delete these
  Components::CmMeshRenderer *mr = new Components::CmMeshRenderer();
  Components::CmPlayerShipDriver *dr = new Components::CmPlayerShipDriver(0);
  ent->AddComponent(*mr);
  ent->AddComponent(*dr);
  mr->SetMesh("ship1.obj");
}

void BuildShipNpc(Entity *ent) {
  *ent = Entity();
  ent->SetName("npc ship");
  // TODO: cleanup delete these
  Components::CmMeshRenderer *npcrmr = new Components::CmMeshRenderer();
  Components::CmNPCShipDriver *npcdr = new Components::CmNPCShipDriver();
  ent->AddComponent(*npcrmr);
  ent->AddComponent(*npcdr);
  npcrmr->SetMesh("ship1.obj");
}

void BuildPowerup(Entity *ent){
	*ent = Entity();
	ent->SetName("Powerup");
	// TODO: cleanup delete these
	Components::CmMeshRenderer *mr = new Components::CmMeshRenderer();
	Components::CmPowerUp *pwrup = new Components::CmPowerUp();
	ent->AddComponent(*mr);
	ent->AddComponent(*pwrup);
	mr->SetMesh("sphereI2.obj");

	const int wps = rand() % Components::CmTrack::waypoints.size();
	ent->SetPosition(Components::CmTrack::waypoints[wps]);
	pwrup->startPos = ent->GetPosition();
}