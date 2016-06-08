#include <string>
#include "App.hpp"
#include "DataTypes.hpp"
#include "ColumnsGameController.hpp"

int main(int argc, char *argv[])
{
    App app;

    ColumnsGameController ctrl(app.eventQueue(), app.resourceManager());

    ctrl.Init();
    
    app.SetLogicUpdateFunction(std::bind(&ColumnsGameController::Update, &ctrl, std::placeholders::_1));
    
    app.SetRenderUpdateFunction(std::bind(&ColumnsGameController::Render, &ctrl, std::placeholders::_1, std::placeholders::_2));
    
    app.UpdateGameLoop();

    return 0;
}
