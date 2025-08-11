#include <string>
#include "common.hpp"
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include "ManipulatorArm.hpp"
#include "Box.hpp"
#include "Controller.hpp"

long long tick = 0;

ALLEGRO_FONT *debug_font = nullptr;
ManipulatorArm arm0(3);
Box box0(Rect2d(Point2d(300, 300), 30, 30));
Controller controller(Point2d(500, 100));

void redraw() {
    al_set_target_bitmap(al_get_backbuffer(al_get_current_display()));
    al_clear_to_color(al_map_rgb(0, 0, 0));
    al_hold_bitmap_drawing(true);

    al_draw_text(debug_font, al_map_rgb(255, 255, 255), 10, 10, 0, "Hello, World!");
    al_draw_text(debug_font, al_map_rgb(255, 255, 255), 10, 20, 0, "Current tick:");
    al_draw_text(debug_font, al_map_rgb(255, 255, 255), 110, 20, 0, std::to_string(tick).c_str());

    arm0.draw();
    box0.draw();
    controller.draw();
    controller.drawInstructions();
    controller.drawRegisters();

    al_hold_bitmap_drawing(false);
    al_flip_display();
}

void update() {
    ALLEGRO_KEYBOARD_STATE keyboardState;
    al_get_keyboard_state(&keyboardState);
    if (al_key_down(&keyboardState, ALLEGRO_KEY_Q))  arm0.rotateJoint(0, RelativeRotation(-0.01));
    if (al_key_down(&keyboardState, ALLEGRO_KEY_E))  arm0.rotateJoint(0, RelativeRotation(0.01));
    if (al_key_down(&keyboardState, ALLEGRO_KEY_A))  arm0.rotateJoint(1, RelativeRotation(-0.01));
    if (al_key_down(&keyboardState, ALLEGRO_KEY_D))  arm0.rotateJoint(1, RelativeRotation(0.01));

    controller.execNextInstr();

    arm0.moveToTarget();
    arm0.recalculate();
}

void onGrabKey() {
    if (arm0.isActive()) {
        arm0.release();
    } else {
        arm0.grab();
        if (box0.getRect().isInside(arm0.getLastJointPos())) {
            arm0.takeBox(&box0);
        }
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

void init() {
    arm0.setRootJointPosition(Point2d(200, 200));
    arm0.setSegmentLength(0, 100);
    arm0.setSegmentLength(1, 100);
    arm0.cleatTarget();
    arm0.recalculate();

    controller.setArm(&arm0);
    controller.setBox(&box0);
    controller.addInstruction("delay 50");
    controller.addInstruction("angle 0 0");
    controller.addInstruction("angle 1 90");
    controller.addInstruction("delay 300");
    controller.addInstruction("grab");
    controller.addInstruction("delay 100");
    controller.addInstruction("angle 0 270");
    controller.addInstruction("angle 1 30");
    controller.addInstruction("delay 400");
    controller.addInstruction("release");
    controller.addInstruction("delay 100");
    controller.addInstruction("angle 0 270");
    controller.addInstruction("angle 1 60");
    controller.addInstruction("delay 300");
    controller.addInstruction("angle 0 270");
    controller.addInstruction("angle 1 30");
    controller.addInstruction("delay 300");
    controller.addInstruction("grab");
    controller.addInstruction("delay 100");
    controller.addInstruction("angle 0 0");
    controller.addInstruction("angle 1 90");
    controller.addInstruction("delay 300");
    controller.addInstruction("release");
    controller.addInstruction("delay 100");
    controller.addInstruction("angle 0 0");
    controller.addInstruction("angle 1 0");
    controller.addInstruction("delay 100");
    controller.addInstruction("goto 0");
}

int main(int argc, char **argv) {
    bool running = true;
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
    al_install_mouse();
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_mouse_event_source());

    // Initialize the timers
    ALLEGRO_TIMER *update_timer = al_create_timer(1.0 / UPS);
    al_register_event_source(event_queue, al_get_timer_event_source(update_timer));
    al_start_timer(update_timer);

    // Initialize the display
    ALLEGRO_DISPLAY *display = nullptr;
    display = al_create_display(700, 700);
    al_register_event_source(event_queue, al_get_display_event_source(display));
    debug_font = al_load_ttf_font("./resources/clacon2.ttf", 14, 0);

    init();

    while (running) {
        ALLEGRO_EVENT event;
        // ALLEGRO_TIMEOUT timeout;
        // al_init_timeout(&timeout, 0.06);
        // bool get_event = al_wait_for_event_until(event_queue, &event, &timeout);
        al_wait_for_event(event_queue, &event);

        switch (event.type) {
            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                running = false;
                break;
            case ALLEGRO_EVENT_TIMER:
                tick++;
                update();
                redraw();
                break;
            case ALLEGRO_EVENT_KEY_DOWN:
                onKeyDown(event.keyboard.keycode);
                break;
            default:
                // fprintf(stderr, "Unsupported event received: %d\n", event.type);
                break;
        }
    }

    al_destroy_display(display);
    al_destroy_event_queue(event_queue);

    return 0;
}