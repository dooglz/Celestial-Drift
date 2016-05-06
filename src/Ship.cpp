#include "ship.h"
#include "entity.h"
#include "component_npc_shipdriver.h"
#include "component_player_shipdriver.h"
#include "component_mesh_renderer.h"

void BuildShipPlayer(Entity* ent){
  *ent = Entity();
  ent->SetName("player ship");
  //TODO: cleanup delete these
  Components::CmMeshRenderer* mr = new Components::CmMeshRenderer();
  Components::CmPlayerShipDriver* dr = new Components::CmPlayerShipDriver();
  ent->AddComponent(*mr);
  ent->AddComponent(*dr);
  mr->SetMesh("ship1.obj");
}

void BuildShipNpc(Entity* ent){
  *ent = Entity();
  ent->SetName("npc ship");
  //TODO: cleanup delete these
  Components::CmMeshRenderer* npcrmr = new Components::CmMeshRenderer();
  Components::CmNPCShipDriver* npcdr = new Components::CmNPCShipDriver();
  ent->AddComponent(*npcrmr);
  ent->AddComponent(*npcdr);
  npcrmr->SetMesh("ship1.obj");
}