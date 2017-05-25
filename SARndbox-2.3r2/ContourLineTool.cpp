#include "ContourLineTool.h"
#include "Sandbox.h"
#include "SurfaceRenderer.h"

ContourLineToolFactory* ContourLineTool::factory=0;
ContourLineToolFactory* ContourLineTool::initClass(Vrui::ToolManager& toolManager)
{    
    /* Create the tool factory: */
    factory=new ContourLineToolFactory("ContourLineTool","Adjust contour interval",0,toolManager);
	
    /* Set up the tool class' input layout: */
    factory->setNumButtons(2);
    factory->setButtonFunction(0,"Decrease interval");
    factory->setButtonFunction(1,"Increase interval");
	
    /* Register and return the class: */
    toolManager.addClass(factory,Vrui::ToolManager::defaultToolFactoryDestructor);
    return factory;
}
ContourLineTool::ContourLineTool(const Vrui::ToolFactory* factory,const Vrui::ToolInputAssignment& inputAssignment)
    :Vrui::Tool(factory,inputAssignment),buttonZeroActive(false),buttonOneActive(false),contourLineOffset(0)
{
}

ContourLineTool::~ContourLineTool(void)
{
}

const Vrui::ToolFactory* ContourLineTool::getFactory(void) const
{
    return factory;
}

void ContourLineTool::buttonCallback(int buttonSlotIndex,Vrui::InputDevice::ButtonCallbackData* cbData)
{
    // button state toggles
    if (buttonSlotIndex == 0)
        {
            buttonZeroActive = !buttonZeroActive;
            if (buttonZeroActive)
                adjustContourLines(0);
        }
    
    if (buttonSlotIndex == 1)
        {
            buttonOneActive = !buttonOneActive;
            if (buttonOneActive)
                adjustContourLines(1);
        }
}

void ContourLineTool::adjustContourLines(int buttonSlotIndex)
{
    double offsetDiff = 0;
    switch(buttonSlotIndex)
        {
        case 0:
            offsetDiff = -CONTOUR_ADJUST_SPEED;
            break;
        case 1:
            offsetDiff = CONTOUR_ADJUST_SPEED;
        default:
            break;
        }
    
    for(std::vector<Sandbox::RenderSettings>::iterator rsIt=application->renderSettings.begin();rsIt!=application->renderSettings.end();++rsIt) {
        
        // change contourLineSpacing in the render settings
        rsIt->contourLineSpacing = Misc::max(0.0,rsIt->contourLineSpacing+offsetDiff);

        // refresh the surfaceRenderer
        rsIt->surfaceRenderer->setContourLineDistance(rsIt->contourLineSpacing);
    }
    
}


