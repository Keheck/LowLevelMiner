#include <iostream>
#include <fstream>

/*
 * Synopsis: embed_shader shader-source-code-path shader-header-path shader-name
*/

int main(int argc, char** argv) {
    if(argc != 4) {
        return 1;
    }

    std::ofstream shader_header(argv[2]);
    std::ifstream shader_source(argv[1]);

    if(!shader_header.is_open() || !shader_source.is_open())
        return -1;
    
    shader_header << "#pragma once" << std::endl;
    shader_header << "const char* _" << argv[3] << " = \"";

    while(true) {
        char c = shader_source.get();
        
        if(shader_source.eof()) {
            break;
        }

        switch(c) {
            case '\\':
            case '"':
                shader_header << "\\";
            default:
                shader_header << c;
                break;
            case '\n':
                shader_header << "\\n";
        }
    }

    shader_header << "\\0\";";

    shader_header.close();
    shader_source.close();
}