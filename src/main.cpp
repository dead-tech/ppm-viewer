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

    fmt::println("debug: found P3 ppm image with width: {}, height: {}", parsed_ppm.width, parsed_ppm.height);

    std::size_t column_count = 0;
    for (const auto &pixel : parsed_ppm.pixels) {
        if (column_count == parsed_ppm.width) {
            fmt::print("\n");
            column_count = 0;
        }
        fmt::print("\x1b[48;2;{};{};{}m \x1b[0m", pixel.r, pixel.g, pixel.b);
        ++column_count;
    }
    fmt::print("\n");

    return 0;
}
