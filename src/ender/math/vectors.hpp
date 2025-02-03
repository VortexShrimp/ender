#pragma once

namespace ender {
    struct vec2f {
        vec2f() : x(), y() {
        }
        vec2f(float x, float y) : x(x), y(y) {
        }

        float x;
        float y;
    };

    struct vec2i {
        vec2i() : x(), y() {
        }
        vec2i(int x, int y) : x(x), y(y) {
        }

        int x;
        int y;
    };

    struct vec3f {
        float x;
        float y;
        float z;
    };
}  // namespace ender
