#ifndef GST_SCRIPT_H_
#define GST_SCRIPT_H_

#include <string>


namespace gltfspot
{
/// Sunspot script
struct Script
{
	/// Uri of the script
	std::string uri = {};
	/// Name of the script
	std::string name = "Unknown";
};


}  // namespace gltfspot

#endif  // GST_SCRIPT_H_
