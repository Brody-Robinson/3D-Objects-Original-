#ifndef LIGHT_H
#define LIGHT_H

#include "glm/glm.hpp"
#include <string>

/*

      Light container object for any 3D point light source. Point lights range are solely 
      determined by a radius value which is used for their frustum culling and attenuation
      properties. 

    */

class Light
{
	public:
		Light() {}
		~Light() {}

	/**
	@brief Creates an instance
	@param color The color of the light
	@param intensity The intensity of the light
	@param ambiental The ambient intensity
	*/
	Light(glm::vec3 color, std::string name = "light",  float intensity = 1.f, float ambiental = 0.25f, float specular = 0.5f);
	
	/**
	@brief Set the position of the light
	@param value The position to set
	*/
	void set_position(glm::vec3 value);
	
	/**
	@brief Set the direction of the light
	@param value The direction to set
	*/
	void set_direction(glm::vec3 value);

	/**
	@brief Set the spot Direction of the spot light
	@param value The direction to set
*/
	void set_Spotdirection(glm::vec3 value);


	/**
	@brief Set the color of the light
	@param color The color to set
	*/
	void set_color(glm::vec3& color);

	/**
	@brief Set the ambient intensity
	@param value The ambient intensity to set
	*/
	void set_ambient_intensity(float value);

	/**
	@brief Set the intensity
	@param The intensity to set
	*/
	void set_intensity(float value);
	
	/**
	@brief Set the specular intensity
	@param The specular intensity to set
	*/
	void set_specular_intensity(float value);

	/**
	@brief Set the cutoff of light
	@param The cutof  to set
	*/
	void set_spotlight_cutoff(float value);


		/**
	@brief Set the inner cutoff
	@param The inner cutoffto set
	*/
	void set_spotlight_innercutoff(float value);


	void set_attenuation_constants(float exponent, float kc, float kl, float kq);



	/**
	@brief Gets the color of the light
	@retun The light color
	*/
	glm::vec3& get_color();

	/**
	@brief Gets the light intensity
	@return The light intensity
	*/
	float get_intensity();

	/**
	@brief Gets the ambient intensity
	@return The ambient intensity
	*/
	float get_ambient_intensity();
	/**
	@brief Gets the specular intensity
	@return The specular intensity
	*/
	float get_specular_intensity();
	/**
	@brief Render. Update the lights values
	@param camera The reference of the camera
	*/
	void render(glm::vec3 CameraPosition, unsigned int shaderID, bool Directional, bool isSpot);

	private:
		float intensity;

		float ambient_intensity;

		float specular_intensity;

		glm::vec3 light_color = glm::vec3(1.0f, 1.0f, 1.0f);
		glm::vec3 attenuation = glm::vec3(0.0f);

		glm::mat4 transform;

		std::string name;

		glm::vec3 spot = glm::vec3(0.0f, 0.0f, 1.0f);
		float cutoff = 1.0f;
		float innercutoff = 1.0f;
		float spotExp = 0.0f;

		union 
		{
			glm::vec3 direction;
			glm::vec3 Position = glm::vec3(0.0f); 
		};

		//float      Radius = 1.0f;
		//bool       Visible = true;

		static std::string Ldirection; 
		static std::string LSpotdirection; 
		static std::string Lposition; 
		static std::string Lambient; 
		static std::string Lspecular; 
		static std::string Ldiffuse; 
		static std::string LCutoff; 
		static std::string LInnerCutoff;
		static std::string LisDirection; 
		static std::string LisPoint; 
		static std::string LisSpot; 
		static std::string LColor;
		static std::string Lkc;
		static std::string Lkl;
		static std::string Lkq;
		static std::string Lexponent;
};


#endif