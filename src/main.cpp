#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include "object_viewer.hpp"
#include "file.hpp"
#include "log.hpp"

int main(int argc, char **argv) 
{
    std::optional<OP::file_buffer> file = {};
    if (argc == 2) {
        if (std::filesystem::exists(argv[1]) == false) {
            OP::LogError("File doesn't exists");
            return EXIT_FAILURE;
        } else {
            file = OP::ReadFileAndValidate(argv[1]);
            if (file.has_value() == false) {
                OP::LogError("Can't read or validate file!");
                return EXIT_FAILURE;
            }
        }
    }

    Fl_Window        window(800, 600, "Object viewer");
    OP::ObjectViewer viewer(window, file);    
    window.show();
    return Fl::run();
}
