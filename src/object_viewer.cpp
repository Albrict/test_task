#include <FL/Fl_Choice.H>
#include <FL/Fl_File_Chooser.H>
#include <FL/Fl_Tabs.H>
#include <array>
#include <fstream>
#include <iomanip>
#include <string_view>

#include "object_viewer.hpp"
#include "file.hpp"
#include "object_table.hpp"
#include "sort.hpp"

OP::ObjectViewer::ObjectViewer(Fl_Window &window, std::optional<file_buffer> file)
    : Fl_Group(0, 0, window.w(), window.h())
{
    Fl_Group::begin();
    {
        std::array<Fl_Button*, 6>  buttons = {}; 
        std::array<std::string_view, 6> labels  = {
            "View objects by distance",
            "View objects by name",
            "View objects by date",
            "View objects by type",
            "Open file",
            "Save current group"
        }; 
        std::array<Fl_Callback*, 6> callbacks = {
            ViewByDistance,
            ViewByName,
            ViewByDate,
            ViewByType,
            OpenFileCallback,
            SaveCurrentGroup
        };

        const int button_width  = 180;
        const int button_height = 40;
        const int button_x      = window.w() - button_width;
        int       button_y      = 60;
        
        for (std::size_t i = 0; i < buttons.size(); ++i) {
            buttons[i] = new Fl_Button(button_x, button_y, button_width, button_height, labels[i].data());
            buttons[i]->callback(callbacks[i], this);
            button_y  += button_height + 10;
        }
        m_group_choice = new Fl_Choice(button_x, 10, button_width, button_height);
        m_group_choice->callback(ChoiceCallback, this);

        if (file.has_value()) {
            auto objects = ParseFile(file.value());         
            m_objects    = objects;
            SortObjects();
            m_table      = new ObjectTable(objects, 10, 20, button_x - 10, window.h() - 20);            
        }
    }
    Fl_Group::end();
}

void OP::ObjectViewer::OpenFileCallback(Fl_Widget *widget, void *data)
{
    ObjectViewer *viewer = static_cast<ObjectViewer*>(data);    
    if (viewer->m_file_chooser == nullptr)
        viewer->m_file_chooser = new Fl_File_Chooser("..", "Input files (*txt)", FL_SINGLE, "Choose file to proccess");
    viewer->m_file_chooser->show();

    while(viewer->m_file_chooser->shown())
        Fl::wait(); 
    const char *file = viewer->m_file_chooser->value();
    if (file == nullptr) {
        delete viewer->m_file_chooser;
        viewer->m_file_chooser = nullptr;
        return;
    }
    std::optional<file_buffer> buffer = ReadFileAndValidate(file);
    delete viewer->m_file_chooser;
    viewer->m_file_chooser = nullptr;

    if (buffer.has_value()) {
        viewer->m_objects = ParseFile(buffer.value());         
        viewer->SortObjects();
        if (viewer->m_table == nullptr) {
            viewer->m_table = new ObjectTable(viewer->m_objects, 10, 20, widget->x() - 10, widget->parent()->h() - 20);
            widget->parent()->add(viewer->m_table);
            Fl::redraw();
        } else {
            viewer->m_table->UpdateData(viewer->m_objects);
            viewer->m_group_choice->clear();
            viewer->m_group_choice->redraw();
            viewer->m_type = CurrentSortType::MIXED;
        }
    } else {
        fl_alert("File format is invalid!");
    }
}

void OP::ObjectViewer::ViewByType(Fl_Widget *widget, void *data)
{
    ObjectViewer *viewer = static_cast<ObjectViewer*>(data);
    viewer->m_group_choice->clear();
    viewer->m_group_choice->redraw();
    if (viewer->m_table == nullptr) {
        fl_alert("Choose file first!");
        return;
    }
    viewer->m_type = CurrentSortType::TYPE;
    viewer->SetChoiceToGroupsValues();
}


void OP::ObjectViewer::ViewByName(Fl_Widget *widget, void *data)
{
    ObjectViewer *viewer = static_cast<ObjectViewer*>(data);
    viewer->m_group_choice->clear();
    viewer->m_group_choice->redraw();
    if (viewer->m_table == nullptr) {
        fl_alert("Choose file first!");
        return;
    }
    viewer->m_type = CurrentSortType::NAME;
    viewer->SetChoiceToGroupsValues();
}

void OP::ObjectViewer::ViewByDate(Fl_Widget *widget, void *data)
{
    ObjectViewer *viewer = static_cast<ObjectViewer*>(data);
    viewer->m_group_choice->clear();
    viewer->m_group_choice->redraw();
    if (viewer->m_table == nullptr) {
        fl_alert("Choose file first!");
        return;
    }
    viewer->m_type = CurrentSortType::DATE;
    viewer->SetChoiceToGroupsValues();
}

void OP::ObjectViewer::ViewByDistance(Fl_Widget *widget, void *data)
{
    ObjectViewer *viewer = static_cast<ObjectViewer*>(data);
    viewer->m_group_choice->clear();
    viewer->m_group_choice->redraw();
    if (viewer->m_table == nullptr) {
        fl_alert("Choose file first!");
        return;
    }
    viewer->m_type = CurrentSortType::DISTANCE;
    viewer->SetChoiceToGroupsValues();
}

void OP::ObjectViewer::SortObjects()
{
    if (m_objects.size() > 0) {
        m_by_name     = SortByName(m_objects);        
        m_by_date     = SortByDate(m_objects);
        m_by_distance = SortByDistance(m_objects);
        m_by_type     = SortByType(m_objects);
    }  
}

void OP::ObjectViewer::SaveCurrentGroup(Fl_Widget *widget, void *data)
{
    ObjectViewer *viewer        = static_cast<ObjectViewer*>(data);
    std::vector<Object> objects = {};
    
    switch(viewer->m_type) {
    case CurrentSortType::TYPE:
        objects = viewer->m_by_type[viewer->m_group_choice->text()];
        break;
    case CurrentSortType::NAME:
        objects = viewer->m_by_name[viewer->m_group_choice->text()];
        break;
    case CurrentSortType::DATE:
        objects = viewer->m_by_date[viewer->m_group_choice->text()];
        break;
    case CurrentSortType::DISTANCE:
        objects = viewer->m_by_distance[viewer->m_group_choice->text()];
        break;
    case CurrentSortType::MIXED:
        objects = viewer->m_objects;
        break;
    default:
        break;
    }
    char *filename = {};
    filename = fl_file_chooser("Save File As?", "*", filename);
    if (filename != nullptr) {
        std::ofstream file(filename);
        file.imbue(std::locale("C"));
        for (const auto &object : objects) {
            std::tm time      = object.date.tm;
            double  timestamp = static_cast<double>(std::mktime(&time));
            file << std::fixed << std::setprecision(5);

            file << object.name << ' ';
            file << object.position.x << ' ' << object.position.y << ' ';
            file << object.type << ' ';
            file << timestamp << '\n'; 
        }
        file.close();
    }
}

void OP::ObjectViewer::ChoiceCallback(Fl_Widget *widget, void *data)
{
    ObjectViewer      *viewer   = static_cast<ObjectViewer*>(data);
    const int          index    = viewer->m_group_choice->value();
    const Fl_Menu_Item item     = viewer->m_group_choice->menu()[index];
    const char        *value    = item.label(); 
    std::vector<Object> objects = {}; 

    switch(viewer->m_type) {
    case CurrentSortType::TYPE:
        objects = viewer->m_by_type[value];
        break;
    case CurrentSortType::NAME:
        objects = viewer->m_by_name[value];
        break;
    case CurrentSortType::DATE:
        objects = viewer->m_by_date[value];
        break;
    case CurrentSortType::DISTANCE:
        objects = viewer->m_by_distance[value];
        break;
    case CurrentSortType::MIXED:
        break;
    default:
        break;
    } 
    viewer->m_table->UpdateData(objects);
}

void OP::ObjectViewer::SetChoiceToGroupsValues()
{
    switch(m_type) {
    case CurrentSortType::TYPE:
        for (const auto &[key, group] : m_by_type)
            m_group_choice->add(key.c_str());
        break;
    case CurrentSortType::NAME:
        for (const auto &[key, group] : m_by_name)
            m_group_choice->add(key.c_str());
        break;
    case CurrentSortType::DATE:
        for (const auto &[key, group] : m_by_date)
            m_group_choice->add(key.c_str());
        break;
    case CurrentSortType::DISTANCE:
        for (const auto &[key, group] : m_by_distance)
            m_group_choice->add(key.c_str());
        break;
    case CurrentSortType::MIXED:
        break;
    default:
        break;
    }
    m_group_choice->value(0);
    m_group_choice->do_callback();
}
