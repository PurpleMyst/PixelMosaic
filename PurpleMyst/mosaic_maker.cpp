#include <array>
#include <iostream>
#include <random>
#include <unordered_set>
#include <vector>

constexpr auto SIDE = 512;
constexpr auto COLORS = 6;

constexpr auto RANDOMIZER_PASSES = 4;

constexpr uint8_t BLACK[3] = {0, 0, 0};

constexpr uint8_t PALLETE[COLORS][3] = {
    {23, 37, 42},
    {43, 122, 120},
    {58, 175, 169},
    {159, 217, 215},
    {222, 242, 241},
    {238, 238, 238},
};

typedef std::pair<size_t, size_t> Point;

class Image {
private:
    // Maps 1D coordinates to their pallete value + 1. 0 means not set.
    std::array<uint8_t, SIDE * SIDE> pixels{{0}};
public:
    uint8_t get_pixel(size_t x, size_t y) const {
        return pixels.at(y * SIDE + x);
    }

    void set_pixel(const size_t x, const size_t y, const uint8_t color) {
        pixels.at(y * SIDE + x) = color;
    }

    void output() const {
        std::cout << "P3" << std::endl
                  << SIDE << ' ' << SIDE << std::endl
                  << "255" << std::endl;

        for (size_t y = 0; y < SIDE; ++y) {
            for (size_t x = 0; x < SIDE; ++x) {
                if (x > 0) std::cout << ' ';

                const uint8_t pixel = get_pixel(x, y);
                const uint8_t *color = pixel == 0 ? BLACK : PALLETE[pixel - 1];

                for (size_t i = 0; i < 3; ++i) {
                    if (i > 0) std::cout << ' ';

                    // This is a weird trick, but it basically converts an
                    // uint8_t to a type printable as a number.
                    std::cout << +color[i];
                }
            }
            std::cout << std::endl;
        }
    }

    std::vector<Point> find_neighbors(const ssize_t x, const ssize_t y) const {
        std::vector<Point> neighbors;

        for (ssize_t xc = -1; xc <= 1; ++xc) {
            for (ssize_t yc = -1; yc <= 1; ++yc) {
                if (xc == 0 && yc == 0) continue;

                const ssize_t nx = x + xc;
                const ssize_t ny = y + yc;

                if (nx < 0 || ny < 0 || nx >= SIDE || ny >= SIDE) continue;

                neighbors.push_back(std::make_pair(nx, ny));
            }
        }

        return neighbors;
    }

    std::unordered_set<uint8_t> find_available_colors(const std::vector<Point> &neighbors) const {
        std::unordered_set<uint8_t> available_colors;
        for (size_t i = COLORS; i > 0; --i) available_colors.insert(i);

        for (const Point &neighbor : neighbors) {
            const auto neighbor_pixel = get_pixel(neighbor.first, neighbor.second);
            const auto it = available_colors.find(neighbor_pixel);
            if (it != available_colors.end()) available_colors.erase(it);
        }

        return available_colors;
    }
};


Image solve() {
    Image image;

    std::default_random_engine generator;
    std::uniform_int_distribution<uint8_t> distribution(1, COLORS);

    for (ssize_t i = RANDOMIZER_PASSES; i > 0; --i) {
        for (ssize_t y = SIDE - 1; y >= 0; --y) {
            for (ssize_t x = SIDE - 1; x >= 0; --x) {
                auto neighbors = image.find_neighbors(x, y);
                auto available_colors = image.find_available_colors(neighbors);

                uint8_t color;
                do {
                    color = distribution(generator);
                } while (!available_colors.count(color));

                image.set_pixel(x, y, color);
            }
        }
    }

    return image;
}

int main() {
    solve().output();
}
