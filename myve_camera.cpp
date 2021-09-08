#include "myve_camera.hpp"
#include "myve_window.hpp"

namespace myve
{
	glm::mat4 Camera::getViewMatrix()
	{
		const glm::vec3 w{ glm::normalize(direction) };
		const glm::vec3 u{ glm::normalize(glm::cross(w, up)) };
		const glm::vec3 v{ glm::cross(w, u) };

		glm::mat4 viewMatrix = glm::mat4{ 1.f };
		viewMatrix[0][0] = u.x;
		viewMatrix[1][0] = u.y;
		viewMatrix[2][0] = u.z;
		viewMatrix[0][1] = v.x;
		viewMatrix[1][1] = v.y;
		viewMatrix[2][1] = v.z;
		viewMatrix[0][2] = w.x;
		viewMatrix[1][2] = w.y;
		viewMatrix[2][2] = w.z;
		viewMatrix[3][0] = -glm::dot(u, position);
		viewMatrix[3][1] = -glm::dot(v, position);
		viewMatrix[3][2] = -glm::dot(w, position);

		return viewMatrix;
	}
	void Camera::setPosition(const glm::vec3& _position)
	{
		position = _position;
	}
	void Camera::look(const glm::vec3& v)
	{
		printf("(%f,%f,%f)\n", direction.x,direction.y, direction.z);
		direction = v;
	}
	void Camera::moveYaw(const double &d)
	{
		yaw += d;

		direction.x = cos(yaw)*cos(pitch);
		direction.y = sin(yaw)*cos(pitch);
	}
	void Camera::movePitch(const double& d)
	{
		pitch += d;
		
		direction.z = sin(pitch);
	}
	void Camera::translate(const glm::vec3& v)
	{
		position += v;
	}
	glm::mat4 Camera::getProjectionMatrix(float fovy, float near, float far)
	{

		const float tanHalfFovy = tan(fovy / 2.f);
		
		const double aspect = (static_cast<double>(Window::WIDTH) / static_cast<double>(Window::HEIGHT));
		glm::mat4 projectionMatrix = glm::mat4{ 0.0f };
		projectionMatrix[0][0] = 1.f / (aspect * tanHalfFovy);
		projectionMatrix[1][1] = 1.f / (tanHalfFovy);
		projectionMatrix[2][2] = far / (far - near);
		projectionMatrix[2][3] = 1.f;
		projectionMatrix[3][2] = -(far * near) / (far - near);

		return projectionMatrix;
	}
	Camera::Camera()
	{

	}
	Camera::~Camera()
	{

	}
}