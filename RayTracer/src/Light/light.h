#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"

class light {
public:
	light();
	light(glm::vec3 inputPosition);
	light(glm::vec3 inputPosition, float inputStrength);

	float getBrightness(glm::vec3 inputLocation, float inputColor);
	float getX();
	float getY();
	float getZ();
	glm::vec3 getPos();
private:
	glm::vec3 position;
	float strength;


};