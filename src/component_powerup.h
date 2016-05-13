#pragma once
#include "common.h"
#include "component.h"
#include "mesh.h"
#include "resource.h"

struct Mesh;

namespace Components {

	class CmPowerUp : public Component {
	protected:
		// RenderObject* _ro;
		Mesh *mesh_;
		float wobble_;
	public:
		vec3 startPos;
		CmPowerUp();
		~CmPowerUp();
		void Update(double delta);
	};
}
