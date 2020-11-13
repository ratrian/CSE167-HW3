#ifndef _POINT_LIGHT_H_
#define _POINT_LIGHT_H_

#include "Object.h"

using namespace std;

class PointLight
{
private:
	/*
	 * Attenuation vector consists of
	 *     atten.x = constant factor
	 *     atten.y = linear factor
	 *     atten.z = quadratic factor
	 */
	glm::vec3 pos, color, atten;

public:
	PointLight(glm::vec3 pos, glm::vec3 color, glm::vec3 atten);

	void sendLightToShader(GLuint shader);
	glm::vec3 getPos();
	void orbit(glm::vec3 direction, float rotAngle, glm::vec3 rotAxis);
	void move(glm::vec3 t);
};

#endif