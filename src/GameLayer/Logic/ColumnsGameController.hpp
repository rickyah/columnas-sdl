//
//  ColumnsGameController.hpp
//  Columns
//
//  Created by Ricardo Amores Hernández on 3/6/16.
//
//

#ifndef ColumnsGameController_hpp
#define ColumnsGameController_hpp

#include "ColumnsBoard.hpp"
#include "ColumnsBoardView.hpp"
#include "Renderer.hpp"

class ColumnsGameController
{
    
public:
    ColumnsGameController(std::shared_ptr<Renderer> renderer):mRenderer(renderer) {}
    
    // TODO: use with a configuration POCO when we read configuration from files
    void Init();
    
private:
    ColumnsGameController(const ColumnsGameController &);
    ColumnsGameController& operator=(const ColumnsGameController &);
    
    const std::shared_ptr<Renderer> mRenderer;
};

#endif /* ColumnsGameController_hpp */
