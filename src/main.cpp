#include <string>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include "common/common.hpp"
#include "logic/ManipulatorArm.hpp"
#include "logic/Box.hpp"
#include "logic/Controller.hpp"
#include "logic/GameWorld.hpp"
#include "ui/GuiEngine.hpp"
#include <iostream>
#include <chrono>

long long tick = 0;
long long eventCounter = 0;

GameWorld gameWorld{}; // should be singleton? maybe not?
ALLEGRO_FONT* debug_font = nullptr;

void init() {
    auto arm1 = gameWorld.addManipulatorArm(3, Point2d(200, 300));
    auto arm2 = gameWorld.addManipulatorArm(3, Point2d(400, 300));
    auto creator3 = gameWorld.addMachine(Point2d(100, 400));
    auto furnace4 = gameWorld.addMachine(Point2d(300, 400));
    auto destoyer5 = gameWorld.addMachine(Point2d(500, 400));

    auto controller6 = gameWorld.addController(Point2d(500, 500));
    controller6->addInstruction("delay 50");
    controller6->addInstruction("delay 100");
    controller6->addInstruction("create 3");
    controller6->addInstruction("destroy 5");
    controller6->addInstruction("angle 1 0 180");
    controller6->addInstruction("angle 1 1 270");
    controller6->addInstruction("angle 2 0 180");
    controller6->addInstruction("angle 2 1 270");
    controller6->addInstruction("delay 400");
    controller6->addInstruction("grab 1");
    controller6->addInstruction("grab 2");
    controller6->addInstruction("delay 10");
    controller6->addInstruction("angle 1 0 0");
    controller6->addInstruction("angle 1 1 90");
    controller6->addInstruction("angle 2 0 0");
    controller6->addInstruction("angle 2 1 90");
    controller6->addInstruction("delay 400");
    controller6->addInstruction("release 1");
    controller6->addInstruction("release 2");
    controller6->addInstruction("delay 10");
    controller6->addInstruction("goto 1");

    auto controller7 = gameWorld.addController(Point2d(500, 600));
    controller7->addInstruction("grill 4");
    controller7->addInstruction("goto 0");

    auto window = GuiEngine::getInstance()->addWindow(Rect2d(Point2d(300, 300), 400, 200), true, true);
    auto button = window->addButton(Rect2d(Point2d(20, 40), Point2d(120, 80)));
    button->setOnClickCallback([](){std::cout << "Hello!" << std::endl;});
    
}

void redraw() {
    // auto start = std::chrono::system_clock::now();
    al_set_target_bitmap(al_get_backbuffer(al_get_current_display()));
    al_clear_to_color(al_map_rgb(0, 0, 0));
    al_hold_bitmap_drawing(true);

    al_draw_text(debug_font, al_map_rgb(255, 255, 255), 10, 10, 0, "Programing game");
    al_draw_text(debug_font, al_map_rgb(255, 255, 255), 10, 20, 0, "Current tick:");
    al_draw_text(debug_font, al_map_rgb(255, 255, 255), 110, 20, 0, std::to_string(tick).c_str());
    al_draw_text(debug_font, al_map_rgb(255, 255, 255), 150, 20, 0, "Events counter:");
    al_draw_text(debug_font, al_map_rgb(255, 255, 255), 270, 20, 0, std::to_string(eventCounter).c_str());

    gameWorld.drawAll();
    gameWorld.getController(6)->drawInstructions();

    al_hold_bitmap_drawing(false);

    al_hold_bitmap_drawing(true);
    GuiEngine::getInstance()->draw();
    al_hold_bitmap_drawing(false);
    
    al_flip_display();
    // auto end = std::chrono::system_clock::now();
    // auto elapsed_ms = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    // std::cout << elapsed_ms << std::endl;
}

void update() {
    tick++;
    auto arm1 = gameWorld.getManipulatorArm(0);
    ALLEGRO_KEYBOARD_STATE keyboardState;
    al_get_keyboard_state(&keyboardState);
    if (al_key_down(&keyboardState, ALLEGRO_KEY_Q))  arm1->rotateJoint(0, RelativeRotation(-0.01));
    if (al_key_down(&keyboardState, ALLEGRO_KEY_E))  arm1->rotateJoint(0, RelativeRotation(0.01));
    if (al_key_down(&keyboardState, ALLEGRO_KEY_A))  arm1->rotateJoint(1, RelativeRotation(-0.01));
    if (al_key_down(&keyboardState, ALLEGRO_KEY_D))  arm1->rotateJoint(1, RelativeRotation(0.01));

    gameWorld.run();
}

void onGrabKey() {
    auto arm1 = gameWorld.getManipulatorArm(0);
    auto box0 = gameWorld.getBox(0);
    if (arm1->isActive()) {
        arm1->release();
    } else {
        arm1->grab();
    }
}


void onKeyDown(int keycode) {
    switch (keycode) {
        case ALLEGRO_KEY_SPACE:
            onGrabKey();
            break;
                    
        default:
            break;
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
                std::cout << event.keyboard.keycode << std::endl;
                onKeyDown(event.keyboard.keycode);
                break;
            case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
                GuiEngine::getInstance()->click(Point2d(event.mouse.x, event.mouse.y));
                break;
            case ALLEGRO_EVENT_MOUSE_BUTTON_UP:
                GuiEngine::getInstance()->releaseMouse(Point2d(event.mouse.x, event.mouse.y));
                break;
            case ALLEGRO_EVENT_MOUSE_AXES: case ALLEGRO_EVENT_MOUSE_ENTER_DISPLAY: case ALLEGRO_EVENT_MOUSE_LEAVE_DISPLAY:
                GuiEngine::getInstance()->moveMouse(Point2d(event.mouse.x, event.mouse.y));
                break;

            default:
                // fprintf(stderr, "Unsupported event received: %d\n", event.type);
                break;
        }
    }
    
}

int main(int argc, char **argv) {
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

    debug_font = al_load_ttf_font("./resources/clacon2.ttf", 14, 0);
    GameObject::debug_font = debug_font;

    // Initialize allegro primitives addon
    if (!al_init_primitives_addon()) {
        fprintf(stderr, "Failed to primitives allegro ttf addon.\n");
        return 1;
    }

    // Create the event queue
    ALLEGRO_EVENT_QUEUE *event_queue = nullptr;
    event_queue = al_create_event_queue();
    if (!event_queue) {
        fprintf(stderr, "Failed to create event queue.");
        return 1;
    }

    //Initialize user inputs
    al_install_keyboard();
    al_register_event_source(event_queue, al_get_keyboard_event_source());

    al_install_mouse();
    al_register_event_source(event_queue, al_get_mouse_event_source());

    // Initialize the timers
    ALLEGRO_TIMER *update_timer = al_create_timer(1.0 / UPS);
    al_register_event_source(event_queue, al_get_timer_event_source(update_timer));
    al_start_timer(update_timer);

    // Initialize the display
    ALLEGRO_DISPLAY *display = nullptr;
    al_set_new_display_option(ALLEGRO_VSYNC, 2, ALLEGRO_REQUIRE);
    display = al_create_display(700, 700);
    al_register_event_source(event_queue, al_get_display_event_source(display));

    init();
    gameWorld.run();

    mainLoop(event_queue);

    al_destroy_display(display);
    al_destroy_event_queue(event_queue);
    al_destroy_font(debug_font);

    return 0;
}