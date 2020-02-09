#include "light.h"

light::light(){
	position = glm::vec3(0.0f, 0.0f, 0.0f);
	strength = 1000;
}

light::light(glm::vec3 inputPosition)
{
	position = inputPosition;
	strength = 1000;
}

light::light(glm::vec3 inputPosition, float inputStrength)
{
	position = inputPosition;
	strength = inputStrength;
}

float light::getBrightness(glm::vec3 inputLocation,float inputColor)
{
	float distance = glm::distance(inputLocation, position);
	float color;
	if (strength - distance > 0)
		color = inputColor * ((strength - distance) / strength);
	else
		color = 0.0f;
	return color;
}

float light::getX()
{
	return position.x;
}

float light::getY()
{
	return position.y;
}

float light::getZ()
{
	return position.z;
}

glm::vec3 light::getPos()
{
	return position;
}
