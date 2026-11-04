#include "SongLoader.h"

#include <cmath>
#include <fstream>
#include <stdexcept>
#include <unordered_map>

namespace {
// read a whole csv record, including quoted commas, quotes and newlines
bool readRow(istream& file, vector<string>& fields) {
    fields.clear();
    string field;
    bool quoted = false;
    bool started = false;
    char c;
    while (file.get(c)) {
        started = true;
        if (quoted) {
            if (c == '"') {
                if (file.peek() == '"') {
                    file.get(c);
                    field += '"';
                } else {
                    quoted = false;
                }
            } else {
                field += c;
            }
        } else if (c == '"' && field.empty()) {
            quoted = true;
        } else if (c == ',') {
            fields.push_back(field);
            field.clear();
        } else if (c == '\n' || c == '\r') {
            if (c == '\r' && file.peek() == '\n') file.get(c);
            fields.push_back(field);
            return true;
        } else {
            field += c;
        }
    }
    if (quoted) throw runtime_error("Unclosed quoted field in CSV");
    if (!started) return false;
    fields.push_back(field);
    return true;
}

bool parseInt(const string& value, int& result) {
    try {
        size_t end;
        int parsed = stoi(value, &end);
        if (end != value.size()) return false;
        result = parsed;
        return true;
    } catch (const invalid_argument&) {
        return false;
    } catch (const out_of_range&) {
        return false;
    }
}

bool parseDouble(const string& value, double& result) {
    try {
        size_t end;
        double parsed = stod(value, &end);
        if (end != value.size() || !isfinite(parsed)) return false;
        result = parsed;
        return true;
    } catch (const invalid_argument&) {
        return false;
    } catch (const out_of_range&) {
        return false;
    }
}
}

vector<Song> loadSongs(const string& filePath) {
    ifstream file(filePath, ios::binary);
    if (!file.is_open()) throw runtime_error("Could not open CSV file: " + filePath);

    vector<string> fields;
    if (!readRow(file, fields)) throw runtime_error("CSV file is empty: " + filePath);
    // some csv files start with a utf-8 byte order mark
    if (fields[0].compare(0, 3, "\xEF\xBB\xBF") == 0) fields[0].erase(0, 3);
    unordered_map<string, size_t> columns;
    for (size_t i = 0; i < fields.size(); ++i) columns[fields[i]] = i;
    const vector<string> required = {"id", "name", "artists", "duration_ms", "tempo", "key", "mode", "energy"};
    for (const auto& name : required) {
        if (columns.count(name) == 0) throw runtime_error("Missing CSV column: " + name);
    }

    vector<Song> songs;
    size_t headerSize = fields.size();
    size_t rowNumber = 1;
    while (readRow(file, fields)) {
        ++rowNumber;
        if (fields.size() == 1 && fields[0].empty()) continue;
        if (fields.size() != headerSize) {
            throw runtime_error("Wrong number of CSV fields in record " + to_string(rowNumber));
        }
        Song song;
        song.id = fields[columns.at("id")];
        song.title = fields[columns.at("name")];
        song.artists = fields[columns.at("artists")];
        if (song.id.empty() || !parseInt(fields[columns.at("duration_ms")], song.durationMs) || song.durationMs <= 0) continue;

        double number;
        if (parseDouble(fields[columns.at("tempo")], number) && number > 0) song.bpm = number;
        if (parseDouble(fields[columns.at("energy")], number) && number >= 0 && number <= 1) song.energy = number;
        int integer;
        if (parseInt(fields[columns.at("key")], integer) && integer >= 0 && integer <= 11) song.key = integer;
        if (parseInt(fields[columns.at("mode")], integer) && (integer == 0 || integer == 1)) song.mode = integer;
        songs.push_back(song);
    }
    if (file.bad()) throw runtime_error("Error reading CSV file: " + filePath);
    return songs;
}
