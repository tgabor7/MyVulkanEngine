#pragma once
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "input_handler.hpp"

namespace myve
{
	class Camera
	{
	public:
		Camera();
		glm::mat4 getProjectionMatrix(float fovy, float near, float far);
		glm::mat4 getViewMatrix();
		void setPosition(const glm::vec3& position);
		void translate(const glm::vec3& v);
		void look(const glm::vec3& v);
		void moveYaw(const double &d);
		void movePitch(const double& d);

		double pitch = 0.0;
		double yaw = 0.0;

		~Camera();
	private:
		glm::vec3 position{0.f,5.f,0.f};
		glm::vec3 direction{0.f,-1.f,0};
		glm::vec3 up{ 0,0,1.f };
	};
}