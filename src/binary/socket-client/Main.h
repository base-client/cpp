#pragma once

#include "EnvironmentVariable.h"

class Main {
	private:
		EnvironmentVariable environmentVariable;

		bool Initialize(int argc, char *argv[]);
		bool Job();

	public:
		Main() = default;
		virtual ~Main() = default;

		bool Run(int argc, char *argv[]);
};
