#pragma once

#include "Object3D.h"

class FireObj : public Object3D {
public:
	float fireHeight = 0.3f;
	float speed = 0.3f;
	float leftSmooth = 0.1f;
	float rightSmooth = 0.1f;
	bool toon = false;
	float blackCorrectionThreshold = 0.0;

	FireObj();
	virtual ~FireObj() override;
};