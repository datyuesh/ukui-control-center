#include "language.h"
#include "ui_language.h"

Language::Language()
{
    ui = new Ui::Language;
    pluginWidget = new CustomWidget;
    pluginWidget->setAttribute(Qt::WA_DeleteOnClose);
    ui->setupUi(pluginWidget);

    pluginName = tr("language");
    pluginType = TIME_LANGUAGE;
}

Language::~Language()
{
    delete ui;
}

QString Language::get_plugin_name(){
    return pluginName;
}

int Language::get_plugin_type(){
    return pluginType;
}

CustomWidget *Language::get_plugin_ui(){
    return pluginWidget;
}

void Language::plugin_delay_control(){

}