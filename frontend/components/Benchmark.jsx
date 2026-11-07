import React, { useState } from 'react';

function formatMs(value) {
  return `${value.toFixed(2)} ms`;
}

function formatMemory(bytes) {
  return `${(bytes / (1024 * 1024)).toFixed(2)} MiB`;
}

export default function Benchmark() {
  const [result, setResult] = useState(null);
  const [loading, setLoading] = useState(false);
  const [error, setError] = useState('');

  async function runComparison() {
    setLoading(true);
    setError('');
    setResult(null);
    try {
      const response = await fetch('/api/benchmark', {
        method: 'POST',
      });
      if (!response.ok) throw new Error(await response.text());
      setResult(await response.json());
    } catch (requestError) {
      setError(requestError.message || 'Could not run comparison');
    } finally {
      setLoading(false);
    }
  }

  return (
    <section className="panel benchmark">
      <h2>Compare the structures</h2>
      <p className="muted">
        Run every song in the catalog through each structure and compare the total time.
      </p>
      <button className="generate" onClick={runComparison} disabled={loading}>
        {loading ? 'Comparing the full catalog…' : 'Compare the full catalog'}
      </button>
      {error && <p className="error" role="alert">{error}</p>}
      {result && (
        <>
          <p className="muted">{result.hash.songCount.toLocaleString()} unique songs tested</p>
          <div className="table-scroll">
            <table>
              <thead>
                <tr>
                  <th>Operation</th>
                  <th>Hash table</th>
                  <th>Red-black tree</th>
                </tr>
              </thead>
              <tbody>
                <tr>
                  <th>Insert all songs</th>
                  <td>{formatMs(result.hash.insertMs)}</td>
                  <td>{formatMs(result.tree.insertMs)}</td>
                </tr>
                <tr>
                  <th>Find all IDs</th>
                  <td>{formatMs(result.hash.findMs)}</td>
                  <td>{formatMs(result.tree.findMs)}</td>
                </tr>
                <tr>
                  <th>Delete all IDs</th>
                  <td>{formatMs(result.hash.eraseMs)}</td>
                  <td>{formatMs(result.tree.eraseMs)}</td>
                </tr>
                <tr>
                  <th>Structural storage estimate</th>
                  <td>{formatMemory(result.hash.storageBytes)}</td>
                  <td>{formatMemory(result.tree.storageBytes)}</td>
                </tr>
              </tbody>
            </table>
          </div>
        </>
      )}
    </section>
  );
}
