SELECT s.name
FROM songs s
JOIN artists a ON s.artist_id = a.id
where a.name = 'Post Malone';
