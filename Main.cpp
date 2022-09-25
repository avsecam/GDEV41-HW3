#include <raylib.h>
#include <raymath.h>
#include <iostream>


const int TARGET_FPS = 60;
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

const int BALL_RADIUS = 30;


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


void drawTable() {
    int holeRadius = BALL_RADIUS + 2;

    // Draw floor
    DrawRectangle(holeRadius, holeRadius, WINDOW_WIDTH - (holeRadius * 2), WINDOW_HEIGHT - (holeRadius * 2), GREEN);

    // Draw walls
    DrawRectangle(holeRadius, 0, WINDOW_WIDTH - (holeRadius * 2), holeRadius, DARKGREEN); // top
    DrawRectangle(holeRadius, WINDOW_HEIGHT - holeRadius, WINDOW_WIDTH - (holeRadius * 2), holeRadius, DARKGREEN); // bottom
    DrawRectangle(0, holeRadius, holeRadius, WINDOW_HEIGHT - (holeRadius * 2), DARKGREEN); // left
    DrawRectangle(WINDOW_WIDTH - holeRadius, holeRadius, holeRadius, WINDOW_HEIGHT - (holeRadius * 2), DARKGREEN); // right

    // Draw holes
    DrawCircle(holeRadius, holeRadius, holeRadius, BLACK);
    DrawCircle(WINDOW_WIDTH - holeRadius, holeRadius, holeRadius, BLACK);
    DrawCircle(WINDOW_WIDTH - holeRadius, WINDOW_HEIGHT - holeRadius, holeRadius, BLACK);
    DrawCircle(holeRadius, WINDOW_HEIGHT - holeRadius, holeRadius, BLACK);
}


int main() {
    Circle c;
    c.position.x = WINDOW_WIDTH / 2;
    c.position.y = WINDOW_HEIGHT / 2;
    c.mass = 1.0f;
    c.radius = BALL_RADIUS;
    c.color = RED;

    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Physics, Collision");
    SetTargetFPS(TARGET_FPS);
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(WHITE);
        
        drawTable();
        c.draw();
        
        EndDrawing();
    }
    
    CloseWindow();
    return 0;
}


