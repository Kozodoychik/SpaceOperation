#define LOG_FILE "game.log"

constexpr const char* __filename(const char* full_path) {

	const char* name = full_path;
	for (const char* p = full_path; *p; p++) {
		if (*p == '\\' || *p == '/')
			name = p + 1;
	}
	return name;

}

#define MODULE_NAME __filename(__FILE__)

namespace logging {

	void OpenLogFile();
	void CloseLogFile();

	void Info(const char* module, const char* message);
	void InfoColored(const char* module, const char* message);

	void Warn(const char* module, const char* message);
	void Err(const char* module, const char* message);
	void Fatal(const char* module, const char* message);

};
