#include "Camera.h"

Camera::Camera():viewDirection(0.0f, 0.0f, -1.0f) {

}

glm::mat4 Camera::worldToView() const {
	return glm::lookAt(position, position + viewDirection, glm::vec3(0.0f, 1.0f, 0.0f));
}