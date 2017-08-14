#!/usr/bin/env python3
import random
from PIL import Image

FULL_WIDTH = FULL_HEIGHT = 512
FULL_COLORS = 6

TILE_WIDTH = TILE_HEIGHT = 4
TILE_COLORS = 4

RANDOMIZER_PASSES = 1

PALLETE = [
    (23, 37, 42),
    (43, 122, 120),
    (58, 175, 169),
    (159, 217, 215),
    (222, 242, 241),
    (238, 238, 238)
]

assert len(PALLETE) >= FULL_COLORS, "Not enough colors specified!"


def find_neighbors(width, height, x, y):
    for yc in range(-1, 2):
        for xc in range(-1, 2):
            if xc == 0 and yc == 0:
                continue

            nx = x + xc
            ny = y + yc

            if 0 <= nx < width and 0 <= ny < height:
                yield (nx, ny)


def find_neighbor_colors(pixels, neighbors):
    return {pixels[ny][nx] for nx, ny in neighbors} - {0}


def make_mosaic(width=512, height=512, color_amount=6):
    all_colors = set(range(1, 1 + color_amount))
    pixels = [[0 for _ in range(width)] for _ in range(height)]

    def recurse(x, y):
        if pixels[y][x] != 0:
            return

        neighbors = tuple(find_neighbors(width, height, x, y))
        available_colors = all_colors - find_neighbor_colors(pixels, neighbors)

        for color in available_colors:
            pixels[y][x] = color

            if set().union(*pixels) == all_colors:
                return pixels

            for nx, ny in neighbors:
                solution = recurse(nx, ny)

                if solution is not None:
                    return solution

        # Unset the pixel after we're done if we didn't find any solutions.
        pixels[y][x] = 0

    return recurse(0, 0)


def save_image(mosaic):
    image = Image.new("RGB", (len(mosaic[0]), len(mosaic)))

    pixels = image.load()
    for y, row in enumerate(mosaic):
        for x, col in enumerate(row):
            pixels[x, y] = PALLETE[col - 1]

    image.save("mosaic.png")


def main():
    small_mosaic = make_mosaic(TILE_WIDTH, TILE_HEIGHT, TILE_COLORS)
    big_mosaic = [[0 for _ in range(FULL_WIDTH)] for _ in range(FULL_HEIGHT)]

    for y in range(FULL_HEIGHT):
        for x in range(FULL_WIDTH):
            big_mosaic[y][x] = small_mosaic[y % TILE_HEIGHT][x % TILE_WIDTH]

    all_colors = set(range(1, FULL_COLORS + 1))
    for _ in range(RANDOMIZER_PASSES):
        for y in range(FULL_HEIGHT):
            for x in range(FULL_WIDTH):
                neighbors = find_neighbors(FULL_WIDTH, FULL_HEIGHT, x, y)
                neighbor_colors = find_neighbor_colors(big_mosaic, neighbors)
                available_colors = tuple(all_colors - neighbor_colors)
                big_mosaic[y][x] = random.choice(available_colors)

    if set().union(*big_mosaic) != all_colors:
        # This to me seems very unlikely to happen, but we want to be sure.
        print("Trying again...")
        main()
        return

    save_image(big_mosaic)


if __name__ == "__main__":
    main()
