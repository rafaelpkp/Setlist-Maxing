import React, { useEffect, useState } from 'react';
import SongSearch from './components/SongSearch.jsx';
import Repertoire from './components/Repertoire.jsx';
import Setlist from './components/Setlist.jsx';
import Benchmark from './components/Benchmark.jsx';

export default function App() {
  const [query, setQuery] = useState('');
  const [results, setResults] = useState([]);
  const [selected, setSelected] = useState([]);
  const [minutes, setMinutes] = useState(30);
  const [structure, setStructure] = useState('hash');
  const [setlist, setSetlist] = useState(null);
  const [error, setError] = useState('');
  const [loading, setLoading] = useState(false);

  useEffect(() => {
    if (!query.trim()) {
      setResults([]);
      return;
    }

    setResults([]);

    // wait for the user to pause typing before searching
    const controller = new AbortController();
    const timer = setTimeout(async () => {
      try {
        const response = await fetch(`/api/songs?q=${encodeURIComponent(query.trim())}&structure=${structure}`, {
          signal: controller.signal,
        });
        if (!response.ok) throw new Error('Search failed');
        const data = await response.json();
        setResults(data.songs);
        setError('');
      } catch (searchError) {
        if (searchError.name !== 'AbortError') {
          setError('Could not reach the backend. Make sure the server is running.');
        }
      }
    }, 250);

    // cancel the old search when the query changes
    return () => {
      clearTimeout(timer);
      controller.abort();
    };
  }, [query, structure]);

  function addSong(song) {
    setSelected([...selected, song]);
    setSetlist(null);
  }

  function removeSong(id) {
    setSelected(selected.filter((song) => song.id !== id));
    setSetlist(null);
  }

  async function generateSetlist() {
    setLoading(true);
    setError('');
    try {
      const response = await fetch('/api/setlist', {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({
          ids: selected.map((song) => song.id),
          minutes: Number(minutes),
          structure,
        }),
      });
      if (!response.ok) throw new Error(await response.text());
      setSetlist(await response.json());
    } catch (requestError) {
      setError(requestError.message || 'Could not generate setlist');
    } finally {
      setLoading(false);
    }
  }

  return (
    <main>
      <header>
        <h1>Setlist Maxing</h1>
      </header>

      <div className="layout">
        <SongSearch
          query={query}
          onQueryChange={setQuery}
          results={results}
          selected={selected}
          onAdd={addSong}
        />
        <Repertoire
          songs={selected}
          onRemove={removeSong}
          minutes={minutes}
          onMinutesChange={setMinutes}
          structure={structure}
          onStructureChange={setStructure}
          onGenerate={generateSetlist}
          loading={loading}
          error={error}
        />
      </div>

      {setlist && <Setlist setlist={setlist} />}
      <Benchmark />
    </main>
  );
}
