#ifndef DENGINE_SCENE_OBJECTS_CAMERA
#define DENGINE_SCENE_OBJECTS_CAMERA

#include <glm/glm.hpp>

typedef float GLfloat;

namespace deng {

	class Camera
	{
	public:
		Camera(glm::vec3 position);
		glm::mat4 GetViewMatrix();

		//Movements 
		void MoveForward(GLfloat distance);
		void MoveBack(GLfloat distance);
		void MoveRight(GLfloat distance);
		void MoveLeft(GLfloat distance);
		void Turn(GLfloat dpitch,GLfloat dyaw);

	private:
		glm::vec3 _position;
		glm::vec3 _front;
		glm::vec3 _up;

		GLfloat _pitch;
		GLfloat _yaw;
	};
}

#endif // !DENGINE_SCENE_OBJECTS_CAMERA
