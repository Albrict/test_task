#pragma once
#include <FL/Fl_Group.H>
#include <optional>
#include "def.hpp"
#include "parse.hpp"
#include "sort.hpp"

class Fl_File_Chooser;
class Fl_Choice;

namespace OP {
    class ObjectTable;
    class ObjectViewer final : public Fl_Group {
    public:
        explicit ObjectViewer(Fl_Window &window, std::optional<file_buffer> buffer = {});
    private:
        void SortObjects();
        void SetChoiceToGroupsValues();
        static void SaveCurrentGroup(Fl_Widget *widget, void *data);
        static void OpenFileCallback(Fl_Widget *widget, void *data);
        static void ViewByDistance(Fl_Widget *widget, void *data);
        static void ViewByType(Fl_Widget *widget, void *data);
        static void ViewByName(Fl_Widget *widget, void *data);
        static void ViewByDate(Fl_Widget *widget, void *data);
        static void ChoiceCallback(Fl_Widget *widget, void *data);
    private:
        enum class CurrentSortType {
            MIXED,
            NAME,
            TYPE,
            DATE,
            DISTANCE
        };

        std::vector<Object> m_objects      = {};
        sorted_groups       m_by_name      = {};
        sorted_groups       m_by_date      = {};
        sorted_groups       m_by_type      = {};
        sorted_groups       m_by_distance  = {};
        ObjectTable        *m_table        = {};
        Fl_File_Chooser    *m_file_chooser = {};
        Fl_Choice          *m_group_choice = {};
        CurrentSortType     m_type         = {};
    };
}
