#ifndef GST_SCRIPT_H_
#define GST_SCRIPT_H_

#include <string>

#define PYSPOT_EXPORT __attribute__( ( annotate( "pyspot" ) ) )

namespace gltfspot
{
/// Sunspot script
struct PYSPOT_EXPORT Script
{
	/// Uri of the script
	std::string uri = {};
	/// Name of the script
	std::string name = "Unknown";
};


}  // namespace gltfspot

#endif  // GST_SCRIPT_H_
