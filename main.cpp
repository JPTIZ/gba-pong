#include <libgba-cpp/arch/display/video.h>
#include <libgba-cpp/arch/display/layers.h>

#include <libgba-cpp/utils/geometry.h>


const static auto WHITE = gba::display::Color{31, 31, 31};
const static auto BLACK = gba::display::Color{0, 0, 0};


void set_pixel(int x, int y, gba::display::Color const& color) {
    ((unsigned short*)0x0600'0000)[x + 240 * y] = color.value();
}

void draw_line_mid() {
    namespace display = gba::display;

    for (auto y = 0; y < 160; ++y) {
        set_pixel(119, y, WHITE);
        set_pixel(120, y, WHITE);
    }
}

void draw_ball(int x, int y, gba::display::Color const& color) {
    namespace display = gba::display;

    set_pixel(x, y, color);
    set_pixel(x + 1, y, color);
    set_pixel(x, y + 1, color);
    set_pixel(x + 1, y + 1, color);
}

void draw_player(int x, int y, gba::display::Color const& color) {
    constexpr auto PLAYER_WIDTH = 4;
    constexpr auto PLAYER_HEIGHT = 16;

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
    auto ball_direction = Point{-2, 0};

    while (true) {
        // Game logic
        auto old_ball_location = ball_location;

        if (ball_location.x <= 0 || ball_location.x >= 237) {
            ball_direction.x *= -1;
        }
        ball_location.x += ball_direction.x;
        ball_location.y += ball_direction.y;

        // Draw
        display::vsync();

        draw_line_mid();
        draw_ball(old_ball_location.x, old_ball_location.y, BLACK);
        draw_ball(ball_location.x, ball_location.y, WHITE);
        draw_player(0, 60, WHITE);
        draw_player(236, 60, WHITE);
    }
}
