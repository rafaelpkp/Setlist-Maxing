import React from 'react';
import { formatTime } from '../formatTime.js';

export default function Repertoire({
  songs, onRemove, minutes, onMinutesChange, structure, onStructureChange,
  onGenerate, loading, error,
}) {
  const totalDuration = songs.reduce((total, song) => total + song.durationMs, 0);
  const invalidMinutes = !minutes || Number(minutes) <= 0 || Number(minutes) > 1440;

  return (
    <section className="panel">
      <h2>2. Your repertoire <span className="count">{songs.length}</span></h2>
      <p className="muted">Songs are considered in the order you add them.</p>

      <div className="selected">
        {songs.length === 0 && (
          <div className="empty">
            Search for songs to add to your repertoire.
          </div>
        )}
        {songs.map((song, index) => (
          <div className="song" key={song.id}>
            <span className="index">{String(index + 1).padStart(2, '0')}</span>
            <div>
              <strong>{song.title}</strong>
              <small>{song.artists}</small>
            </div>
            <span className="duration">{formatTime(song.durationMs)}</span>
            <button
              className="remove"
              onClick={() => onRemove(song.id)}
              aria-label={`Remove ${song.title}`}
            >
              ×
            </button>
          </div>
        ))}
      </div>

      <div className="total">
        Repertoire duration <strong>{formatTime(totalDuration)}</strong>
      </div>
      <div className="controls">
        <div>
          <label htmlFor="minutes">Time available (minutes)</label>
          <input
            id="minutes"
            type="number"
            min="1"
            max="1440"
            value={minutes}
            onChange={(event) => onMinutesChange(event.target.value)}
          />
        </div>
        <div>
          <label htmlFor="structure">Structure for search and setlist</label>
          <select
            id="structure"
            value={structure}
            onChange={(event) => onStructureChange(event.target.value)}
          >
            <option value="hash">Hash table</option>
            <option value="tree">Red-black tree</option>
          </select>
        </div>
      </div>
      <button
        className="generate"
        disabled={songs.length === 0 || loading || invalidMinutes}
        onClick={onGenerate}
      >
        {loading ? 'Building…' : 'Generate setlist'} <span>↗</span>
      </button>
      {error && <p className="error" role="alert">{error}</p>}
    </section>
  );
}
