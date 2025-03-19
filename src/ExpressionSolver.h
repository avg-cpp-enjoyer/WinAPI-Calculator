#pragma once

#include <string>
#include <cmath>
#include <format>
#include <unordered_map>
#include <functional>

#define PI 3.141592653589793

class CalcException : public std::exception {
public:
	explicit CalcException(const std::wstring& msg) : msg(msg) {}
	const wchar_t* wwhat() const noexcept;
private:
	std::wstring msg;
};

class ExpressionSolver {
public:
	ExpressionSolver() = delete;
	static std::wstring EvaluateExpression(std::wstring_view expr);
	static void UseDegrees(bool state);
private:
	static double ParseNumber(std::wstring_view expr, size_t& pos);
	static double ParsePriorityFirst(std::wstring_view expr, size_t& pos);
	static double ParsePrioritySecond(std::wstring_view expr, size_t& pos);
	static double ParsePriorityThird(std::wstring_view expr, size_t& pos);
	static double ParsePriorityFourth(std::wstring_view expr, size_t& pos);

	static const std::unordered_map<std::wstring_view, std::function<double(double)>> s_functions;
	static bool s_useDegrees;
};
