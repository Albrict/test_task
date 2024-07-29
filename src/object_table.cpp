#include "object_table.hpp"
#include <FL/fl_draw.H>

OP::ObjectTable::ObjectTable(const std::vector<Object> &objects, const int x, 
                             const int y, const int width, const int height, 
                             const char *label)
    : Fl_Table(x, y, width, height, label),
    m_objects(objects) 
{
    // Rows
    rows(static_cast<int>(m_objects.size()));  // how many rows
    row_header(1);                             // enable row headers (along left)
    row_height_all(20);                        // default height of rows
    row_resize(0);                             // disable row resizing
                                               //
    // Cols
    cols(static_cast<int>(m_labels.size()));   // how many columns
    col_header(1);                             // enable column headers (along top)
    col_width_all(125);                         // default width of columns
    col_resize(1);                             // enable column resizing
    end();			                           // end the Fl_Table group
}

void OP::ObjectTable::DrawHeader(const char *s, const int x, const int y, const int width, 
            const int height)
{
    fl_push_clip(x, y, width, height);
      fl_draw_box(FL_THIN_UP_BOX, x, y, width, height, row_header_color());
      fl_color(FL_BLACK);
      fl_draw(s, x, y, width, height, FL_ALIGN_CENTER);
    fl_pop_clip();
}

void OP::ObjectTable::DrawData(const char *s, const int x, const int y, const int width, 
          const int height)
{
    fl_push_clip(x, y, width , height);
      // Draw cell bg
      fl_color(FL_WHITE); fl_rectf(x, y, width, height);
      // Draw cell data
      fl_color(FL_GRAY0); fl_draw(s, x, y, width, height, FL_ALIGN_CENTER);
      // Draw box border
      fl_color(color()); fl_rect(x, y, width, height);
    fl_pop_clip();
}

void OP::ObjectTable::DefineDataToDraw(char *s, const int row, const ColType col)
{
    const Object &object = m_objects[static_cast<const std::size_t>(row)];
    switch(col) {
    case ColType::NAME:
        sprintf(s, "%s", object.name.c_str());
        break;
    case ColType::POSITION:
        sprintf(s, "%.2lf, %.2lf", object.position.x, object.position.y);
        break;
    case ColType::TYPE:
        sprintf(s, "%s", object.type.c_str());
        break;
    case ColType::DATE:
        sprintf(s, "%d/%d/%d", 1900 + object.date.tm.tm_year, object.date.tm.tm_mon, object.date.tm.tm_mday);
        break;
    default:
        break;
    }
}

void OP::ObjectTable::UpdateData(const std::vector<Object> &objects)
{
    m_objects = objects;
    begin();
    // Rows
    rows(static_cast<int>(m_objects.size()));  // how many rows
    row_header(1);                             // enable row headers (along left)
    row_height_all(20);                        // default height of rows
    row_resize(0);                             // disable row resizing
    end();			                           // end the Fl_Table group
}

void OP::ObjectTable::draw_cell(const TableContext context, const int row , const int col, 
               const int x, const int y, const int width, const int height)
{
    static char s[256];
    switch (context) {
    case CONTEXT_STARTPAGE:                   // before page is drawn..
        fl_font(FL_HELVETICA, 16);              // set the font for our drawing operations
        return; 
    case CONTEXT_COL_HEADER:                  // Draw column headers
        sprintf(s,"%s", m_labels[col].c_str());
        DrawHeader(s, x, y, width, height);
        return; 
    case CONTEXT_ROW_HEADER:                  // Draw row headers
        sprintf(s,"%03d:", row);                 // "001:", "002:", etc
        DrawHeader(s, x, y, width, height);
        return; 
    case CONTEXT_CELL:                        // Draw data in cells
        DefineDataToDraw(s, row, static_cast<ColType>(col));
        DrawData(s, x, y, width, height);
        return;
    default:
        return;
    }
}
