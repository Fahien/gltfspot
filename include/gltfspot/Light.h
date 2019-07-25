#ifndef GST_LIGHT_H_
#define GST_LIGHT_H_

#include <string>

#include <mathspot/Math.h>

namespace gltfspot
{
/// Punctual light that emit light in well-defined directions and intensities
struct PYSPOT_EXPORT Light
{
	/// Type of light
	enum class Type
	{
		Point,
		Directional,
		Spot
	};

	/// Spot light which emits light in a cone in the direction of the local -z axis
	struct Spot
	{
		/// Angle in radians, from center where falloff begins
		float inner_cone_angle = 0.0f;

		/// Angle in radians, from center where falloff ends
		float outer_cone_angle = mathspot::kPi / 4.0f;
	};

	/// Name of the light
	std::string name = "Unknown";

	/// Color of the light
	mathspot::Vec3 color = mathspot::Vec3{ 1.0f, 1.0f, 1.0f };

	/// Unit depending on the type of light
	/// Point and spot lights use luminous intensity in candela (lm/sr)
	/// Directional lights use illuminance in lux (lm/m^2)
	float intensity = 1.0f;

	/// Type of light
	Type type = Type::Point;

	/// Distance cutoff where intensity reaches zero (point and spot)
	/// If <= 0 it is assumed to be infinite
	float range = 0.0f;

	/// Spot light values
	Spot spot = {};
};

}  // namespace gltfspot


#endif  // GST_LIGHT_H_