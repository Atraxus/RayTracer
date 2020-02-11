#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
public:
	Camera(int width, int height, float fovY, glm::vec3 pos, glm::vec3 lookAt, glm::vec3 up);
	
	// setter
	void setFov(float inputFov);
	void setPosition(glm::vec3 inputPosition);
	void setLookAt(glm::vec3 lookAt);
	void setViewDirection(glm::vec3 viewDir);
	
	// getter
	glm::mat4 getView() const;
	inline glm::mat4 getProj() const { return glm::perspective(m_Fov, 1.33f, 0.1f, 100.f); };
	inline glm::vec3 getPosition() const { return m_Position; };
	inline glm::vec3 getViewDirection() const { return m_ViewDirection; };
	inline glm::vec3 getUp() const { return m_Up; };
	inline glm::vec3 getRight() const { return m_Right; };
	inline float getFovX() const { return m_FovX; };
	inline float getFovY() const { return m_FovY; };
private:

	int	m_Width;
	int m_Height;
	float m_Fov;
	float m_FovY;
	float m_FovX;

	glm::vec3 m_Position;
	glm::vec3 m_LookAt;
	glm::vec3 m_ViewDirection;
	glm::vec3 m_Up;
	glm::vec3 m_Right;

};