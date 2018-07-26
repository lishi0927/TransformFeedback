#include "camera.h"
#include <glfw3.h>
#include <glm/gtc/matrix_transform.hpp>


Camera::Camera()
{
	Position = glm::vec3(0.0, 0.0, 0.0);
}

glm::mat4 Camera::SetPerspective(float fov, float aspectRatio, float nearPlane, float farPlane)
{
	ProjectionMatrix = glm::perspective(fov, aspectRatio, nearPlane, farPlane);
	
	return ProjectionMatrix;
}

// This function sets the camera's position and rotation values
void Camera::PositionCamera(float positionX, float positionY, float positionZ, float yaw, float pitch)
{
	// Assign our camera variables
	Position = glm::vec3(positionX, positionY, positionZ);
	Yaw = yaw;
	Pitch = pitch;
}

glm::mat4 Camera::GetViewMatrix(glm::vec3 direction1)
{
	float horizontalAngle = 3.14f;
	// Initial vertical angle : none
	float verticalAngle = 0.0f;

	glm::vec3 direction(
		cos(verticalAngle) * sin(horizontalAngle),
		sin(verticalAngle),
		cos(verticalAngle) * cos(horizontalAngle)
	);

	// Right vector
	glm::vec3 right = glm::vec3(
		sin(horizontalAngle - 3.14f / 2.0f),
		0,
		cos(horizontalAngle - 3.14f / 2.0f)
	);

	// Up vector
	glm::vec3 up = glm::cross(right, direction);

	return glm::lookAt(Position, Position + direction, up);
}

// This builds and returns a rotation matrix from the yaw and pitch rotations
glm::mat4 Camera::GetRotationMatrix()
{
	// Create an identity matrix
	glm::mat4 rotationMatrix(1.0f);

	// Add the Pitch rotation along the x-axis
	rotationMatrix = glm::rotate(rotationMatrix, Pitch, glm::vec3(1, 0, 0));

	// Add the Yaw rotation along the y-axis
	rotationMatrix = glm::rotate(rotationMatrix, Yaw, glm::vec3(0, 1, 0));

	// Return the final rotation matrix that stores our camera rotations
	return rotationMatrix;
}
// This returns the camera's view matrix that is used to pass to our shaders
glm::mat4 Camera::GetViewMatrix()
{
	// Return a view matrix by multiplying our rotation matrix by the inverse of a translation matrix
	return GetRotationMatrix() * glm::inverse(glm::translate(glm::mat4(), Position));
}

// This returns the current view vector (the direction the camera is looking) from the rotation matrix
glm::vec3 Camera::GetView()
{
	// Calculate the view vector by taking the inverse of our rotation matrix multiplied by a vector 
	// looking down the negative z-axis.
	glm::vec4 viewVector = glm::inverse(GetRotationMatrix()) * glm::vec4(0, 0, -1, 1);

	// We are using a 4x4 matrix so our result is a vec4, but just turn it back into a vec3
	return glm::vec3(viewVector);
}

// This will move the camera forward or backward depending on the speed
void Camera::MoveCamera(float speed)
{
	// Get our normalized view vector (The direction we are facing)
	glm::vec3 viewVector = GetView();

	// Move our camera position forward or backward along the view vector
	Position.x += viewVector.x * speed;		// Add our acceleration to our position's X
	Position.z += viewVector.z * speed;		// Add our acceleration to our position's Z
}

void Camera::MoveUp(float speed)
{
	Position.y += speed;
}

// This returns the camera's right vector (the direction pointing perpendicular to the view and up vector)
glm::vec3 Camera::GetRight()
{
	// Calculate the right vector by taking the inverse of the rotation matrix multiplied by a 
	// default right vector.
	glm::vec4 rightVector = glm::inverse(GetRotationMatrix()) * glm::vec4(1, 0, 0, 1);

	// We are using a 4x4 matrix so our result is a vec4, but just cast it back to a vec3
	return glm::vec3(rightVector);
}

// This will strafe the camera left or right depending on the speed
void Camera::Strafe(float speed)
{
	// Get our normalized right vector (The direction perpendicular to our view)
	glm::vec3 rightVector = GetRight();

	// Move our camera position right or left along the right vector
	Position.x += rightVector.x * speed;		// Add our acceleration to our position's X
	Position.z += rightVector.z * speed;		// Add our acceleration to our position's Z
}

//////////// *** NEW *** ////////// *** NEW *** ///////////// *** NEW *** ////////////////////


// This sets the pitch and yaw of our camera from the mouse x and y deltas from the last frame
void Camera::SetViewByMouse(float xOffset, float yOffset)
{
	// Rotate the yaw by the mouse's x offset, multiplied by a sensitivity speed setting
	Yaw += xOffset * MouseSpeed;

	// Rotate the pitch by the mouse's y offset, multiplied by a sensitivity speed setting
	Pitch += yOffset * MouseSpeed;

	// If the yaw goes above 360 degrees (2 PI in radians) then set it back to 0
	if (Yaw > 2 * PI)
		Yaw = 0;

	// Cap the yaw between 0 and 360
	if (Yaw < 0)
		Yaw = 2 * PI;

	// Let's cap the pitch to 75 degrees up and -75 degrees down
	if (Pitch > glm::radians(75.0f))
		Pitch = glm::radians(75.0f);
	if (Pitch < glm::radians(-75.0f))
		Pitch = glm::radians(-75.0f);
}

void Camera::CameraKeyMove(bool keys[], float deltaTime)
{
	if (keys[GLFW_KEY_W])
		MoveCamera(Speed * deltaTime);
	if (keys[GLFW_KEY_S])
		MoveCamera(-Speed * deltaTime);
	if (keys[GLFW_KEY_X])
		MoveUp(Speed * deltaTime);
	if (keys[GLFW_KEY_C])
		MoveUp(-Speed * deltaTime);
	if (keys[GLFW_KEY_A])
		Strafe(-Speed * deltaTime);
	if (keys[GLFW_KEY_D])
		Strafe(Speed * deltaTime);
}
