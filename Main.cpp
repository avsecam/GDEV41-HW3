#include <raylib.h>
#include <raymath.h>

#include <iostream>

const int TARGET_FPS(60);
const float TIMESTEP(1.0f / TARGET_FPS);
const int WINDOW_WIDTH(800);
const int WINDOW_HEIGHT(600);

const int GUIDE_THICKNESS(3);
const int BALL_COUNT(5);
const float BALL_MASS(0.5f);
const int BALL_RADIUS(30);
const Vector2 CUE_START_POSITION({200, WINDOW_HEIGHT / 2});

const float FRICTION(-0.75f);
const float VELOCITY_THRESHOLD(5.0f);

const int FORCE_MULTIPLIER(50);
const float ELASTICITY(0.5f);

struct Circle {
  Vector2 position = {0.0f, 0.0f};
  Vector2 velocity = {0.0f, 0.0f};
  Vector2 acceleration = {0.0f, 0.0f};

  float mass = BALL_MASS;
  int radius = BALL_RADIUS;
  Color color = RED;

  void draw() { DrawCircle(position.x, position.y, radius, color); }

  void update(Vector2 force = {0.0f, 0.0f}, float timestep = TIMESTEP) {
    acceleration = Vector2Add(
      Vector2Scale(force, 1 / mass), (Vector2Scale(velocity, FRICTION))
    );  // Sum of all forces
    velocity = Vector2Add(velocity, Vector2Scale(acceleration, TIMESTEP));
    velocity.x = (abs(velocity.x) < VELOCITY_THRESHOLD) ? 0.0f : velocity.x;
    velocity.y = (abs(velocity.y) < VELOCITY_THRESHOLD) ? 0.0f : velocity.y;
    position = Vector2Add(position, Vector2Scale(velocity, TIMESTEP));
  }

  bool isMoving() {
    if (velocity.x != 0.0f || velocity.y != 0.0f)
      return true;
    else
      return false;
  }
};

void drawTable() {
  int holeRadius = BALL_RADIUS + 2;

  // Draw floor
  DrawRectangle(
    holeRadius, holeRadius, WINDOW_WIDTH - (holeRadius * 2),
    WINDOW_HEIGHT - (holeRadius * 2), GREEN
  );

  // Draw walls
  DrawRectangle(
    holeRadius, 0, WINDOW_WIDTH - (holeRadius * 2), holeRadius, DARKGREEN
  );  // top
  DrawRectangle(
    holeRadius, WINDOW_HEIGHT - holeRadius, WINDOW_WIDTH - (holeRadius * 2),
    holeRadius, DARKGREEN
  );  // bottom
  DrawRectangle(
    0, holeRadius, holeRadius, WINDOW_HEIGHT - (holeRadius * 2), DARKGREEN
  );  // left
  DrawRectangle(
    WINDOW_WIDTH - holeRadius, holeRadius, holeRadius,
    WINDOW_HEIGHT - (holeRadius * 2), DARKGREEN
  );  // right

  // Draw holes
  DrawCircle(holeRadius, holeRadius, holeRadius, BLACK);
  DrawCircle(WINDOW_WIDTH - holeRadius, holeRadius, holeRadius, BLACK);
  DrawCircle(
    WINDOW_WIDTH - holeRadius, WINDOW_HEIGHT - holeRadius, holeRadius, BLACK
  );
  DrawCircle(holeRadius, WINDOW_HEIGHT - holeRadius, holeRadius, BLACK);
}

float getImpulse(
  Circle a, Circle b, Vector2 relativeVelocity, Vector2 collisionNormal
) {
  float impulse(-(
    ((1.0f + ELASTICITY) *
     (Vector2DotProduct(relativeVelocity, collisionNormal)) /
     (Vector2DotProduct(collisionNormal, collisionNormal) *
      ((1.0f / a.mass) + (1.0f / b.mass))))
  ));

  return impulse;
}

void resetTable(Circle* balls) {
  balls[0].position = CUE_START_POSITION;

  balls[1].position = {495, WINDOW_HEIGHT / 2};
  balls[2].position = {545, (WINDOW_HEIGHT / 2) - 35};
  balls[3].position = {595, WINDOW_HEIGHT / 2};
  balls[4].position = {545, (WINDOW_HEIGHT / 2) + 35};
}

int main() {
  // Setup balls
  Circle* balls = new Circle[BALL_COUNT];
  resetTable(balls);

  // Setup cue
  Circle* cue = &balls[0];
  cue->color = WHITE;

  bool isPlayersTurn(false);

  bool mouseStartedDragging(false);
  Vector2 mouseDragStartPosition;
  Vector2 mousePosition;

  Vector2 hitForce({0.0f, 0.0f});  // Force when releasing mouse to hit cue ball

  float accumulator(0.0f);
  float deltaTime;

  // Sounds
  InitAudioDevice();
  Sound ballHit = LoadSound("ball_hit.wav");

  InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Physics, Collision");
  SetTargetFPS(TARGET_FPS);
  while (!WindowShouldClose()) {
    deltaTime = GetFrameTime();

    BeginDrawing();
    ClearBackground(WHITE);

    drawTable();

    isPlayersTurn = true;
    // Check if balls aren't moving
    for (int i = 0; i < BALL_COUNT; i++) {
      if (balls[i].isMoving()) {
        isPlayersTurn = false;
        break;
      }
    }

    if (isPlayersTurn)
      DrawText(
        "Your move. Press R to reset table.", 130, 2, BALL_RADIUS, YELLOW
      );

    // Input
    if (IsKeyPressed(KEY_R) && isPlayersTurn) {
      resetTable(balls);
    }

    mousePosition = GetMousePosition();
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
      if (isPlayersTurn) {
        if (!mouseStartedDragging) {
          mouseDragStartPosition = mousePosition;
          mouseStartedDragging = true;
        } else {
          DrawLineEx(
            mouseDragStartPosition, mousePosition, GUIDE_THICKNESS, YELLOW
          );
        }
      }
    } else {
      hitForce = {0.0f, 0.0f};
      if (mouseStartedDragging) {
        // Hit cue
        hitForce = Vector2Scale(
          (Vector2Subtract(mousePosition, mouseDragStartPosition)),
          -FORCE_MULTIPLIER
        );
        mouseStartedDragging = false;
      }
    }

    // Physics
    accumulator += deltaTime;
    while (accumulator >= TIMESTEP) {
      // Collision detection
      // Collision detection between balls
      for (int i = 0; i < BALL_COUNT; i++) {
        for (int j = 0; j < BALL_COUNT; j++) {
          if (j == i) continue;

          Circle* a = &balls[i];
          Circle* b = &balls[j];

          float sumOfRadii(pow(BALL_RADIUS * 2, 2));
          float distanceBetweenCenters(
            Vector2DistanceSqr(a->position, b->position)
          );

          // Collision detected
          if (sumOfRadii >= distanceBetweenCenters) {
            Vector2 collisionNormalAB(
              {b->position.x - a->position.x, b->position.y - a->position.y}
            );
            Vector2 relativeVelocityAB(Vector2Subtract(a->velocity, b->velocity)
            );
            Vector2 collisionNormalABNormalized(
              Vector2Normalize(collisionNormalAB)
            );
            Vector2 relativeVelocityABNormalized(
              Vector2Normalize(relativeVelocityAB)
            );

            // I think we should also separate balls that are touching
            if (Vector2Length(relativeVelocityAB) <= 0.1f) {
              a->position = Vector2Subtract(
                a->position, Vector2Scale(collisionNormalABNormalized, 0.5f)
              );
              b->position = Vector2Add(
                b->position, Vector2Scale(collisionNormalABNormalized, 0.5f)
              );
            }

            // Set hit sound volume depending on strength of hit
            SetSoundVolume(
              ballHit,
              Remap(
                Vector2Length(relativeVelocityAB), 200.0f, 1000.0f, 0.0f, 1.0f
              )
            );
            PlaySoundMulti(ballHit);

            // Collision response
            // Check dot product between collision normal and relative velocity
            if (Vector2DotProduct(relativeVelocityABNormalized, collisionNormalABNormalized) > 0) {
              float impulse =
                getImpulse(*a, *b, relativeVelocityAB, collisionNormalAB);
              a->velocity = Vector2Add(
                a->velocity,
                Vector2Scale(
                  Vector2Scale(collisionNormalAB, 1.0f / a->mass), impulse
                )
              );
              b->velocity = Vector2Subtract(
                b->velocity,
                Vector2Scale(
                  Vector2Scale(collisionNormalAB, 1.0f / b->mass), impulse
                )
              );
            }
          }
        }
      }

      // Movement
      cue->update(hitForce, TIMESTEP);
      for (int i = 1; i < BALL_COUNT; i++) {
        balls[i].update();
      }
      accumulator -= TIMESTEP;
    }

    // Draw balls
    for (int i = 0; i < BALL_COUNT; i++) {
      balls[i].draw();
    }

    EndDrawing();
  }

  delete cue;
  delete[] balls;
  UnloadSound(ballHit);

  CloseWindow();
  CloseAudioDevice();
  return 0;
}
