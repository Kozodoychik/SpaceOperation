#include "logging.hpp"
#include <iostream>
#include <chrono>

namespace logging {

	void Info(const char* module, const char* message) {
		auto time = std::chrono::system_clock::now();
		std::cout << std::format("[{}] [{}] [INFO] {}\n", time, module, message);
	}

	void Warn(const char* module, const char* message) {
		auto time = std::chrono::system_clock::now();
		std::cout << std::format("\033[33m[{}] [{}] [WARN] {}\033[0m\n", time, module, message);
	}

	void Err(const char* module, const char* message) {
		auto time = std::chrono::system_clock::now();
		std::cerr << std::format("\033[91m[{}] [{}] [ERR] {}\033[0m\n", time, module, message);
	}

	void Fatal(const char* module, const char* message) {
		auto time = std::chrono::system_clock::now();
		std::cerr << std::format("\033[31m[{}] [{}] [FATAL] {}\033[0m\n", time, module, message);
	}

}
