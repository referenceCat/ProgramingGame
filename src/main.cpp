#include <string>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <common.hpp>
#include "logic/Arm.hpp"
#include "logic/Box.hpp"
#include "logic/Controller.hpp"
#include "logic/GameWorld.hpp"
#include "GuiEngine.hpp"
#include <iostream>
#include <chrono>
#include "logic/CustomMachinery.hpp"
#include "GraphicsEngine.hpp"
#include "logic/MachineryBuilder.hpp"
#include <format>

long long tick = 0;
long long eventCounter = 0;
bool drawDebug = false, drawInfo = false;

void init() {
    CameraParameters parameters;
    parameters.fov = 90;
    parameters.displayDimensions = Vector2d(900, 900);
    parameters.position = Vector2d(0, 0);
    parameters.z = -40; // fov = 90 deg

    GraphicsEngine::instance()->setCameraParameters(parameters);
    MachineryBuilder::instance()->createWindow();

    ModuleBuilder::instance()->createModulePrototype(LargeModule);
    ModuleBuilder::instance()->buildModule(true);

    auto box = new TapeBox(Vector2d(10, -10));
    GameWorld::instance()->addBox(box);

    // auto window = new Window(GuiEngine::instance()->getDisplayArea(), Aligment::byDimensionsAndCentered(Vector2d(500, 500)), true);
    // auto console = new Console(window->getInternalArea(), Aligment::byMargin(10, 10, 10, 10));
    // console->addLine("12313254");
    // console->addLine("Hello, world!");
    // for (int i = 0; i < 120; i++) {
    //     console->addLine("Hello, world! Line: " + std::to_string(i));
    // }
}

void redraw() {
    // auto start = std::chrono::system_clock::now();
    GameWorld::instance()->drawAll(drawInfo, drawDebug);
    MachineryBuilder::instance()->drawGhost();
    // GraphicsEngine::instance()->drawDebugBackgroung2();
    al_set_target_bitmap(al_get_backbuffer(al_get_current_display()));
    // al_hold_bitmap_drawing(true);
    al_clear_to_color(al_map_rgb(0, 0, 5));

    GraphicsEngine::instance()->drawLayers();
    GuiEngine::instance()->draw();

    al_draw_text(GraphicsEngine::instance()->debugFont, al_map_rgb(255, 255, 255), 10, 10, 0, "Programing game");
    al_draw_text(GraphicsEngine::instance()->debugFont, al_map_rgb(255, 255, 255), 10, 26, 0, std::format("Current tick: {}, Events counter: {}", tick, eventCounter).c_str());
    ALLEGRO_MOUSE_STATE mouseState;
    al_get_mouse_state(&mouseState);
    auto mousePos = GraphicsEngine::instance()->transformPointInverse(Vector2d(mouseState.x, mouseState.y));
    al_draw_text(GraphicsEngine::instance()->debugFont, al_map_rgb(255, 255, 255), 10, 42, 0, std::format("Mouse pos: {:.2f}, {:.2f}", mousePos.x, mousePos.y).c_str());

    // al_hold_bitmap_drawing(false);
    al_flip_display();
    GraphicsEngine::instance()->clearBitmaps();

    // auto end = std::chrono::system_clock::now();
    // auto elapsed_ms = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    // std::cout << elapsed_ms << std::endl;
}

void updateMouse() {
    ALLEGRO_MOUSE_STATE mouseState;
    al_get_mouse_state(&mouseState);
    if (!GuiEngine::instance()->updateMousePos(Vector2d(mouseState.x, mouseState.y), al_mouse_button_down(&mouseState, 1))) // working only with LMB for now
        ; 
    MachineryBuilder::instance()->mousePos(GraphicsEngine::instance()->transformPointInverse(Vector2d(mouseState.x, mouseState.y)));
}

void update() {
    tick++;

    if (!GuiEngine::instance()->handlingKeyboardInput()) {
        ALLEGRO_KEYBOARD_STATE keyboardState;
        al_get_keyboard_state(&keyboardState);

        CameraParameters camera = GraphicsEngine::instance()->getCameraParameters();
        if (al_key_down(&keyboardState, ALLEGRO_KEY_W))
            camera.position.y -= 1;
        if (al_key_down(&keyboardState, ALLEGRO_KEY_S))
            camera.position.y += 1;
        if (al_key_down(&keyboardState, ALLEGRO_KEY_A))
            camera.position.x -= 1;
        if (al_key_down(&keyboardState, ALLEGRO_KEY_D))
            camera.position.x += 1;
        if (al_key_down(&keyboardState, ALLEGRO_KEY_EQUALS)) {
            camera.z /= 1.01;
            camera.fov /= 1.01;
        }
        if (al_key_down(&keyboardState, ALLEGRO_KEY_MINUS)) {
            camera.z *= 1.01;
            camera.fov *= 1.01;
        }
        GraphicsEngine::instance()->setCameraParameters(camera);
    }

    updateMouse();
    GameWorld::instance()->run();
}

void onKeyDown(int keycode) {
    if (GuiEngine::instance()->handlingKeyboardInput()) {
        GuiEngine::instance()->keyboardKeyPress(keycode);
        return;
    }
    switch (keycode) {
        case ALLEGRO_KEY_R:
            GraphicsEngine::instance()->loadBitmaps();
            break;
        case ALLEGRO_KEY_ESCAPE:
            MachineryBuilder::instance()->clearItem();
            break;
        case ALLEGRO_KEY_F1:
            drawDebug = !drawDebug;
            break;
        case ALLEGRO_KEY_ALT:
            drawInfo = !drawInfo;
            break;
        default:
            break;
    }
}

void onKeyChar(char ch) {
    if (GuiEngine::instance()->handlingKeyboardInput()) {
        GuiEngine::instance()->keyboardCharPress(ch);
        return;
    }
}

void onMouseClick(double x, double y) {
    if (GuiEngine::instance()->click(Vector2d(x, y))) { // check if clicking on windows, buttons, etc
        return;
    }

    Vector2d gameWorldClickPos = GraphicsEngine::instance()->transformPointInverse(Vector2d(x, y));
    GameWorld::instance()->click(gameWorldClickPos);
    MachineryBuilder::instance()->onClick();
}

void onMouseRelease(double x, double y) {
    if (GuiEngine::instance()->releaseMouse(Vector2d(x, y))) { // check if clicking on windows, buttons, etc
        return;
    }
}

void onMouseWheel(double x, double y, bool up) {
    if (GuiEngine::instance()->moveMouseWheel(Vector2d(x, y), up)) { // check if clicking on windows, buttons, etc
        return;
    }
}

void mainLoop(ALLEGRO_EVENT_QUEUE* event_queue) {
    bool running = true;
    while (running) {
        ALLEGRO_EVENT event;
        // ALLEGRO_TIMEOUT timeout;
        // al_init_timeout(&timeout, 0.06);
        // bool get_event = al_wait_for_event_until(event_queue, &event, &timeout);
        al_wait_for_event(event_queue, &event);
        eventCounter++;
        switch (event.type) {
            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                running = false;
                break;
            case ALLEGRO_EVENT_TIMER:
                update();
                redraw();
                break;
            case ALLEGRO_EVENT_KEY_DOWN:
                onKeyDown(event.keyboard.keycode);
                break;
            case ALLEGRO_EVENT_KEY_CHAR:
                if (event.keyboard.unichar > 0)
                    onKeyChar(event.keyboard.unichar);
                break;
            case ALLEGRO_EVENT_DISPLAY_RESIZE:
                GraphicsEngine::instance()->changeDisplayDimensions(Vector2d(event.display.width, event.display.height));
                redraw();
                al_acknowledge_resize(event.display.source);
                break;
            case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
                onMouseClick(event.mouse.x, event.mouse.y);
                break;
            case ALLEGRO_EVENT_MOUSE_BUTTON_UP:
                onMouseRelease(event.mouse.x, event.mouse.y);
                break;
            case ALLEGRO_EVENT_MOUSE_AXES:
            case ALLEGRO_EVENT_MOUSE_ENTER_DISPLAY:
            case ALLEGRO_EVENT_MOUSE_LEAVE_DISPLAY:
                if (event.mouse.dz)
                    onMouseWheel(event.mouse.x, event.mouse.y, event.mouse.dz > 0);
                break;

            default:
                // fprintf(stderr, "Unsupported event received: %d\n", event.type);
                break;
        }
    }
}

int main(int argc, char** argv) {
    const int UPS = 60;

    // Initialize allegro
    if (!al_init()) {
        fprintf(stderr, "Failed to initialize allegro.\n");
        return 1;
    }

    // Initialize allegro font addon
    if (!al_init_font_addon()) {
        fprintf(stderr, "Failed to initialize allegro font addon.\n");
        return 1;
    }

    // Initialize allegro ttf addon
    if (!al_init_ttf_addon()) {
        fprintf(stderr, "Failed to initialize allegro ttf addon.\n");
        return 1;
    }

    // Initialize allegro image addon
    if (!al_init_image_addon()) {
        fprintf(stderr, "Failed to initialize allegro image addon.\n");
        return 1;
    }

    // Initialize allegro primitives addon
    if (!al_init_primitives_addon()) {
        fprintf(stderr, "Failed to primitives allegro ttf addon.\n");
        return 1;
    }

    // Create the event queue
    ALLEGRO_EVENT_QUEUE* event_queue = nullptr;
    event_queue = al_create_event_queue();
    if (!event_queue) {
        fprintf(stderr, "Failed to create event queue.");
        return 1;
    }

    GraphicsEngine::instance()->debugFont = al_load_ttf_font("./resources/clacon2.ttf", 16, 0);
    GuiEngine::debugFont = al_load_ttf_font("./resources/clacon2.ttf", 16, 0);
    GraphicsEngine::instance()->loadBitmaps();

    // Initialize user inputs
    al_install_keyboard();
    al_register_event_source(event_queue, al_get_keyboard_event_source());

    al_install_mouse();
    al_register_event_source(event_queue, al_get_mouse_event_source());

    // Initialize the timers
    ALLEGRO_TIMER* update_timer = al_create_timer(1.0 / UPS);
    al_register_event_source(event_queue, al_get_timer_event_source(update_timer));
    al_start_timer(update_timer);

    // Initialize the display
    ALLEGRO_DISPLAY* display = nullptr;
    al_set_new_display_option(ALLEGRO_VSYNC, 2, ALLEGRO_REQUIRE);
    al_set_new_display_flags(ALLEGRO_RESIZABLE);
    display = al_create_display(1920, 1080);
    al_register_event_source(event_queue, al_get_display_event_source(display));

    init();
    GameWorld::instance()->run();

    mainLoop(event_queue);

    al_destroy_display(display);
    al_destroy_event_queue(event_queue);
    al_destroy_font(GraphicsEngine::instance()->debugFont);

    return 0;
}