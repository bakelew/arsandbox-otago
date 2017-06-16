#include "HeightColorMapTool.h"
#include "Sandbox.h"
#include "ElevationColorMap.h"
#include "SurfaceRenderer.h"
#include "Config.h"

#include <dirent.h>
#include <vector>

/* https://stackoverflow.com/questions/874134/find-if-string-ends-with-another-string-in-c */
bool hasEnding(std::string const &fullString, std::string const &ending) {
    if (fullString.length() >= ending.length()) {
        return (0 == fullString.compare (fullString.length() - ending.length(), ending.length(), ending));
    } else {
        return false;
    }
}

HeightColorMapToolFactory* HeightColorMapTool::factory=0;
HeightColorMapToolFactory* HeightColorMapTool::initClass(Vrui::ToolManager& toolManager)
{    
    /* Create the tool factory: */
    factory=new HeightColorMapToolFactory("HeightColorMapTool","Switch Color Map",0,toolManager);
	
    /* Set up the tool class' input layout: */
    factory->setNumButtons(1);
    factory->setButtonFunction(0,"Scroll");
	
    /* Register and return the class: */
    toolManager.addClass(factory,Vrui::ToolManager::defaultToolFactoryDestructor);
    
    return factory;
}
HeightColorMapTool::HeightColorMapTool(const Vrui::ToolFactory* factory,const Vrui::ToolInputAssignment& inputAssignment)
    :Vrui::Tool(factory,inputAssignment),mapIndex(0),buttonOneActive(false)
{
}

HeightColorMapTool::~HeightColorMapTool(void)
{
}

const Vrui::ToolFactory* HeightColorMapTool::getFactory(void) const
{
    return factory;
}

void HeightColorMapTool::buttonCallback(int buttonSlotIndex,Vrui::InputDevice::ButtonCallbackData* cbData)
{
    buttonOneActive=!buttonOneActive;
    if (buttonOneActive)
        scrollHeightColorMaps(buttonSlotIndex);
}

void HeightColorMapTool::scrollHeightColorMaps(int buttonSlotIndex)
{
    std::vector<std::string> mapNames;
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir(CONFIG_CONFIGDIR)) != NULL)
        {
            while ((ent = readdir(dir)) != NULL)
                {
                    std::string fname(ent->d_name);
                    if (hasEnding(fname, ".cpt"))
                        {
                            mapNames.push_back(fname);
                        }
                }
        }

    if (mapIndex >= mapNames.size()) mapIndex %= mapNames.size();
    
    std::string selectedMapName=mapNames[mapIndex];
    for(std::vector<Sandbox::RenderSettings>::iterator rsIt=application->renderSettings.begin();rsIt!=application->renderSettings.end();++rsIt)
        {
            if(rsIt->elevationColorMap!=0)
                {
                    rsIt->loadHeightMap(selectedMapName.c_str());
                    rsIt->elevationColorMap->calcTexturePlane(application->depthImageRenderer);
                    rsIt->surfaceRenderer->setElevationColorMap(rsIt->elevationColorMap);
            }
        }
    
    ++mapIndex;
}


