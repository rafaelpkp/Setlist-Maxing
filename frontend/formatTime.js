// display a song's duration as minutes and seconds
export function formatTime(milliseconds) {
  const minutes = Math.floor(milliseconds / 60000);
  const seconds = Math.floor(milliseconds / 1000) % 60;
  return `${minutes}:${String(seconds).padStart(2, '0')}`;
}
