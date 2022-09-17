//
// Created by n3dry on 17.09.22.
//

#pragma once

struct Rect {
    union {
        struct {
            int x, y;
        };
        struct {
            int width, height;
        };
    };
};

