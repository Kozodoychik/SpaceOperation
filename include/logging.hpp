#include <chrono>

constexpr const char* filename(const char* full_path) {

	const char* name = full_path;
	for (const char* p = full_path; *p; p++) {
		if (*p == '\\' || *p == '/')
			name = p + 1;
	}
	return name;

}

#define MODULE_NAME filename(__FILE__)

namespace logging {

	void Info(const char* module, const char* message);
	void Warn(const char* module, const char* message);
	void Err(const char* module, const char* message);
	void Fatal(const char* module, const char* message);

};
