#include "PPMParser.hpp"

#include <algorithm>
#include <array>
#include <numeric>
#include <ranges>
#include <utility>

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

auto PPMParser::parse_pixels(
  [[maybe_unused]] const std::uint32_t color_max_value
) -> std::vector<Pixel> {
    std::vector<std::uint32_t> raw_pixels = {};

    std::string pixels_string{ this->ppm_file_content.begin() + this->cursor, this->ppm_file_content.end() };
    std::replace(pixels_string.begin(), pixels_string.end(), '\n', ' ');

    for (const auto elem : pixels_string | std::views::split(' ')) {
        const auto view      = elem | std::views::common;
        const auto split_str = std::string{ view.begin(), view.end() };

        const auto is_pixel_value = [&split_str]() {
            if (split_str.size() < 1) { return false; }

            for (const auto& ch : split_str) {
                if (std::isspace(ch)) { return false; }
            }

            return true;
        }();

        if (is_pixel_value) {
            const auto pixel_value =
              static_cast<std::uint32_t>(std::stoi(split_str));
            assert(
              pixel_value <= color_max_value
              && "error: one pixel component exceeds max color depth"
            );
            raw_pixels.push_back(pixel_value);
        } else {
            continue;
        }
    }

    std::vector<Pixel> pixels = {};

    for (std::size_t i = 0; i <= raw_pixels.size() - 3; i += 3) {
        pixels.push_back(Pixel{ .r = raw_pixels.at(i),
                                .g = raw_pixels.at(i + 1),
                                .b = raw_pixels.at(i + 2) });
    }

    return pixels;
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
