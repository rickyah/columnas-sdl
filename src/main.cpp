#include <string>
#include "App.hpp"
#include "DataTypes.hpp"
#include "ColumnsGameController.hpp"


int main(int argc, char *argv[])
{
    App app;

    ColumnsGameController ctrl(app.eventQueue(), app.resourceManager());

    ctrl.Init();

    app.eventQueue()->AddListener(AppTouch_Event::sEventType, [](std::shared_ptr<IEventData> event) {
        auto ptr = std::static_pointer_cast<AppTouch_Event>(event);
        
        if (ptr->args().type == AppTouch_Event::ETouchType::Motion)
        {
            SDL_Log("motion (%f,%f)", ptr->args().motion.dx, ptr->args().motion.dy);
        }
        else
        {
            
            SDL_Log("pos (%d,%d)", ptr->args().pos.x, ptr->args().pos.y);
        }
    });
    
    app.SetLogicUpdateFunction(std::bind(&ColumnsGameController::Update, &ctrl, std::placeholders::_1));
    
    app.SetRenderUpdateFunction(std::bind(&ColumnsGameController::Render, &ctrl, std::placeholders::_1, std::placeholders::_2));
    
    app.UpdateGameLoop();

    return 0;
}
