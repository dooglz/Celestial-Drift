#include "component_powerup.h"
#include "entity.h"


namespace Components {
	CmPowerUp::CmPowerUp() :wobble_(0.0f),startPos(vec3()), Component("Powerup"){}

	CmPowerUp::~CmPowerUp(){};

	void CmPowerUp::Update(double delta){
		wobble_ +=  delta*0.5f;
		float up = sin(wobble_)* 2.5f;
		Ent_->SetPosition(startPos + vec3(0, up, 0));
		Ent_->SetRotation(Ent_->GetRotation() * angleAxis((float)delta, vec3(0, 1.0, 0)));
	};
}
