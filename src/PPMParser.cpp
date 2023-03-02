#include "PPMParser.hpp"

#include <numeric>
#include <ranges>
#include <utility>
#include <array>
#include <algorithm>

ParsedPPM::ParsedPPM(
  const std::size_t  width,
  const std::size_t  height,
  std::vector<Pixel> pixels
)
  : width{ width },
    height{ height },
    pixels{ std::move(pixels) } {}

PPMParser::PPMParser(const std::string_view& ppm_file_content)
  : ppm_file_content{ ppm_file_content } {}

auto PPMParser::parse_image_format() -> void {
    const auto image_format = this->read_line();
    assert(
      image_format == "P3"
      && "error: currently supporting only 'P3' ppm file formats\n"
    );
}

auto PPMParser::parse_image_dimensions()
  -> std::pair<std::size_t, std::size_t> {
    const auto                 dimensions = this->read_line();
    std::array<std::size_t, 2> temp       = {};

    // clang-format off
    std::ranges::copy(dimensions
        | std::views::split(' ')
        | std::views::transform([](const auto view) {
            auto it = view | std::views::common;
            return std::stoi(std::string{ it.begin(), it.end() });
        }), std::begin(temp));
    // clang-format on

    try {
        return { temp.at(0), temp.at(1) };
    } catch (const std::exception& exception) {
        assert(false && "error: width and height not found\n");
    }
}

auto PPMParser::parse_color_max_value() -> std::uint32_t {
    return static_cast<std::uint32_t>(std::stoi(this->read_line()));
}

auto PPMParser::parse_pixels(const std::uint32_t color_max_value)
-> std::vector<Pixel> {
    return {};
}

auto PPMParser::is_eof() const noexcept -> bool {
    return this->cursor >= this->ppm_file_content.size();
}

auto PPMParser::read_line() -> std::string {
    std::string line;

    char current_ch = this->ppm_file_content[this->cursor++];
    while (!this->is_eof() && current_ch != '\n') {
        line += current_ch;
        current_ch = this->ppm_file_content[this->cursor++];
    }

    return line;
}
