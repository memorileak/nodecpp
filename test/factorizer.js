const factorizer = require('../build/Release/factorizer');

factorizer.factorize(7328493247, (value) => {
  console.log(value);
}, () => {
  console.log('All completed!');
});
