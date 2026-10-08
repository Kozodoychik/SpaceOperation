#include "logging.hpp"
#include <iostream>
#include <fstream>
#include <chrono>

namespace logging {

	std::ofstream log_file;

	void _WriteToFile(std::string message) {
		if (!log_file.is_open()) return;

		log_file << message;
		log_file.flush();
	}

	void OpenLogFile() {
		log_file.open(LOG_FILE);
	}

	void CloseLogFile() {
		log_file.close();
	}

	void Info(const char* module, const char* message) {
		auto time = std::chrono::system_clock::now();
		std::string msg = std::format("[{}] [{}] [INFO] {}\n", time, module, message);

		std::cout << msg;
		_WriteToFile(msg);
	}

	void InfoColored(const char* module, const char* message) {
		auto time = std::chrono::system_clock::now();
		std::string msg = std::format("[{}] [{}] [INFO] {}\n", time, module, message);
		
		std::cout << "\033[36m" << msg << "\033[0m";
		_WriteToFile(msg);
	}

	void Warn(const char* module, const char* message) {
		auto time = std::chrono::system_clock::now();
		std::string msg = std::format("[{}] [{}] [WARN] {}\n", time, module, message);

		std::cout << "\033[33m" << msg << "\033[0m";
		_WriteToFile(msg);
	}

	void Err(const char* module, const char* message) {
		auto time = std::chrono::system_clock::now();
		std::string msg = std::format("[{}] [{}] [ERR] {}\n", time, module, message);

		std::cerr << "\033[91m" << msg << "\033[0m";
		_WriteToFile(msg);
	}

	void Fatal(const char* module, const char* message) {
		auto time = std::chrono::system_clock::now();
		std::string msg = std::format("[{}] [{}] [FATAL] {}\n", time, module, message);

		std::cerr << "\033[31m" << msg << "\033[0m";
		_WriteToFile(msg);
	}

}
