const factorizer = require('../build/Release/factorizer');

factorizer.factorize(9007199254740991, (value) => {
  console.log(value);
}, () => {
  console.log('All completed!');
});
