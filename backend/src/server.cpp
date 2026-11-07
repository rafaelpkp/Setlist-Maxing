#include "crow.h"
#include "ApiRoutes.h"
#include "SongLibrary.h"
#include "SongLoader.h"
#include <iostream>
#include <string>

int main(int argc, char** argv) {
    // use the path from the command line, or the default data file
    std::string path = argc > 1 ? argv[1] : "data/tracks.csv";
    try {
        std::vector<Song> songs = loadSongs(path);
        SongLibrary library(songs);
        std::cout << "Loaded " << library.size() << " unique songs\n";

        crow::SimpleApp app;
        addRoutes(app, library);
        app.bindaddr("127.0.0.1").port(18080).multithreaded().run();
    } catch (const std::exception& error) {
        std::cerr << error.what() << '\n';
        return 1;
    }
}
