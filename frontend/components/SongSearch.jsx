import React from 'react';
import { formatTime } from '../formatTime.js';

export default function SongSearch({ query, onQueryChange, results, selected, onAdd }) {
  return (
    <section className="panel">
      <h2>1. Find your songs</h2>

      <label htmlFor="search">Search by title or artist</label>
      <input
        id="search"
        value={query}
        onChange={(event) => onQueryChange(event.target.value)}
        placeholder="Try an artist or song name"
      />

      <div className="results">
        {query.trim() && results.length === 0 && <p className="muted">No songs found.</p>}
        {results.map((song) => {
          const alreadySelected = selected.some((item) => item.id === song.id);
          return (
            <div className="song" key={song.id}>
              <div>
                <strong title={song.title}>{song.title}</strong>
                <small title={song.artists}>{song.artists}</small>
              </div>
              <span className="duration">{formatTime(song.durationMs)}</span>
              <button
                className="add"
                disabled={alreadySelected}
                onClick={() => onAdd(song)}
                aria-label={`Add ${song.title}`}
              >
                {alreadySelected ? '✓' : '+'}
              </button>
            </div>
          );
        })}
      </div>
    </section>
  );
}
