#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#include  <Universal/3rdparty/include/glm/gtx/quaternion.hpp>

#include <Universal/3rdparty/include/glm/gtx/rotate_vector.hpp>

#include "../../Common/user_interaction.h"
//#include "object_basis.h"
//#include "../Shader/shader_util.h"

#include "../../Shader/shader.h"

enum class camera_ailgnment_type_enum {none, xy_plane, xz_plane, yz_plane };
enum class camera_movement_mode_enum { free, xy_plane, xz_plane, yz_plane, orbital, none };

class camera_object_class //: public object_basis_class
{
public:
	glm::vec3 mPosition = { 0.0f, 0.0f, 8.0f };
	float mPitch = 0.0f;
	float mYaw   = 0.0f;
	float mRoll  = 0.0f;

	float cRotationSpeed = 0.25f;
	glm::mat4 mvpMatrix;


	camera_object_class()//(const glm::vec3& position, float fov, float aspect, float near, float far)
	{
		//(glm::vec3(0, 0, 3), 45.0f, 1.3f, 0.1f, 100.0f)

		mPosition = glm::vec3(0, 0, 8.0);
		mAspect   = 1.3f;
		mNear     = 0.1f;
		mFar      = 10000.0f;
		mFOV      = 45.0f;

		set_aspect(mAspect);

		update_view_matrix();
	}
/*
	void update(shader_class* shader) override
	{
		glm::mat4 model{ 1.0f };
		shader->set_mat4(model, "model");
		shader->set_mat4(mViewMatrix, "view");
		shader->set_mat4(get_projection(), "projection");
		shader->set_vec3(mPosition, "camPos");
	}
*/
	void update(unsigned int shader_program_id)
	{
		shader_class shader;

		glm::mat4 model{ 1.0f };

		//shader.set_mat4(shader_program_id, model,            "model");
		shader.set_mat4(shader_program_id, get_view_matrix(),"view");
		shader.set_mat4(shader_program_id, get_projection(), "projection");
		
		//glm::mat4 mvpMatrix = get_projection() * mViewMatrix * model;
		mvpMatrix = get_projection() * mViewMatrix * model;
		shader.set_mat4(shader_program_id, mvpMatrix,        "mvpMatrix");

		// Following for my shaders
//		glm::mat4 mvpMatrix = get_projection() * mViewMatrix * model; // It seems that using this does not work and gives the incorrect result
//printf("update %f")
		shader.set_vec3(shader_program_id, mPosition    , "camera_loc");
		shader.set_vec3(shader_program_id, get_forward(), "camera_front_vector");
		shader.set_vec3(shader_program_id, get_up()     , "camera_up_vector");
		shader.set_vec3(shader_program_id, get_right()  , "camera_right_vector");

//printf("camera update : % f : %f :%f \n", mPosition.x, mPosition.y, mPosition.z);
	}

	// **** ADDED ADDITIONS ********************
	camera_movement_mode_enum movement = camera_movement_mode_enum::free;

	bool  pitch_rotation   = true;
	bool  yaw_rotation     = true;
	bool  roll_rotation    = true;
	bool  orbital_rotation = true;

	float movement_factor = 1.0f;

	glm::vec3 look_at_location = { 0.0,0.0,0.0 };

	// ###################### Camera translation and rotation ###########################

	void move_in_direction(glm::vec3 direction) { // not tested
		mPosition += cForward * direction.z * movement_multiplier;// move_forward(direction.z);
		mPosition += cUp * direction.y * movement_multiplier;     //move_up(direction.y);
		mPosition += cRight * direction.x * movement_multiplier;  // move_right(direction.x);

		update_view_matrix();
	}
	
	void move_forward() {
		mPosition += cForward * movement_factor * movement_multiplier;
		update_view_matrix();
	}

	void move_backward() {
		mPosition += cForward * (-movement_factor) * movement_multiplier;
		update_view_matrix();
	}

	void move_up() {
		mPosition += cUp * movement_factor * movement_multiplier;
		update_view_matrix();
	}

	void move_down() {
		mPosition += cUp * (-movement_factor) * movement_multiplier;
		update_view_matrix();
	}

	void move_left() {
		mPosition += cRight * (-movement_factor) * movement_multiplier;
		update_view_matrix();
	}

	void move_right() {
		mPosition += cRight * movement_factor * movement_multiplier;
		update_view_matrix();
	}

	// **** ADDED ADDITIONS **********************

	void pitch_rotate(float delta) {
		mPitch += delta ;
		cUp      = glm::rotate(cUp, ( -delta), cRight);
		cForward = glm::rotate(cForward, (-delta), cRight);
		update_view_matrix();
	}

	void yaw_rotate(float delta) {
		mYaw += delta;
		cRight   = glm::rotate(cRight, (-delta), cUp);
		cForward = glm::rotate(cForward, (-delta), cUp);
		update_view_matrix();
	}

	void roll_rotate(float delta) {
		mRoll += delta;
		cRight = glm::rotate(cRight, delta, cForward);
		cUp    = glm::rotate(cUp, delta, cForward);
		update_view_matrix();
	}

	void roll_camera(float delta) {
		if(roll_rotation) roll_rotate(delta * cRotationSpeed);
	}

	// ###################### Camera alignment ###########################
	void align_to_plane(camera_ailgnment_type_enum camera_ailgnment) { // not tested
		glm::vec3 alignment_vector = {0.0,0.0,0.0};

		switch (camera_ailgnment) {
			case camera_ailgnment_type_enum::yz_plane: alignment_vector = { 0.0,mPosition.y,mPosition.z }; break;
			case camera_ailgnment_type_enum::xz_plane: alignment_vector = { mPosition.x,0.0,mPosition.z }; break;
			case camera_ailgnment_type_enum::xy_plane: alignment_vector = { mPosition.x,mPosition.y,0.0 }; break;
		}

		look_at(alignment_vector);
	}

	// ##################################################################
	void set_location(glm::vec3 location) {
printf("Camera location 0000 %f:%f:%f\n", mPosition.x, mPosition.y, mPosition.z);
		mPosition = location;
printf("Camera location 1111 %f:%f:%f\n", mPosition.x, mPosition.y, mPosition.z);
		update_view_matrix();
	}

	void set_orientation(glm::quat orientation) {
		float pitch = glm::eulerAngles(orientation).x;
		float yaw   = glm::eulerAngles(orientation).y;
		float roll  = glm::eulerAngles(orientation).z;

		set_orientation(pitch,yaw, roll);
	}

	void set_orientation(float pitch, float yaw, float roll) { // Needs testing
		float pitch_rotate_angle = pitch - mPitch;
		float yaw_rotate_angle   = yaw - mYaw;
		float roll_rotate_angle  = roll - mRoll;

		// Pitch rotate direction vectors
		cUp      = glm::rotate(cUp, ( -pitch_rotate_angle), cRight);
		cForward = glm::rotate(cForward, (-pitch_rotate_angle), cRight);
		// Yaw rotate direction vectors
		cRight   = glm::rotate(cRight, (-yaw_rotate_angle), cUp);
		cForward = glm::rotate(cForward, (-yaw_rotate_angle), cUp);
		// Roll rotate direction vectors
		cRight = glm::rotate(cRight, roll_rotate_angle, cForward);
		cUp    = glm::rotate(cUp, roll_rotate_angle, cForward);

		mPitch = pitch; mYaw = yaw; mRoll = roll;

		update_view_matrix();
	}

	void look_at(glm::vec3 look_at_location) {
		mViewMatrix = glm::lookAt(mPosition, look_at_location, glm::normalize(cUp));
		glm::quat orientation = glm::conjugate(glm::toQuat(mViewMatrix));

		// Get direction vectors and axis directly from the view matrix
		// Some thing that glm does not provide or is poorly ar not documented elsewhere
		cRight   = glm::vec3(mViewMatrix[0][0], mViewMatrix[1][0], mViewMatrix[2][0]);
		cUp      = glm::vec3(mViewMatrix[0][1], mViewMatrix[1][1], mViewMatrix[2][1]);
		cForward = glm::vec3(-mViewMatrix[0][2], -mViewMatrix[1][2], -mViewMatrix[2][2]);
		//cForward = glm::normalize(look_at_location-mPosition); // can use this But probably less efficient

		mPitch = glm::eulerAngles(orientation).x;
		mYaw =   glm::eulerAngles(orientation).y;
		mRoll =  glm::eulerAngles(orientation).z;
	}

	void set_pivot_location(glm::vec3 pivot_location) {
		mFocus = pivot_location;
		look_at_location = pivot_location;
	}

	float get_focus_distance() {
		return glm::distance(mPosition , mFocus);
	}

	float get_FOV() {
		return mFOV;
	}
	// ##################################################################
	// **** end ADDED ADDITIONS **********************

    void set_aspect(float aspect) {
      mProjection = glm::perspective(mFOV, aspect, mNear, mFar);
    }

	void set_distance(float offset)	{
		mDistance += offset;
		update_view_matrix();
	}

	const glm::mat4& get_projection() const{
		return mProjection;
	}

    glm::mat4 get_view_projection() const {
      return mProjection * get_view_matrix();
    }

	// Alternative methods of getting Up, Right and Forward direction vectors given
	glm::vec3 get_up() const{
		//return glm::rotate(get_orientation(), cUp);
		//return glm::vec3(mViewMatrix[0][1], mViewMatrix[1][1], mViewMatrix[2][1]);
		return cUp;
	}

	glm::vec3 get_right() const{
		//return glm::rotate(get_orientation(), cRight);
		//return glm::vec3(mViewMatrix[0][0], mViewMatrix[1][0], mViewMatrix[2][0]);
		return cRight;
	}

	glm::vec3 get_forward() const{
		//return glm::rotate(get_orientation(), cForward);
		//return glm::vec3(-mViewMatrix[0][2], -mViewMatrix[1][2], -mViewMatrix[2][2]);
		return cForward;
	}

	glm::quat get_orientation() const	{
		//return glm::quat(glm::vec3(-mPitch, -mYaw, 0.0f));
		return glm::quat(glm::vec3(mPitch, mYaw, mRoll));
	}

	glm::mat4 get_view_matrix() const{
		return mViewMatrix;
	}

	void on_mouse_wheel(float delta)	{
//printf("on_mouse_wheel\n");
		
		//zoom_camera(delta * 0.5f);
		// Disabled

		//set_distance(delta * 0.5f);
		//update_view_matrix();
	}

	void reset()	{
		mFocus = { 0.0f, 0.0f, 0.0f };
		//reset_zoom();
		//mDistance = 5.0f;
		update_view_matrix();
	}



	void on_mouse_move(float x, float y, mouse_button_enum button){
//printf("on_mouse_move\n");
		glm::vec2 pos2d{ x, y };

		if (button == mouse_button_enum::Right ){
//printf("on_mouse_move button right\n");
			glm::vec2 delta = (pos2d - mCurrentPos2d) * movement_multiplier;

			float sign = get_up().y < 0 ? -1.0f : 1.0f;
			//mYaw   += sign * delta.x * cRotationSpeed;
			//mPitch += delta.y * cRotationSpeed;

			if(pitch_rotation) pitch_rotate(delta.y* cRotationSpeed);
			if(yaw_rotation)   yaw_rotate(sign * delta.x* cRotationSpeed);

			//update_view_matrix();
			
		} /*else if (button == mouse_button_enum::Middle) {
//printf("on_mouse_move button middle\n");
			glm::vec2 delta = (pos2d - mCurrentPos2d) * movement_multiplier;

			mFocus += -get_right() * delta.x * mDistance;
			mFocus +=  get_up() * delta.y * mDistance;

			update_view_matrix();
		}*/

		mCurrentPos2d = pos2d;
	}

	void update_view_matrix(){
		if (orbital_rotation) {
			mDistance = glm::distance(mPosition ,look_at_location);
			mFocus    = look_at_location;
			//mPosition = mFocus - get_forward() * mDistance;
			mPosition = mFocus - cForward * mDistance;// ****
		}

		mViewMatrix = glm::lookAt(mPosition, mPosition + cForward, cUp); // ****

		//glm::quat orientation = get_orientation();
		//mViewMatrix = glm::translate(glm::mat4(1.0f), mPosition) * glm::toMat4(orientation);
		//mViewMatrix = glm::inverse(mViewMatrix);
	}

private:
	glm::mat4 mViewMatrix;
	glm::mat4 mProjection = glm::mat4{ 1.0f };

	glm::vec3 mFocus = { 0.0f, 0.0f, 0.0f };

	float mDistance = 5.0f;
	float mAspect=1.3f;
	float mFOV = 45.0f;
	float mNear = 0.1f;
	float mFar = 100.0f;

	glm::vec2 mCurrentPos2d = { 0.0f, 0.0f };

	glm::vec3 cRight = { 1.0f, 0.0f, 0.0f };
	glm::vec3 cUp = { 0.0f, 1.0f, 0.0f };
	glm::vec3 cForward = { 0.0f, 0.0f, -1.0f };

	const float movement_multiplier = 0.05f;

};


