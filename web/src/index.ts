const img = document.getElementById('frame') as HTMLImageElement;
const stats = document.getElementById('stats') as HTMLDivElement;

// Replace with an exported base64 PNG from the Android app to preview real frames.
const sampleBase64 = "data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAAUA" +
  "AAAFCAYAAACNbyblAAAAHElEQVQI12P4//8/w38GIAXDIBKE0DHxgljNBAAO" +
  "9TXL0Y4OHwAAAABJRU5ErkJggg==";

img.src = sampleBase64;
img.onload = () => {
  stats.innerText = `FPS: -- | Resolution: ${img.naturalWidth}x${img.naturalHeight}`;
};
