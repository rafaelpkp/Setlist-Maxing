import React from 'react';
import { formatTime } from '../formatTime.js';

export default function Setlist({ setlist }) {
  return (
    <section className="panel output">
      <h2>Your setlist</h2>
      <p className="muted">
        {setlist.songs.length} songs · {formatTime(setlist.totalDurationMs)} total
      </p>
      {setlist.songs.length > 0 ? (
        setlist.songs.map((song, index) => (
          <div className="song" key={song.id}>
            <span className="index">{String(index + 1).padStart(2, '0')}</span>
            <div>
              <strong>{song.title}</strong>
              <small>{song.artists}</small>
            </div>
            <span className="duration">{formatTime(song.durationMs)}</span>
          </div>
        ))
      ) : (
        <p>No selected songs fit the available time.</p>
      )}
    </section>
  );
}
