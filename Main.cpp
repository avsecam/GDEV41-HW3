#include <raylib.h>
#include <raymath.h>
#include <iostream>


const int TARGET_FPS = 60;
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;


struct Circle {
    Vector2 position;
    Vector2 velocity;
    Vector2 acceleration;

    float mass;
    int radius;
    Color color;


    void draw() {
        DrawCircle(position.x, position.y, radius, color);
    }
};


int main() {
    Circle c;
    c.position.x = WINDOW_WIDTH / 2;
    c.position.y = WINDOW_HEIGHT / 2;
    c.mass = 1.0f;
    c.radius = 30;
    c.color = RED;

    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Physics, Collision");
    SetTargetFPS(TARGET_FPS);
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);

        c.draw();
        
        EndDrawing();
    }
    
    CloseWindow();
    return 0;
}


