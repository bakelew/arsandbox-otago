#include "WaterLevelTool.h"
#include "Sandbox.h"
#include "ElevationColorMap.h"
#include "SurfaceRenderer.h"

WaterLevelToolFactory* WaterLevelTool::factory=0;
WaterLevelToolFactory* WaterLevelTool::initClass(Vrui::ToolManager& toolManager)
{    
    /* Create the tool factory: */
    factory=new WaterLevelToolFactory("WaterLevelTool","Raise or Lower Water Level",0,toolManager);
	
    /* Set up the tool class' input layout: */
    factory->setNumButtons(2);
    factory->setButtonFunction(0,"Raise water");
    factory->setButtonFunction(1,"Lower water");
	
    /* Register and return the class: */
    toolManager.addClass(factory,Vrui::ToolManager::defaultToolFactoryDestructor);
    return factory;
}
WaterLevelTool::WaterLevelTool(const Vrui::ToolFactory* factory,const Vrui::ToolInputAssignment& inputAssignment)
    :Vrui::Tool(factory,inputAssignment),buttonZeroActive(false),buttonOneActive(false),waterLevelOffset(0)
{
}

WaterLevelTool::~WaterLevelTool(void)
{
}

const Vrui::ToolFactory* WaterLevelTool::getFactory(void) const
{
    return factory;
}

void WaterLevelTool::buttonCallback(int buttonSlotIndex,Vrui::InputDevice::ButtonCallbackData* cbData)
{
    // button state toggles
    if (buttonSlotIndex == 0)
        buttonZeroActive = !buttonZeroActive;
    
    if (buttonSlotIndex == 1)
        buttonOneActive = !buttonOneActive;
}
void WaterLevelTool::frame(void)
{
    // check toggle at every frame (allows to hold the key down etc.)
    if (buttonZeroActive)
        adjustWaterLevel(0);
    
    if (buttonOneActive)
        adjustWaterLevel(1);
            
}
void WaterLevelTool::adjustWaterLevel(int buttonSlotIndex)
{
    double offsetDiff = 0;
    switch(buttonSlotIndex)
        {
        case 0:
            offsetDiff = WATER_ADJUST_SPEED;
            break;
        case 1:
            offsetDiff = -WATER_ADJUST_SPEED;
        default:
            break;
        }
    for(std::vector<Sandbox::RenderSettings>::iterator rsIt=application->renderSettings.begin();rsIt!=application->renderSettings.end();++rsIt) {
        
        // Calculate the texture mapping plane for this renderer's height map:
        if(rsIt->elevationColorMap!=0)
            {
                rsIt->elevationColorMap->texturePlaneEq[3] += offsetDiff;
            }
    }
}


