#include <libgba-cpp/arch/display/video.h>
#include <libgba-cpp/arch/display/layers.h>

#include <libgba-cpp/utils/geometry.h>


const static auto WHITE = gba::display::Color{31, 31, 31};
const static auto BLACK = gba::display::Color{0, 0, 0};
const static auto RED = gba::display::Color{31, 0, 0};

constexpr auto PLAYER_WIDTH = 4;
constexpr auto PLAYER_HEIGHT = 16;

constexpr auto SCREEN_SIZE = gba::geometry::Size{
    gba::display::mode3::screen_width,
    gba::display::mode3::screen_height,
};


void set_pixel(int x, int y, gba::display::Color const& color) {
    gba::display::mode3::vram(x, y) = color.value();
}

void draw_line_mid() {
    for (auto y = 0; y < 160; ++y) {
        set_pixel(119, y, WHITE);
        set_pixel(120, y, WHITE);
    }
}

void draw_ball(int x, int y, gba::display::Color const& color) {
    set_pixel(x, y, color);
    set_pixel(x + 1, y, color);
    set_pixel(x, y + 1, color);
    set_pixel(x + 1, y + 1, color);
}

void draw_player(gba::geometry::Point const& location, gba::display::Color const& color) {
    auto [x, y] = location;

    for (auto l = y; l < y + PLAYER_HEIGHT; ++l) {
        for (auto c = x; c < x + PLAYER_WIDTH; ++c) {
            set_pixel(c, l, color);
        }
    }
}

int main() {
    namespace display = gba::display;
    using gba::geometry::Point;

    display::change_mode(display::Mode::MODE3);

    display::layer_visible(display::Layer::BG2);

    auto ball_location = Point{10, 15};
    auto ball_direction = Point{-2, -2};

    auto player_location = Point{0, 60};

    auto player2_location = Point{SCREEN_SIZE.width - PLAYER_WIDTH - 1, 60};
    auto player2_direction = Point{0, -1};

    while (true) {
        // Game logic
        auto old_ball_location = ball_location;
        auto old_player2_location = player2_location;

        // Screen size is 240x160, so value ranges [0..239, 0..159].
        // For the upper index we must consider that the ball starts -2 pixels
        // from it, because its reference is top-left and the ball has a size
        // of 2x2.
        if (ball_location.x <= 0 or ball_location.x >= SCREEN_SIZE.width - 3) {
            ball_direction.x *= -1;
        }

        if (ball_location.y <= 0 or ball_location.y >= SCREEN_SIZE.height - 3) {
            ball_direction.y *= -1;
        }
        ball_location.x += ball_direction.x;
        ball_location.y += ball_direction.y;

        //   Update opponent location
        if (player2_location.y == 0 or player2_location.y == 159 - PLAYER_HEIGHT) {
            player2_direction.y *= -1;
        }

        player2_location.y += player2_direction.y;

        // Draw
        display::vsync();

        //   Clear objects
        draw_ball(old_ball_location.x, old_ball_location.y, BLACK);
        draw_player(old_player2_location, BLACK);

        //   Draw background
        draw_line_mid();

        //   Draw objects
        draw_ball(ball_location.x, ball_location.y, WHITE);
        draw_player(player_location, WHITE);
        draw_player(player2_location, WHITE);
    }
}
