#ifndef CAMERA_H
#define CAMERA_H


#include <vector>
#include <glm/glm.hpp>

#define PI 3.14159265358979323846264338327950288

class Camera{
private:
	glm::vec3 Position;
	glm::mat4 ProjectionMatrix;								// The camera's projection matrix										// The camera's position

	float Speed = 50.0;									// The camera's speed that is used with a time slice
	float RotationSpeed = 2;							// The rotation speed for the camera
	double MouseSpeed = 0.001;							// The speed for the mouse rotations
	float Yaw = 0;										// The horizontal rotation angle (in radians), y-axis
	float Pitch = 0;									// The vertical rotation angle (in radians, x-axis
public:
	Camera();
	~Camera(){}

	// This sets and returns a perspective matrix that is build from the field of fiew, aspect ratio and near/far planes
	glm::mat4 SetPerspective(float fov, float aspectRatio, float nearPlane, float farPlane);

	// This returns the current ProjectionMatrix
	glm::mat4 GetProjectionMatrix() { return ProjectionMatrix; }

	// This returns the current view matrix according to the camera's position and rotation
	glm::mat4 GetViewMatrix();

	glm::mat4 GetRotationMatrix();

	glm::mat4 Camera::GetViewMatrix(glm::vec3 direction);

	// This sets and gets the camera's position
	void SetPosition(glm::vec3 position) { Position = position; }
	glm::vec3 GetPosition() { return Position; }

	// This calculates the current view vector from the rotation matrix (hard coded for now)
	glm::vec3 GetView();

	// This returns the camera's right vector (the direction perpendicular to the view and up vectors)
	glm::vec3 GetRight();

	// This sets and gets the Yaw and Pitch rotations (in radians)
	float GetYaw() { return Yaw; }
	void SetYaw(float yaw) { Yaw = yaw; }
	float GetPitch() { return Pitch; }
	void SetPitch(float pitch) { Pitch = pitch; }

	// This sets and gets the camera's movement speed
	void SetSpeed(double speed) { Speed = speed; }
	double GetSpeed() { return Speed; }

	// This sets the Position and the yaw and pitch rotations (in radians)
	void PositionCamera(float positionX, float positionY, float positionZ, float yaw, float pitch);

	// This gets and sets the rotation speed for the camera
	void SetRotationSpeed(double speed) { RotationSpeed = speed; }
	double GetRotationSpeed() { return RotationSpeed; }

	// This sets the pitch and yaw of our camera from the mouse x and y deltas from the last frame
	void SetViewByMouse(float mouseX, float mouseY);


	// This will move the camera forward or backwards depending on the speed (negative for backwards)
	void MoveCamera(float speed);

	void MoveUp(float speed);

	void CameraKeyMove(bool keys[], float deltaTime);

	// This strafes the camera left or right depending on the speed
	void Strafe(float speed);
};

#endif
