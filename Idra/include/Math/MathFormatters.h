#pragma once

#include <spdlog/fmt/bundled/format.h>
#include <glm/glm.hpp>

// Custom formatters for glm types
// Teach fmt (and therefore spdlog) how to format glm::vec3
template<>
struct fmt::formatter<glm::vec3>
{
	// Parses the format string (we just ignore it here)
	constexpr auto parse(format_parse_context& ctx) const { return ctx.begin(); }

	// Formats the glm::vec2 into the output
	template<typename FormatContext>
	auto format(const glm::vec3& v, FormatContext& ctx) const { return fmt::format_to(ctx.out(), "({}, {}, {})", v.x, v.y, v.z); }
};

template<>
struct fmt::formatter<glm::vec2>
{
	constexpr auto parse(format_parse_context& ctx) const { return ctx.begin(); }

	template<typename FormatContext>
	auto format(const glm::vec2& v, FormatContext& ctx) const { return fmt::format_to(ctx.out(), "({}, {})", v.x, v.y); }
};

template<>
struct fmt::formatter<glm::vec4>
{
	constexpr auto parse(format_parse_context& ctx) const { return ctx.begin(); }

	template<typename FormatContext>
	auto format(const glm::vec4& v, FormatContext& ctx) const { return fmt::format_to(ctx.out(), "({}, {}, {}, {})", v.x, v.y, v.z, v.w); }
};

namespace Idra::Math {
	template<typename MatType, int Rows, int Cols>
	std::string MatrixToString(const MatType& m)
	{
		std::stringstream ss;
		ss << "[\n ";
		for (int col = 0; col < Cols; ++col) {
			ss << "[";
			for (int row = 0; row < Rows; ++row) {
				ss << m[col][row];
				if (row < Rows - 1)
					ss << ", ";
			}
			ss << "]";
			if (col < Cols - 1)
				ss << ",\n ";
		}
		ss << "]";
		return ss.str();
	}
}

template<>
struct fmt::formatter<glm::mat4>
{
	constexpr auto parse(format_parse_context& ctx) const { return ctx.begin(); }

	template<typename FormatContext>
	auto format(const glm::mat4& m, FormatContext& ctx) const
	{
		return fmt::format_to(ctx.out(), "{}", Idra::Math::MatrixToString<glm::mat4, 4, 4>(m));
	}
};

template<>
struct fmt::formatter<glm::mat3>
{
	constexpr auto parse(format_parse_context& ctx) const { return ctx.begin(); }
	template<typename FormatContext>
	auto format(const glm::mat3& m, FormatContext& ctx) const
	{
		return fmt::format_to(ctx.out(), "{}", Idra::Math::MatrixToString<glm::mat3, 3, 3>(m));
	}
};
