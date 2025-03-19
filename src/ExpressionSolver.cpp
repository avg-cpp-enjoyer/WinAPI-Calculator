#include "ExpressionSolver.h"

const wchar_t* CalcException::wwhat() const noexcept {
	return msg.c_str();
}

const std::unordered_map<std::wstring_view, std::function<double(double)>> ExpressionSolver::s_functions = {
	{L"sin",  [](double x) -> double { return s_useDegrees ? std::sin (x * PI / 180.0) : std::sin(x);  }},
	{L"cos",  [](double x) -> double { return s_useDegrees ? std::cos (x * PI / 180.0) : std::cos(x);  }},
	{L"tan",  [](double x) -> double { return s_useDegrees ? std::tan (x * PI / 180.0) : std::tan(x);  }},
	{L"atan", [](double x) -> double { return s_useDegrees ? std::atan(x * PI / 180.0) : std::atan(x); }},
	{L"sqrt", [](double x) -> double { return std::sqrt  (x); }},
	{L"log",  [](double x) -> double { return std::log10 (x); }},
	{L"ln",   [](double x) -> double { return std::log   (x); }}
};

bool ExpressionSolver::s_useDegrees = true;

double ExpressionSolver::ParseNumber(std::wstring_view expr, size_t& pos) {
	bool isNegative = false;
	bool isDecimal = false;
	double number = 0.0;
	double factor = 0.1;
	wchar_t currentChar = expr[pos];

	if (currentChar == '-') {
		isNegative = true;
		pos++;
	}

	while (pos < expr.size()) {
		currentChar = expr[pos];
		if (currentChar == '.') {
			if (isDecimal) {
				throw CalcException(L"ERROR: Multiple decimal points in number");
			}
			isDecimal = true;
			pos++;
			continue;
		}

		if (!std::isdigit(currentChar)) {
			break;
		}

		if (isDecimal) {
			number += (currentChar - '0') * factor;
			factor *= 0.1;
		} else {
			number = number * 10 + (currentChar - '0');
		}

		pos++;
	}

	return isNegative ? -number : number;
}

double ExpressionSolver::ParsePriorityFirst(std::wstring_view expr, size_t& pos) {
	wchar_t currentChar = expr[pos];

	if (currentChar == '(') {
		pos++;
		double result = ParsePriorityFourth(expr, pos);

		if (pos >= expr.size() || expr[pos] != ')') {
			throw CalcException(L"ERROR: Missing closing parenthesis");
		}

		pos++;
		return result;
	}

	if (std::isalpha(currentChar)) {
		size_t start = pos;
		while (pos < expr.size() && std::isalpha(expr[pos])) {
			pos++;
		}

		std::wstring_view func = expr.substr(start, pos - start);

		if (pos >= expr.size() || expr[pos] != '(') {
			throw CalcException(L"ERROR: Expected '(' after function name");
		}

		pos++;
		double arg = ParsePriorityFourth(expr, pos);

		if (pos >= expr.size() || expr[pos] != ')') {
			throw CalcException(L"ERROR: Missing closing parenthesis after function argument");
		}

		pos++;

		auto it = s_functions.find(func);
		if (it != s_functions.end()) {
			return it->second(arg);
		}

		throw CalcException(L"ERROR: Unknown function: " + std::wstring(func));
	}

	return ParseNumber(expr, pos);
}

double ExpressionSolver::ParsePrioritySecond(std::wstring_view expr, size_t& pos) {
	double result = ParsePriorityFirst(expr, pos);

	if (pos < expr.size() && expr[pos] == '^') {
		pos++;
		double right = ParsePrioritySecond(expr, pos);
		result = std::pow(result, right);
	}

	return result;
}

double ExpressionSolver::ParsePriorityThird(std::wstring_view expr, size_t& pos) {
	double result = ParsePrioritySecond(expr, pos);

	while (pos < expr.size()) {
		wchar_t currentChar = expr[pos];

		if (currentChar == L'÷') {
			pos++;
			double divisor = ParsePrioritySecond(expr, pos);

			if (divisor == 0) {
				throw CalcException(L"Division by zero");
			}

			result /= divisor;

		} else if (currentChar == L'×') {
			pos++;
			result *= ParsePrioritySecond(expr, pos);
		} else {
			break;
		}
	}

	return result;
}

double ExpressionSolver::ParsePriorityFourth(std::wstring_view expr, size_t& pos) {
	double result = ParsePriorityThird(expr, pos);

	while (pos < expr.size()) {
		wchar_t currentChar = expr[pos];

		if (currentChar == '+') {
			pos++;
			result += ParsePriorityThird(expr, pos);
		} else if (currentChar == '-') {
			pos++;
			result -= ParsePriorityThird(expr, pos);
		} else {
			break;
		}
	}

	return result;
}

std::wstring ExpressionSolver::EvaluateExpression(std::wstring_view expr) {
	size_t pos = 0;
	double result = ParsePriorityFourth(expr, pos);

	if (pos != expr.size()) {
		throw CalcException(L"ERROR: Unexpected characters at the end of expression");
	}

	return std::format(L"{:.9g}", result);
}

void ExpressionSolver::UseDegrees(bool state) {
	s_useDegrees = state;
}
