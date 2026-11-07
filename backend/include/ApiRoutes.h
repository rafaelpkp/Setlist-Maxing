#pragma once

#include "crow.h"
#include "SongLibrary.h"

// connect the API routes to the song library
void addRoutes(crow::SimpleApp& app, const SongLibrary& library);
