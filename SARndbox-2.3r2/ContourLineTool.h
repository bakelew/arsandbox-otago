#ifndef CONTOURLINETOOL_INCLUDED
#define CONTOURLINETOOL_INCLUDED

#define CONTOUR_ADJUST_SPEED 0.05

#include <Vrui/Tool.h>
#include <Vrui/GenericToolFactory.h>
#include <Vrui/Application.h>

/* Forward declarations: */
class Sandbox;
class ContourLineTool;
typedef Vrui::GenericToolFactory<ContourLineTool> ContourLineToolFactory;

class ContourLineTool:public Vrui::Tool,public Vrui::Application::Tool<Sandbox>
{
    friend class Vrui::GenericToolFactory<ContourLineTool>;
 private:
    static ContourLineToolFactory* factory;
    bool buttonZeroActive;
    bool buttonOneActive;
    double contourLineOffset;
    
    /* Constructors and destructors: */
 public:
    static ContourLineToolFactory* initClass(Vrui::ToolManager& ToolManager);
    ContourLineTool(const Vrui::ToolFactory* factory,const Vrui::ToolInputAssignment& inputAssignment);
    virtual ~ContourLineTool(void);
	
    /* Methods from class Vrui::Tool: */
    virtual const Vrui::ToolFactory* getFactory(void) const;
    virtual void buttonCallback(int buttonSlotIndex,Vrui::InputDevice::ButtonCallbackData* cbData);

    /* New methods: */
    void adjustContourLines(int buttonSlotIndex);
    
};
#endif
