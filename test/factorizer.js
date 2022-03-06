const factorizer = require('../build/Release/factorizer');

factorizer.factorize(7328493247, (value) => {
  console.log('Factorizer1: ', value);
}, () => {
  console.log('Factorizer1 completed!');
});

factorizer.factorize(7328493321, (value) => {
  console.log('Factorizer2: ', value);
}, () => {
  console.log('Factorizer2 completed!');
});
