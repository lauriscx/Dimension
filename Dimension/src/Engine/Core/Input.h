#pragma once

#include <iostream>

namespace Dimension {
	class Input {
	public:
		static bool IsKeyPressed(int Key);
		static bool	IsMousePressed(int Key);
		static std::pair<double, double> GetMousePosition();
		static double GetMouseX();
		static double GetMouseY();

	private:

	};
}