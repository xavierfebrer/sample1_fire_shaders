#version 330 core

in vec2 texFragment;

out vec4 out_color;

uniform sampler2D tex1;
uniform sampler2D tex2;

uniform float fireHeight;
uniform float leftSmooth;
uniform float rightSmooth;
uniform bool toon;
uniform float blackCorrectionThreshold;
uniform float speed;
uniform float time;

vec4 smoothLeft (vec4 color, vec2 coords, float leftPercent) {
	// if the zone is the left side, start to apply an alpha gradient
	if (coords.x > 1.0 - leftPercent) {
		float piece = coords.x - (1.0 - leftPercent);					// range [0, leftPercent]
		float percent = smoothstep(0, 1, 1 - (piece / leftPercent));	// transform piece to range [0, 1]
		color.a = color.a * percent;									// tune down the current color alpha
	}
	return color;
}

vec4 smoothRight (vec4 color, vec2 coords, float rightPercent) {
	// if the zone is the right side, start to apply an alpha gradient
	if (coords.x < rightPercent) {
		float piece = rightPercent - coords.x;							// range [0, rightPercent]
		float percent = smoothstep(0, 1, 1 - (piece / rightPercent));	// transform piece to range [0, 1]
		color.a = color.a * percent;									// tune down the current color alpha
	}
	return color;
}

vec4 correctBlackFire(vec4 color, float threshold) {
	// if a black spot in the fire is detected, make it transparent
	if((color.x + color.y + color.z) / 3.0 < threshold){
		color.a = 0.0;	// a gradient in range [0, threshold] using something like smoothstep(...) could also be considered to make the black spots visible but less noticeable because of the gradient, but for now it seems OK this way
	}
	return color;
}

vec4 getToonColor1(vec4 color){
	// like greyscale human perceived brightnes (adjusted degrees of RGB).
	// A modified version is used to too adjust the colors to a red toon tone.
	float avgFinalColor = 0.7152*color.r + 0.2126*color.g + 0.0722*color.b;

	// set the toon degrees, splitting too much can create a false toon perception
	if (avgFinalColor > 0.95) {
		color = vec4(1.0,0.6,0.4,color.w);
	} else if (avgFinalColor > 0.5) {
		color = vec4(0.75,0.35,0.25,color.w);
	} else if (avgFinalColor > 0.25) {
		color = vec4(0.5,0.25,0.15,color.w);
	} else {
		color = vec4(0.0,0.0,0.0,color.w);
	}
	return color;
}

vec4 getFireColor(vec2 coords) {
	vec2 movingCoords = coords;
	movingCoords.y -= speed * time;
	vec2 movingCoords2 = coords;
	movingCoords2.y -= (speed + 0.4) * time;

	vec3 fireShapes1 = texture(tex1, vec2(movingCoords2.x, movingCoords2.y)).rgb;
	vec3 fireShapes2 = texture(tex2, vec2(movingCoords.x, movingCoords.y)).rgb;
	vec3 fireShapes = vec3((fireShapes1.r + fireShapes2.r) / 2,
							(fireShapes1.r + fireShapes2.g) / 2,
							(fireShapes1.r + fireShapes2.b) / 2);
	fireShapes = fireShapes * vec3(2.0, 1.575, 1.1);

	float fireZones = smoothstep(0, 1, 1.0-coords.y);
	float alpha = fireZones;

	vec4 color = vec4(fireShapes, alpha);

	return color;
}

void main() {

	// plane coords upside down
	vec2 coords = vec2(1.0-texFragment.x, 1.0-texFragment.y);

	// get the flame color (high alpha) or empty (low alpha)
	vec4 finalColor = getFireColor(coords);

	// left attenuation correction
	finalColor = smoothLeft(finalColor, coords, leftSmooth);

	// right attenuation correction
	finalColor = smoothRight(finalColor, coords, rightSmooth);
	
	// black fire color correction
	finalColor = correctBlackFire(finalColor, blackCorrectionThreshold);

	// apply toonish colored fire
	if (toon) {
		finalColor = getToonColor1(finalColor);
	}
 
	out_color = finalColor;
}