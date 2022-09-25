#include <raylib.h>
#include <raymath.h>
#include <iostream>


const int TARGET_FPS = 60;
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

const int BALL_COUNT = 4;
const float BALL_MASS = 1.0f;
const int BALL_RADIUS = 30;
const Vector2 CUE_START_POSITION = {200, WINDOW_HEIGHT / 2};


struct Circle {
    Vector2 position;
    Vector2 velocity;
    Vector2 acceleration;

    float mass = BALL_MASS;
    int radius = BALL_RADIUS;
    Color color = RED;
    

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
    // Setup balls
    Circle* balls = new Circle[BALL_COUNT];
    balls[0].position.x = 495;
    balls[0].position.y = WINDOW_HEIGHT / 2;
    balls[1].position.x = 545;
    balls[1].position.y = (WINDOW_HEIGHT / 2) - 35;
    balls[2].position.x = 595;
    balls[2].position.y = WINDOW_HEIGHT / 2;
    balls[3].position.x = 545;
    balls[3].position.y = (WINDOW_HEIGHT / 2) + 35;

    // Setup cue
    Circle* cue = new Circle;
    cue->position = CUE_START_POSITION;
    cue->color = WHITE;


    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Physics, Collision");
    SetTargetFPS(TARGET_FPS);
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(WHITE);
        
        drawTable();
        for (int i = 0; i < BALL_COUNT; i++) {
            balls[i].draw();
        }
        cue->draw();
        
        EndDrawing();
    }
    
    CloseWindow();
    return 0;
}


