#ifndef PPM_PARSER
#define PPM_PARSER

#include <cassert>
#include <string>
#include <string_view>
#include <vector>

struct Pixel {
    std::uint32_t r = 0;
    std::uint32_t g = 0;
    std::uint32_t b = 0;
};

struct ParsedPPM {
    std::size_t        width;
    std::size_t        height;
    std::vector<Pixel> pixels;

    ParsedPPM(
      const std::size_t  width,
      const std::size_t  height,
      std::vector<Pixel> pixels
    );
};

class PPMParser {
  public:
    static auto parse(const std::string& ppm_file_content) {
        PPMParser parser(std::string_view{ ppm_file_content });

        parser.parse_image_format();

        const auto [width, height] = parser.parse_image_dimensions();
        [[maybe_unused]] const auto color_max_value =
          parser.parse_color_max_value();
        const auto pixels = parser.parse_pixels(color_max_value);

        return ParsedPPM(width, height, pixels);
    }

  private:
    explicit PPMParser(const std::string_view& ppm_file_content);

    // clang-format off
    auto parse_image_format() -> void;
    [[nodiscard]] auto parse_image_dimensions() -> std::pair<std::size_t, std::size_t>;
    [[nodiscard]] auto parse_color_max_value() -> std::uint32_t;
    [[nodiscard]] auto parse_pixels(const std::uint32_t color_max_value) -> std::vector<Pixel>;
    // clang-format on

    [[nodiscard]] auto is_eof() const noexcept -> bool;
    [[nodiscard]] auto read_line() -> std::string;

    std::string_view ppm_file_content;
    std::size_t      cursor = 0;
};

#endif // PPM_PARSER
