#include "Shell.h"

// void Shell::main()
// {
//     return;
// }

void Shell::Loop()
{
    switch(current_page)
    {
        case 0:
            ApplicationLauncher();
            break;
    }
}

void Shell::AddCommonBarInUI(UI* ui)
{
    ui->AddUIElement(UIElement("Application Launcher", Color(0x00FFAA), [&]() -> void {if(current_page != 0) {current_page = 0; ui->Exit();}}), Point(0, 7));
    ui->AddUIElement(UIElement("System Setting", Color(0xFFFFFF), [&]() -> void {MatrixOS::SYS::OpenSetting();}), Point(7, 7));
}
    
namespace MatrixOS::SYS{void ExecuteAPP(uint32_t app_id); uint16_t GetApplicationCount();} //Use non exposed Matrix OS API

void Shell::ApplicationLauncher()
{
    UI applicationLauncher("Application Launcher", Color(0x00FFAA));

    AddCommonBarInUI(&applicationLauncher);

    uint16_t app_count = MatrixOS::SYS::GetApplicationCount();
    MatrixOS::Logging::LogDebug("Shell", "%d apps detected", app_count);

    uint16_t rendered_app_count = 0;
    for(uint8_t i = 0; i < app_count; i++) //I don't like the for loop but tbh there's nothing wrong with it.
    { 
        Application_Info* application = applications[i];
        if(application->visibility)
        {
            uint8_t x = rendered_app_count % 8; 
            uint8_t y = rendered_app_count / 8;

            string app_author = application->author;
            string app_name = application->name;
            Color app_color = application->color;

            applicationLauncher.AddUIElement(UIElement(app_name, 
                                    app_color, 
                                    [&, app_author, app_name]() -> void {MatrixOS::SYS::ExecuteAPP(application->id);}), 
                                    Point(x, y));
            MatrixOS::Logging::LogDebug("Shell", "App #%d %s-%s loaded.", rendered_app_count, application->author.c_str(), application->name.c_str());
            rendered_app_count ++;
        }
        else
        {
            MatrixOS::Logging::LogDebug("Shell", "%s not visible, skip.", application->name.c_str());
        }
    }
    applicationLauncher.Start();
}