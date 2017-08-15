#include <array>
#include <iostream>
#include <random>
#include <unordered_set>
#include <vector>

constexpr size_t FULL_SIDE = 512;
constexpr size_t FULL_COLORS = 6;

constexpr size_t TILE_SIDE = 4;
constexpr size_t TILE_COLORS = 4;

constexpr size_t RANDOMIZER_PASSES = 4;

constexpr uint8_t BLACK[3] = {0, 0, 0};

constexpr uint8_t PALLETE[FULL_COLORS][3] = {
    {23, 37, 42},
    {43, 122, 120},
    {58, 175, 169},
    {159, 217, 215},
    {222, 242, 241},
    {238, 238, 238},
};

typedef std::pair<size_t, size_t> Point;

template <size_t Side, size_t Colors>
class Image {
private:
    // Maps 1D coordinates to their pallete value + 1. 0 means not set.
    std::array<uint8_t, Side * Side> pixels{{0}};
public:
    uint8_t get_pixel(size_t x, size_t y) const {
        return pixels.at(y * Side + x);
    }

    void set_pixel(const size_t x, const size_t y, const uint8_t color) {
        pixels.at(y * Side + x) = color;
    }

    void output() const {
        std::cout << "P3" << std::endl
                  << Side << ' ' << Side << std::endl
                  << "255" << std::endl;

        for (size_t y = 0; y < Side; ++y) {
            for (size_t x = 0; x < Side; ++x) {
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

    bool is_solution() const {
        // We assume that the condition of colors not touching is true, due to
        // how the rest of the program is coded.
        std::unordered_set<uint8_t> seen_colors;

        for (size_t y = 0; y < Side; ++y) {
            for (size_t x = 0; x < Side; ++x) {
                const uint8_t pixel = get_pixel(x, y);

                if (pixel == 0) {
                    return false;
                } else {
                    seen_colors.insert(pixel);
                }
            }
        }

        return seen_colors.size() == Colors;
    }

    std::vector<Point> find_neighbors(const ssize_t x, const ssize_t y) const {
        std::vector<Point> neighbors;

        for (ssize_t xc = -1; xc <= 1; ++xc) {
            for (ssize_t yc = -1; yc <= 1; ++yc) {
                if (xc == 0 && yc == 0) continue;

                const ssize_t nx = x + xc;
                const ssize_t ny = y + yc;

                if (nx < 0 || ny < 0 || nx >= Side || ny >= Side) continue;

                neighbors.push_back(std::make_pair(nx, ny));
            }
        }

        return neighbors;
    }

    std::unordered_set<uint8_t> find_available_colors(const std::vector<Point> &neighbors) const {
        std::unordered_set<uint8_t> available_colors;
        for (size_t i = Colors; i > 0; --i) available_colors.insert(i);

        for (const Point &neighbor : neighbors) {
            const auto neighbor_pixel = get_pixel(neighbor.first, neighbor.second);
            const auto it = available_colors.find(neighbor_pixel);
            if (it != available_colors.end()) available_colors.erase(it);
        }

        return available_colors;
    }
};

typedef Image<FULL_SIDE, FULL_COLORS> FullImage;
typedef Image<TILE_SIDE, TILE_COLORS> TileImage;

bool recurse(TileImage &tile, const size_t x, const size_t y) {
    if (tile.get_pixel(x, y) != 0) return false;

    auto neighbors = tile.find_neighbors(x, y);
    auto available_colors = tile.find_available_colors(neighbors);

    for (auto color : available_colors) {
        tile.set_pixel(x, y, color);

        if (tile.is_solution()) return true;

        for (const Point &neighbor : neighbors) {
            bool found_solution = recurse(tile, neighbor.first, neighbor.second);

            if (found_solution) return true;
        }
    }

    tile.set_pixel(x, y, 0);

    return false;
}

FullImage solve() {
    TileImage tile;
    recurse(tile, 0, 0);

    FullImage full;

    for (size_t y = 0; y < FULL_SIDE; ++y) {
        for (size_t x = 0; x < FULL_SIDE; ++x) {
            full.set_pixel(x, y, tile.get_pixel(x % TILE_SIDE, y % TILE_SIDE));
        }
    }

    std::default_random_engine generator;
    std::uniform_int_distribution<uint8_t> distribution(1, FULL_COLORS);

    for (ssize_t i = RANDOMIZER_PASSES; i > 0; --i) {
        for (ssize_t y = FULL_SIDE - 1; y >= 0; --y) {
            for (ssize_t x = FULL_SIDE - 1; x >= 0; --x) {
                auto neighbors = full.find_neighbors(x, y);
                auto available_colors = full.find_available_colors(neighbors);

                uint8_t  color;
                do {
                    color = distribution(generator);
                } while (!available_colors.count(color));

                full.set_pixel(x, y, color);
            }
        }
    }

    return full;
}

int main() {
    solve().output();
}
