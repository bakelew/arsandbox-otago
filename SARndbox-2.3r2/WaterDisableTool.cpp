#include "WaterDisableTool.h"
#include "Sandbox.h"
#include "SurfaceRenderer.h"

WaterDisableToolFactory* WaterDisableTool::factory=0;
WaterDisableToolFactory* WaterDisableTool::initClass(Vrui::ToolManager& toolManager)
{    
    /* Create the tool factory: */
    factory=new WaterDisableToolFactory("WaterDisableTool","Toggle the water renderer",0,toolManager);
	
    /* Set up the tool class' input layout: */
    factory->setNumButtons(1);
    factory->setButtonFunction(0,"Toggle water");
	
    /* Register and return the class: */
    toolManager.addClass(factory,Vrui::ToolManager::defaultToolFactoryDestructor);
    return factory;
}
WaterDisableTool::WaterDisableTool(const Vrui::ToolFactory* factory,const Vrui::ToolInputAssignment& inputAssignment)
    :Vrui::Tool(factory,inputAssignment),buttonZeroActive(false),waterOn(true),wtBkp(0)
{
}

WaterDisableTool::~WaterDisableTool(void)
{
}

const Vrui::ToolFactory* WaterDisableTool::getFactory(void) const
{
    return factory;
}

void WaterDisableTool::buttonCallback(int buttonSlotIndex,Vrui::InputDevice::ButtonCallbackData* cbData)
{
    // button state toggles
    if (buttonSlotIndex == 0)
        {
            buttonZeroActive = !buttonZeroActive;
            if (buttonZeroActive)
                toggleWater();
        }
}

void WaterDisableTool::toggleWater()
{
    waterOn = !waterOn;
    if (waterOn)
        {
            for(std::vector<Sandbox::RenderSettings>::iterator rsIt=application->renderSettings.begin();rsIt!=application->renderSettings.end();++rsIt) {
                if (wtBkp!=0)
                    rsIt->surfaceRenderer->setWaterTable(wtBkp);
            }
        }
    else
        {
            for(std::vector<Sandbox::RenderSettings>::iterator rsIt=application->renderSettings.begin();rsIt!=application->renderSettings.end();++rsIt) {
                wtBkp = application->waterTable;
                rsIt->surfaceRenderer->setWaterTable(0);
            }
        }
    
}


