#pragma once
#include "parse.hpp"
#include <FL/Fl_Table.H>
#include <array>
#include <string>

namespace OP {
    class ObjectTable final : public Fl_Table {
    public:
        explicit ObjectTable(const std::vector<Object> &objects, const int x, const int y, 
                             const int width, const int height, const char *label = nullptr);
        void UpdateData(const std::vector<Object> &objects);
    private:
        enum class ColType {
            NAME,
            POSITION,
            TYPE,
            DATE
        };

        void DefineDataToDraw(char *s, const int row, const ColType col);
        void DrawHeader(const char *s, const int x, const int y, const int width, 
                        const int height);
        void DrawData(const char *s, const int x, const int y, const int width, 
                      const int height);
        
        void draw_cell(const TableContext context, const int row = 0, const int col = 0, 
                       const int x = 0, const int y = 0, const int width = 0, const int height = 0);
    private:
        std::array<std::string, 4> m_labels = {
            "Name",
            "Position",
            "Type",
            "Date of creation"
        };
        std::vector<Object> m_objects = {};
    };
}
