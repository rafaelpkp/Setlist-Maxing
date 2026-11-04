

## Setlist idea

The user picks songs for their repertoire and enters how long they have to
play. For now, we'll go through the songs in that order and add each one if it
fits. We'll skip songs with no ID or an invalid duration, and only use each ID
once. If the time limit is zero or less, or nothing fits, the setlist is empty.
The total can be under the limit, but it should never go over.

This won't always fill the time as closely as possible because the order of
the repertoire matters. We can improve that later.

To order the songs, we'll keep the first selected song as the opener. Then
we'll compare it to the remaining songs and pick the one with the lowest
transition score. We'll repeat that until every song is in the setlist.

For a first try, the score will be 50% BPM difference and 50% energy
difference. Lower means the songs are more similar:

- BPM: divide the difference by 50 and cap it at 1.
- Energy: use the difference between the two values, which range from 0 to 1.

If either song is missing a valid feature, leave that feature out and adjust the
weights based on what's left. BPM needs to be positive and energy needs to be
0–1. If neither feature can be compared, the transition is unknown. We'll
prefer songs with a known transition score; if all the remaining transitions
are unknown, we'll pick the first one. If known scores tie, pick the first one
encountered among the remaining songs.

These weights are just a starting point. We'll need to try the results and see
if the transitions sound good. The current generator selects valid songs that
fit, skips duplicate IDs, and orders the selected songs. We still need to test
it with more cases.
