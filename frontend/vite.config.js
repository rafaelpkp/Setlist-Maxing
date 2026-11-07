import { defineConfig } from 'vite';

export default defineConfig({
  server: {
    // send frontend API requests to the local Crow server
    proxy: { '/api': 'http://127.0.0.1:18080' },
  },
});
