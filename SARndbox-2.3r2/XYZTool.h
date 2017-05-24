#ifndef XYZTOOL_INCLUDED
#define XYZTOOL_INCLUDED

#define XYZ_OUTPUT_DIR "/home/sandbox/Desktop"

#include <Vrui/Tool.h>
#include <Vrui/GenericToolFactory.h>
#include <Vrui/Application.h>

/* Forward declarations: */
class Sandbox;
class XYZTool;

typedef Vrui::GenericToolFactory<XYZTool> XYZToolFactory;
typedef float FilteredDepth;

class XYZTool:public Vrui::Tool,public Vrui::Application::Tool<Sandbox>
{
    friend class Vrui::GenericToolFactory<XYZTool>;
 private:
    static XYZToolFactory* factory;
    bool buttonDown;
    
    /* Constructors and destructors: */
 public:
    static XYZToolFactory* initClass(Vrui::ToolManager& ToolManager);
    XYZTool(const Vrui::ToolFactory* factory,const Vrui::ToolInputAssignment& inputAssignment);
    virtual ~XYZTool(void);
	
    /* Methods from class Vrui::Tool: */
    virtual const Vrui::ToolFactory* getFactory(void) const;
    virtual void buttonCallback(int buttonSlotIndex,Vrui::InputDevice::ButtonCallbackData* cbData);
    virtual void frame(void);
    
};

#endif
