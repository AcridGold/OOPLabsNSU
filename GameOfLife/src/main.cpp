#include "raylib.h"
#include "Simulation.h"

int main() {
    const int WINDOW_WIDTH = 1920;
    const int WINDOW_HEIGHT = 1200;
    const int CELL_SIZE = 10;
    const int FPS = 10;
    int currentTargetFPS = FPS;

    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Conway's Game of Life");
    SetTargetFPS(currentTargetFPS);

    Simulation simulation(WINDOW_WIDTH, WINDOW_HEIGHT, CELL_SIZE);

    // Modal dialog state
    bool showClearDialog = false;

    // Button dimensions
    const int BUTTON_WIDTH = 300;
    const int BUTTON_HEIGHT = 100;
    const int BUTTON_SPACING = 100;

    while (!WindowShouldClose()) {
        if (!showClearDialog) {
            if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
                Vector2 mousePos = GetMousePosition();
                int row = static_cast<int>(mousePos.y) / CELL_SIZE;
                int column = static_cast<int>(mousePos.x) / CELL_SIZE;
                simulation.ToggleCell(row, column);
            }

            if (IsKeyPressed(KEY_ENTER)) {
                simulation.Start();
                SetWindowTitle("Conway's Game of Life - Running");
            }

            if (IsKeyPressed(KEY_SPACE)) {
                simulation.Stop();
                SetWindowTitle("Conway's Game of Life - Paused");
            }

            if (IsKeyPressed(KEY_R)) {
                simulation.CreateRandomState();
            }

            if (IsKeyPressed(KEY_C)) {
                showClearDialog = true;
            }

            if (IsKeyPressed(KEY_F)) {
                currentTargetFPS = (currentTargetFPS == 10) ? 60 : 10;
                SetTargetFPS(currentTargetFPS);
            }

            simulation.Update();
        }

        // Drawing
        BeginDrawing();
        ClearBackground(Color{25, 25, 25, 255});
        simulation.Draw();

        // Instructions
        DrawText("ENTER - Start | SPACE - Pause | R - Random | C - Clear | F - Speed", 10, 10, 20, LIGHTGRAY);
        DrawText(TextFormat("%s | Target FPS: %d", simulation.IsRunning() ? "Running" : "Paused", currentTargetFPS), WINDOW_WIDTH - 400, 10, 20, simulation.IsRunning() ? GREEN : RED);

        // Подтверждение очистки
        if (showClearDialog) {
            // Затемнение фона
            DrawRectangle(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, Color{0, 0, 0, 180});

            int dialogWidth = 800;
            int dialogHeight = 400;
            int dialogX = (WINDOW_WIDTH - dialogWidth) / 2;
            int dialogY = (WINDOW_HEIGHT - dialogHeight) / 2;

            DrawRectangle(dialogX, dialogY, dialogWidth, dialogHeight, Color{50, 50, 50, 255});
            DrawRectangleLines(dialogX, dialogY, dialogWidth, dialogHeight, WHITE);

            const char* questionText = "Want to clear?";
            int textWidth = MeasureText(questionText, 30);
            DrawText(questionText, dialogX + (dialogWidth - textWidth) / 2, dialogY + 40, 30, WHITE);

            int buttonY = dialogY + 120;
            int yesButtonX = dialogX + (dialogWidth / 2) - BUTTON_WIDTH - BUTTON_SPACING / 2;
            int noButtonX = dialogX + (dialogWidth / 2) + BUTTON_SPACING / 2;

            Rectangle yesButton = {static_cast<float>(yesButtonX), static_cast<float>(buttonY),
                                   static_cast<float>(BUTTON_WIDTH), static_cast<float>(BUTTON_HEIGHT)};
            Rectangle noButton = {static_cast<float>(noButtonX), static_cast<float>(buttonY),
                                  static_cast<float>(BUTTON_WIDTH), static_cast<float>(BUTTON_HEIGHT)};

            Vector2 mousePos = GetMousePosition();
            bool yesHovered = CheckCollisionPointRec(mousePos, yesButton);
            bool noHovered = CheckCollisionPointRec(mousePos, noButton);

            // "Да"
            DrawRectangleRec(yesButton, yesHovered ? GREEN : Color{0, 150, 0, 255});
            DrawRectangleLinesEx(yesButton, 2, WHITE);
            int yesTextWidth = MeasureText("Yes", 25);
            DrawText("Yes", yesButtonX + (BUTTON_WIDTH - yesTextWidth) / 2, buttonY + 12, 25, WHITE);

            // "Нет"
            DrawRectangleRec(noButton, noHovered ? RED : Color{150, 0, 0, 255});
            DrawRectangleLinesEx(noButton, 2, WHITE);
            int noTextWidth = MeasureText("No", 25);
            DrawText("No", noButtonX + (BUTTON_WIDTH - noTextWidth) / 2, buttonY + 12, 25, WHITE);

            // Обработка кликов
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                if (yesHovered) {
                    simulation.ClearGrid();
                    showClearDialog = false;
                } else if (noHovered) {
                    showClearDialog = false;
                }
            }

            if (IsKeyPressed(KEY_ESCAPE)) {
                showClearDialog = false;
            }
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
