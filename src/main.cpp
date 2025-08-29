#include <string>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include "common/common.hpp"
#include "logic/ManipulatorArm.hpp"
#include "logic/Box.hpp"
#include "logic/Controller.hpp"
#include "logic/GameWorld.hpp"
#include "ui/GuiEngine.hpp"
#include <iostream>
#include <chrono>
#include "logic/CustomMachines.hpp"
#include "graphics/GraphicsEngine.hpp"

long long tick = 0;
long long eventCounter = 0;

GameWorld gameWorld{}; // should be singleton? maybe not?
ALLEGRO_FONT* debug_font = nullptr;

void init() {
    GuiEngine::getInstance()->init();

    CameraParameters parameters;
    parameters.fov = 900;
    parameters.displaySize = Point2d(900, 900);
    parameters.position = Point2d(600, 300);
    parameters.z = -400; // fov = 90 deg

    GraphicsEngine::instance()->setCameraParameters(parameters);

    // TEST 
    auto arm1 = gameWorld.addManipulatorArm(3, Point2d(200, 510));
    auto arm2 = gameWorld.addManipulatorArm(3, Point2d(600, 510));
    auto creator3 = new BoxGenerator(Point2d(100, 505), &gameWorld);
    creator3->setPeriod(300);
    creator3->setType(BoxContent::IronPlate);
    auto furnace4 = new Furnace(Point2d(280, 460), &gameWorld);
    auto destoyer5 = new BoxDestroyer(Point2d(700, 505), &gameWorld);
    auto assembler6 = new PlateCombiner(Point2d(440, 505), &gameWorld);
    // auto furnace4 = gameWorld.addMachine(Point2d(300, 400));
    // auto destoyer5 = gameWorld.addMachine(Point2d(300, 400));

    auto controller7 = gameWorld.addController(Point2d(900, 370));
    controller7->addInstruction("delay 50");
    controller7->addInstruction("delay 100");
    controller7->addInstruction("angle 1 0 180");
    controller7->addInstruction("angle 1 1 270");
    controller7->addInstruction("delay 400");
    controller7->addInstruction("grab 1");
    controller7->addInstruction("delay 10");
    controller7->addInstruction("angle 1 0 190"); // move box 1 to furnace
    controller7->addInstruction("angle 1 1 100");
    controller7->addInstruction("delay 400");
    controller7->addInstruction("release 1");
    controller7->addInstruction("delay 10");

    controller7->addInstruction("angle 1 0 180");
    controller7->addInstruction("angle 1 1 270");
    controller7->addInstruction("delay 400");
    controller7->addInstruction("grab 1");
    controller7->addInstruction("delay 10");
    controller7->addInstruction("angle 1 0 180"); // move box 2 to furnace
    controller7->addInstruction("angle 1 1 125");
    controller7->addInstruction("delay 400");
    controller7->addInstruction("release 1");
    controller7->addInstruction("delay 10");

    controller7->addInstruction("angle 1 0 190"); // grab box 1 from furnace
    controller7->addInstruction("angle 1 1 100");
    controller7->addInstruction("delay 200");
    controller7->addInstruction("grab 1");
    controller7->addInstruction("delay 10");
    controller7->addInstruction("angle 1 0 232"); // move box 1 to assembler
    controller7->addInstruction("angle 1 1 125");
    controller7->addInstruction("delay 200");
    controller7->addInstruction("release 1");
    controller7->addInstruction("delay 10");

    controller7->addInstruction("angle 1 0 180"); // grab box 2 from furnace
    controller7->addInstruction("angle 1 1 125");
    controller7->addInstruction("delay 200");
    controller7->addInstruction("grab 1");
    controller7->addInstruction("delay 10");
    controller7->addInstruction("angle 1 0 238"); // move box 2 to assembler
    controller7->addInstruction("angle 1 1 102");
    controller7->addInstruction("delay 200");
    controller7->addInstruction("release 1");
    controller7->addInstruction("delay 10");

    controller7->addInstruction("goto 1");

    auto controller8 = gameWorld.addController(Point2d(1000, 370));

    controller8->addInstruction("delay 50");
    controller8->addInstruction("delay 100");
    controller8->addInstruction("angle 2 0 200");
    controller8->addInstruction("angle 2 1 103");
    controller8->addInstruction("delay 400");
    controller8->addInstruction("grab 2");
    controller8->addInstruction("delay 10");
    controller8->addInstruction("angle 2 0 0");
    controller8->addInstruction("angle 2 1 90");
    controller8->addInstruction("delay 400");
    controller8->addInstruction("release 2");
    controller8->addInstruction("delay 10");

    controller8->addInstruction("goto 1");

    // auto window = GuiEngine::getInstance()->addWindow(Rect2d(Point2d(300, 300), 400, 200), true, true);
    // auto button = window->addButton(Rect2d(Point2d(20, 40), Point2d(120.5, 80.5)));
    // button->setOnClickCallback([](){std::cout << "Hello!" << std::endl;});
    // window->addLabel(button->getRect().center(), true, "Some text", 0);
    // window->addLabel(Point2d(20, 90), false, "Some text", 0);
    // window->addLabel(Point2d(20, 90), false, "More text", 1);
    // window->addLabel(Point2d(20, 90), false, "Hello, World!!!", 2);
    // LabelFlags flags;
    // flags.background = true;
    // window->addLabel(Point2d(20, 90), false, "Highlighted", 3)->setFlags(flags);
    controller7->createWindow();
    
}

void redraw() {
    // auto start = std::chrono::system_clock::now();
    gameWorld.drawAll();
    al_set_target_bitmap(al_get_backbuffer(al_get_current_display()));
    al_clear_to_color(al_map_rgb(0, 0, 0));

    al_hold_bitmap_drawing(true);
    GraphicsEngine::instance()->draw();
    al_hold_bitmap_drawing(false);


    // gameWorld.getController(6)->drawInstructions();

    al_hold_bitmap_drawing(true);
    GuiEngine::getInstance()->draw();
    al_hold_bitmap_drawing(false);

    al_hold_bitmap_drawing(true);

    al_draw_text(debug_font, al_map_rgb(255, 255, 255), 10, 10, 0, "Programing game");
    al_draw_text(debug_font, al_map_rgb(255, 255, 255), 10, 20, 0, "Current tick:");
    al_draw_text(debug_font, al_map_rgb(255, 255, 255), 110, 20, 0, std::to_string(tick).c_str());
    al_draw_text(debug_font, al_map_rgb(255, 255, 255), 150, 20, 0, "Events counter:");
    al_draw_text(debug_font, al_map_rgb(255, 255, 255), 270, 20, 0, std::to_string(eventCounter).c_str());

    al_hold_bitmap_drawing(false);
    GraphicsEngine::instance()->clearBitmaps();
    al_flip_display();

    // auto end = std::chrono::system_clock::now();
    // auto elapsed_ms = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    // std::cout << elapsed_ms << std::endl;
}

void update() {
    tick++;
    ALLEGRO_KEYBOARD_STATE keyboardState;
    al_get_keyboard_state(&keyboardState);

    CameraParameters camera = GraphicsEngine::instance()->getCameraParameters();
    if (al_key_down(&keyboardState, ALLEGRO_KEY_W)) camera.position.y -= 7;
    if (al_key_down(&keyboardState, ALLEGRO_KEY_S)) camera.position.y += 7;
    if (al_key_down(&keyboardState, ALLEGRO_KEY_A)) camera.position.x -= 7;
    if (al_key_down(&keyboardState, ALLEGRO_KEY_D)) camera.position.x += 7;
    if (al_key_down(&keyboardState, ALLEGRO_KEY_EQUALS)) {
        camera.z /= 1.01;
        camera.fov /= 1.01;
    }
    if (al_key_down(&keyboardState, ALLEGRO_KEY_MINUS)) {
        camera.z *= 1.01;
        camera.fov *= 1.01;
    }
    GraphicsEngine::instance()->setCameraParameters(camera);

    gameWorld.run();
}


void onKeyDown(int keycode) {
    switch (keycode) {
        case ALLEGRO_KEY_0:
            gameWorld.getController(6)->pause();
            break;
        case ALLEGRO_KEY_1:
            gameWorld.getController(6)->next();
            break;
        case ALLEGRO_KEY_2:
            gameWorld.getController(6)->unpause();
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
            case ALLEGRO_EVENT_DISPLAY_RESIZE:
                al_acknowledge_resize(event.display.source);
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
    ALLEGRO_EVENT_QUEUE *event_queue = nullptr;
    event_queue = al_create_event_queue();
    if (!event_queue) {
        fprintf(stderr, "Failed to create event queue.");
        return 1;
    }

    debug_font = al_load_ttf_font("./resources/clacon2.ttf", 14, 0);
    GameObject::debug_font = debug_font;
    GuiEngine::debug_font = al_load_ttf_font("./resources/clacon2.ttf", 14, 0);
    GraphicsEngine::instance()->loadImages();

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
    al_set_new_display_flags(ALLEGRO_RESIZABLE);
    display = al_create_display(900, 900);
    al_register_event_source(event_queue, al_get_display_event_source(display));

    init();
    gameWorld.run();

    mainLoop(event_queue);

    al_destroy_display(display);
    al_destroy_event_queue(event_queue);
    al_destroy_font(debug_font);

    return 0;
}