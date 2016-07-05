#include <string>
#include "App.hpp"
#include "DataTypes.hpp"
#include "ColumnsGameController.hpp"

int main(int argc, char *argv[])
{
    App app;

    ColumnsGameController ctrl(app.eventQueue(), app.resourceManager());

    ctrl.Init();
    
    app.SetLogicUpdateFunction([&ctrl](LogicFrameInfo time){
        ctrl.Update(time);
    });
    
    app.SetRenderUpdateFunction([&ctrl](RenderFrameInfo time, Renderer &renderer){
        ctrl.Render(time, renderer);
    });
    
    app.UpdateGameLoop();

    return 0;
}
