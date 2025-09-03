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
#include "logic/CustomMachinery.hpp"
#include "graphics/GraphicsEngine.hpp"
#include "logic/MachineryBuilder.hpp"

long long tick = 0;
long long eventCounter = 0;

void init()
{
    GuiEngine::instance()->init();

    CameraParameters parameters;
    parameters.fov = 90;
    parameters.displayDimensions = Vector2d(900, 900);
    parameters.position = Vector2d(0, 0);
    parameters.z = -40; // fov = 90 deg

    GraphicsEngine::instance()->setCameraParameters(parameters);
    MachineryBuilder::instance()->createWindow();

    BasicModule* rootModule = new BasicModule(2);
    rootModule->addNode(Vector2d(Rotation(0), 16), Rotation(0));
    rootModule->addNode(Vector2d(Rotation(M_PI), 16), Rotation(M_PI));
    rootModule->addBitmap(GraphicsEngine::instance()->corridorModuleLayer0, Vector2d(160, 160), CommonValues::zModuleMainBackgroung);
    rootModule->addBitmap(GraphicsEngine::instance()->corridorModuleLayer1, Vector2d(160, 160), CommonValues::zModuleWalls);
    rootModule->setTransforms(Vector2d(0, 0), Rotation(0));
    rootModule->addToGameWorld();

    auto controller7 = new Controller(Vector2d(-10, 3));
    controller7->addInstruction("delay 50");
    controller7->addInstruction("delay 100");
    controller7->addInstruction("send 3 0 180");
    controller7->addInstruction("send 3 1 270");
    controller7->addInstruction("delay 400");
    controller7->addInstruction("send 3 200 0");
    controller7->addInstruction("delay 10");
    controller7->addInstruction("send 3 0 190"); // move box 1 to furnace
    controller7->addInstruction("send 3 1 100");
    controller7->addInstruction("delay 400");
    controller7->addInstruction("send 3 100 0");
    controller7->addInstruction("delay 10");
    controller7->addInstruction("goto 2");
    controller7->addToGameWorld();

}

void redraw()
{
    // auto start = std::chrono::system_clock::now();
    GameWorld::instance()->drawAll();
    MachineryBuilder::instance()->drawGhost();
    GraphicsEngine::instance()->drawDebugBackgroung2();
    al_set_target_bitmap(al_get_backbuffer(al_get_current_display()));
    al_clear_to_color(al_map_rgb(0, 0, 5));

    al_hold_bitmap_drawing(true);
    GraphicsEngine::instance()->draw();
    al_hold_bitmap_drawing(false);

    // gameWorld.getController(6)->drawInstructions();

    al_hold_bitmap_drawing(true);
    GuiEngine::instance()->draw();
    al_hold_bitmap_drawing(false);

    al_hold_bitmap_drawing(true);

    al_draw_text(GraphicsEngine::instance()->debugFont, al_map_rgb(255, 255, 255), 10, 10, 0, "Programing game");
    al_draw_text(GraphicsEngine::instance()->debugFont, al_map_rgb(255, 255, 255), 10, 20, 0, "Current tick:");
    al_draw_text(GraphicsEngine::instance()->debugFont, al_map_rgb(255, 255, 255), 110, 20, 0, std::to_string(tick).c_str());
    al_draw_text(GraphicsEngine::instance()->debugFont, al_map_rgb(255, 255, 255), 150, 20, 0, "Events counter:");
    al_draw_text(GraphicsEngine::instance()->debugFont, al_map_rgb(255, 255, 255), 270, 20, 0, std::to_string(eventCounter).c_str());

    al_hold_bitmap_drawing(false);
    GraphicsEngine::instance()->clearBitmaps();
    al_flip_display();

    // auto end = std::chrono::system_clock::now();
    // auto elapsed_ms = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    // std::cout << elapsed_ms << std::endl;
}

void updateMouse() {
    ALLEGRO_MOUSE_STATE mouseState;
    al_get_mouse_state(&mouseState);
    MachineryBuilder::instance()->mousePos(GraphicsEngine::instance()->transformPointInverse(Vector2d(mouseState.x, mouseState.y)));
}

void update()
{
    tick++;
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
    if (al_key_down(&keyboardState, ALLEGRO_KEY_EQUALS))
    {
        camera.z /= 1.01;
        camera.fov /= 1.01;
    }
    if (al_key_down(&keyboardState, ALLEGRO_KEY_MINUS))
    {
        camera.z *= 1.01;
        camera.fov *= 1.01;
    }
    GraphicsEngine::instance()->setCameraParameters(camera);
    updateMouse();
    GameWorld::instance()->run();
}

void onKeyDown(int keycode)
{
    switch (keycode)
    {
    case ALLEGRO_KEY_R:
        GraphicsEngine::instance()->loadImages();
        break;
    case ALLEGRO_KEY_ESCAPE:
        MachineryBuilder::instance()->clearItem();
        break;
    default:
        break;
    }
}

void onNodeClick(ModuleNode* node) {
    ModuleBuilder::instance()->setParentNode(node);
    ModuleBuilder::instance()->createWindow();
}

void onMouseClick(double x, double y) {
    std::cout << "mouse click" << std::endl;
    if (GuiEngine::instance()->click(Vector2d(x, y))) { // check if clicking on windows, buttons, etc
        return;
    }

    for (auto module: GameWorld::instance()->getModules()) {
        std::vector<ModuleNode*> nodes = module->getNodes();
        for (auto node: nodes) {
            if ((GraphicsEngine::instance()->transformPoint(module->getPosition() + node->position.rotate(module->getRotation()), 0) - Vector2d(x, y)).lenght() < 20) {
                onNodeClick(node);
                return;
            }
        }
    }

    Vector2d gameWorldClickPos = GraphicsEngine::instance()->transformPointInverse(Vector2d(x, y));
    GameWorld::instance()->click(gameWorldClickPos);
    MachineryBuilder::instance()->onClick();
}

void mainLoop(ALLEGRO_EVENT_QUEUE *event_queue)
{
    bool running = true;
    while (running)
    {
        ALLEGRO_EVENT event;
        // ALLEGRO_TIMEOUT timeout;
        // al_init_timeout(&timeout, 0.06);
        // bool get_event = al_wait_for_event_until(event_queue, &event, &timeout);
        al_wait_for_event(event_queue, &event);
        eventCounter++;
        switch (event.type)
        {
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
            onMouseClick(event.mouse.x, event.mouse.y);
            
            break;
        case ALLEGRO_EVENT_DISPLAY_RESIZE:
            al_acknowledge_resize(event.display.source);
            break;
        case ALLEGRO_EVENT_MOUSE_BUTTON_UP:
            GuiEngine::instance()->releaseMouse(Vector2d(event.mouse.x, event.mouse.y));
            break;
        case ALLEGRO_EVENT_MOUSE_AXES:
        case ALLEGRO_EVENT_MOUSE_ENTER_DISPLAY:
        case ALLEGRO_EVENT_MOUSE_LEAVE_DISPLAY:
            GuiEngine::instance()->moveMouse(Vector2d(event.mouse.x, event.mouse.y));
            break;

        default:
            // fprintf(stderr, "Unsupported event received: %d\n", event.type);
            break;
        }
    }
}

int main(int argc, char **argv)
{
    const int UPS = 60;

    // Initialize allegro
    if (!al_init())
    {
        fprintf(stderr, "Failed to initialize allegro.\n");
        return 1;
    }

    // Initialize allegro font addon
    if (!al_init_font_addon())
    {
        fprintf(stderr, "Failed to initialize allegro font addon.\n");
        return 1;
    }

    // Initialize allegro ttf addon
    if (!al_init_ttf_addon())
    {
        fprintf(stderr, "Failed to initialize allegro ttf addon.\n");
        return 1;
    }

    // Initialize allegro image addon
    if (!al_init_image_addon())
    {
        fprintf(stderr, "Failed to initialize allegro image addon.\n");
        return 1;
    }

    // Initialize allegro primitives addon
    if (!al_init_primitives_addon())
    {
        fprintf(stderr, "Failed to primitives allegro ttf addon.\n");
        return 1;
    }

    // Create the event queue
    ALLEGRO_EVENT_QUEUE *event_queue = nullptr;
    event_queue = al_create_event_queue();
    if (!event_queue)
    {
        fprintf(stderr, "Failed to create event queue.");
        return 1;
    }

    GraphicsEngine::instance()->debugFont = al_load_ttf_font("./resources/clacon2.ttf", 14, 0);
    GuiEngine::debugFont = al_load_ttf_font("./resources/clacon2.ttf", 14, 0);
    GraphicsEngine::instance()->loadImages();

    // Initialize user inputs
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
    GameWorld::instance()->run();

    mainLoop(event_queue);

    al_destroy_display(display);
    al_destroy_event_queue(event_queue);
    al_destroy_font(GraphicsEngine::instance()->debugFont);

    return 0;
}