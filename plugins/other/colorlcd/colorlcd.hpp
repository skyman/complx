#ifndef COLORLCD_HPP
#define COLORLCD_HPP

#include <lc3_all.hpp>
#include <wx/wx.h>
#include <wx/timer.h>
#include "colorlcdgui.h"

#define COLORLCD_MAJOR_VERSION 1
#define COLORLCD_MINOR_VERSION 5

wxDECLARE_EVENT(wxEVT_COMMAND_CREATE_DISPLAY, wxThreadEvent);
wxDECLARE_EVENT(wxEVT_COMMAND_DESTROY_DISPLAY, wxThreadEvent);

class ColorLCD : public COLORLCDGUI
{
public:
    ColorLCD(wxWindow* top, int width, int height, unsigned short startaddr, lc3_state* state);
    virtual void OnUpdate(wxTimerEvent& event);
    void OnPaint(wxPaintEvent& event) override;
private:
    wxTimer timer;
    lc3_state* state;
    int width;
    int height;
    unsigned short startaddr;
};

class ColorLCDPlugin : public wxEvtHandler, public Plugin
{
public:
    ColorLCDPlugin(unsigned short width, unsigned short height, unsigned short initaddr, unsigned short startaddr);
    ~ColorLCDPlugin();
    void OnWrite(lc3_state& state, unsigned short address, short value) override;
    void InitDisplay(wxThreadEvent& event);
    void UpdateDisplay(wxThreadEvent& event);
    void DestroyDisplay(wxThreadEvent& event);
    bool AvailableInLC3Test() const override {return false;}
private:
    unsigned short width;
    unsigned short height;
    unsigned short initaddr;
    unsigned short startaddr;
    ColorLCD* lcd;
    bool lcd_initializing;

};

extern "C" Plugin* create_plugin(const std::map<std::string, std::string>& params);
extern "C" void destroy_plugin(Plugin* ptr);

#endif

