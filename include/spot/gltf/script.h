#ifndef GST_SCRIPT_H_
#define GST_SCRIPT_H_

#include <string>

namespace spot::gltf
{
/// Sunspot script
struct Script
{
	/// Uri of the script
	std::string uri = {};
	/// Name of the script
	std::string name = "Unknown";
};


}  // namespace spot::gltf

#endif  // GST_SCRIPT_H_
