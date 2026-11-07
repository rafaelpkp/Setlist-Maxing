# The Best Setlist

Search a catalog of songs, build a repertoire, and generate a setlist for the
time you have. You can also compare a hash table and red-black tree using the
same songs.

## Setup

You'll need CMake 3.24 or newer, a C++17 compiler, Ninja, Node.js (18, 20, or
22+), and npm. The first CMake configuration downloads Crow and Asio.

Download `tracks.csv` from the [Spotify dataset](https://www.kaggle.com/datasets/yamaerenay/spotify-dataset-19212020-600k-tracks?select=tracks.csv)
and put it in a `data` folder in the repo root. The full dataset isn't included
in the repo.

From the repo root, build and run the backend:

```sh
cmake -S backend -B backend/build -G Ninja -DCMAKE_BUILD_TYPE=Debug
cmake --build backend/build
./backend/build/setlist_server data/tracks.csv
```

On Windows, use `backend\build\setlist_server.exe data\tracks.csv` for the last
command. For a quick test without the full dataset, use
`backend/tests/fixtures/sample_tracks.csv` instead of `data/tracks.csv`.

In a second terminal, start the frontend:

```sh
cd frontend
npm install
npm run dev
```

Open the local URL Vite prints, usually `http://localhost:5173`. Keep both
terminals open while using the app. Press Ctrl+C in each terminal to stop it.

## How it works

The server loads each unique song into both structures, keyed by Spotify ID.
Title and artist searches scan a separate list to find matching IDs; the
selected structure then retrieves the songs. The same structure retrieves your
chosen songs when generating a setlist.

The generator adds songs in the order you picked them, skipping anything that
won't fit. It keeps the first song as the opener and orders the rest using BPM
and energy. Each gets equal weight when both are available. This is a simple
approach, so it won't always fill the time as closely as possible. Your
repertoire isn't saved when you refresh the page.

The comparison runs insertion, exact ID lookup, and deletion on fresh copies of
the full catalog. Times can vary, so repeat runs in a release build when
comparing results. The storage estimate counts the structures and their reserved
song slots or nodes, but leaves out separately allocated strings and allocator
overhead; it isn't total process memory.

## Tests

From the repo root, run:

```sh
ctest --test-dir backend/build --output-on-failure
```

Use a debug build for the tests so their `assert` checks stay enabled.
