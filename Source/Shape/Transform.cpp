#include "Transform.h"

#include <ostream>

Transform::Transform() {
	updateTransformMatrix();
}

void Transform::updateTransformMatrix() {
	transform = glm::translate(position) * glm::mat4_cast(glm::quat(rotation)) * glm::scale(scale);
	transformIsInvalid = false;
}

glm::mat4 & Transform::getTransformMatrix() {
	if (transformIsInvalid) { updateTransformMatrix(); }
	return transform;
}

glm::vec3 Transform::forward() { return glm::quat(rotation) * glm::vec3(0, 0, 1); }
glm::vec3 Transform::up() { return glm::quat(rotation) * glm::vec3(0, 1, 0); }
glm::vec3 Transform::right() { return glm::quat(rotation) * glm::vec3(-1, 0, 0); }

std::ostream & operator<<(std::ostream & os, const Transform & t) {
	os << "- - - transform - - -" << std::endl;
	os << "position: " << t.position.x << ", " << t.position.y << ", " << t.position.z << std::endl;
	os << "rotation: " << t.rotation.x << ", " << t.rotation.y << ", " << t.rotation.z << std::endl;
	os << "scale: " << t.scale.x << ", " << t.scale.y << ", " << t.scale.z << std::endl;
	os << "matrix: " << std::endl;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			os << t.transform[i][j] << " ";
		}
		os << std::endl;
	}
	os << "- - -         - - -" << std::endl;
	return os;
}