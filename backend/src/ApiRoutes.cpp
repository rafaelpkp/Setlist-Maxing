#include "ApiRoutes.h"
#include "SetlistGenerator.h"
#include "Benchmark.h"
#include <vector>

namespace {
// turn a song into the format the frontend expects
crow::json::wvalue songJson(const Song& song) {
    crow::json::wvalue result;
    result["id"] = song.id;
    result["title"] = song.title;
    result["artists"] = song.artists;
    result["durationMs"] = song.durationMs;
    result["bpm"] = song.bpm;
    result["energy"] = song.energy;
    return result;
}

crow::json::wvalue songListJson(const std::vector<Song>& songs) {
    std::vector<crow::json::wvalue> result;
    for (const Song& song : songs) {
        result.push_back(songJson(song));
    }
    return crow::json::wvalue(std::move(result));
}

crow::response searchSongs(const crow::request& request, const SongLibrary& library) {
    const char* query = request.url_params.get("q");
    const char* requestedStructure = request.url_params.get("structure");
    std::string structure = requestedStructure ? requestedStructure : "hash";
    if (structure != "hash" && structure != "tree") {
        return crow::response(400, "Invalid structure");
    }
    std::vector<Song> matches = library.search(query ? query : "", structure);

    crow::json::wvalue result;
    result["songs"] = songListJson(matches);
    return crow::response(result);
}

crow::response makeSetlist(const crow::request& request, const SongLibrary& library) {
    auto body = crow::json::load(request.body);
    if (!body || !body.has("ids") || body["ids"].t() != crow::json::type::List ||
        !body.has("minutes") || body["minutes"].t() != crow::json::type::Number ||
        !body.has("structure") || body["structure"].t() != crow::json::type::String) {
        return crow::response(400, "Expected ids, minutes, and structure");
    }

    double minutes = body["minutes"].d();
    std::string structure = body["structure"].s();
    if (minutes <= 0 || minutes > 1440 || (structure != "hash" && structure != "tree") ||
        body["ids"].size() > 1000) {
        return crow::response(400, "Invalid duration, structure, or song count");
    }

    // get each selected song from the structure the user picked
    std::vector<Song> selected;
    for (const auto& id : body["ids"]) {
        if (id.t() != crow::json::type::String) {
            return crow::response(400, "Song IDs must be strings");
        }
        Song song;
        if (!library.find(id.s(), structure, song)) {
            return crow::response(400, "Song ID not found");
        }
        selected.push_back(song);
    }

    Setlist setlist = generateSetlist(selected, static_cast<long long>(minutes * 60000));
    crow::json::wvalue result;
    result["songs"] = songListJson(setlist.songs);
    result["totalDurationMs"] = setlist.totalDurationMs;
    return crow::response(result);
}

crow::json::wvalue benchmarkJson(const BenchmarkResult& result) {
    crow::json::wvalue json;
    json["songCount"] = result.songCount;
    json["found"] = result.found;
    json["deleted"] = result.deleted;
    json["storageBytes"] = result.storageBytes;
    json["insertMs"] = result.insertMs;
    json["findMs"] = result.findMs;
    json["eraseMs"] = result.eraseMs;
    return json;
}

crow::response benchmark(const SongLibrary& library) {
    if (library.size() == 0) return crow::response(400, "No songs available to compare");
    BenchmarkComparison results = compareStructures(library.songs());
    crow::json::wvalue json;
    json["hash"] = benchmarkJson(results.hash);
    json["tree"] = benchmarkJson(results.tree);
    return crow::response(json);
}
}

void addRoutes(crow::SimpleApp& app, const SongLibrary& library) {
    // the frontend calls this when someone searches for a song
    CROW_ROUTE(app, "/api/songs")([&library](const crow::request& request) {
        return searchSongs(request, library);
    });

    // compare fresh copies so the user's song library stays unchanged
    CROW_ROUTE(app, "/api/benchmark").methods(crow::HTTPMethod::POST)
    ([&library]() {
        return benchmark(library);
    });

    // the frontend sends the selected IDs and time limit here
    CROW_ROUTE(app, "/api/setlist").methods(crow::HTTPMethod::POST)
    ([&library](const crow::request& request) {
        return makeSetlist(request, library);
    });
}
