#define FMT_HEADER_ONLY
#include "PPMParser.hpp"
#include <fmt/format.h>
#include <fstream>

auto print_usage() -> void { fmt::print("usage: ppm-viewer <filename.ppm>"); }

[[nodiscard]] auto read_entire_file(const char* ppm_file_path) -> std::string {
    std::ifstream stream(ppm_file_path);
    return { std::istreambuf_iterator<char>(stream),
             std::istreambuf_iterator<char>() };
};

auto main(const std::int32_t argc, const char** argv) -> int {
    if (argc != 2) {
        print_usage();
        return -1;
    }

    const char* ppm_file_path    = argv[1];
    const auto  ppm_file_content = read_entire_file(ppm_file_path);

    const ParsedPPM parsed_ppm = PPMParser::parse(ppm_file_content);

    for (std::size_t y = 0; y < parsed_ppm.height; ++y) {
        for (std::size_t x = 0; x < parsed_ppm.width; ++x) {
            const auto pixel = parsed_ppm.pixels[y * parsed_ppm.width + x];
            fmt::print("\x1b[48;2;{};{};{}m   \x1b[0m", pixel.r, pixel.g, pixel.b);
        }
        fmt::print("\n");
    }

    return 0;
}
