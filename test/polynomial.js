const polynomial = require('../build/Release/polynomial');

const a = new polynomial.Polynomial(1, 3, 2);
console.log(a.a);
console.log(a.at(5));
a.a = 3;
console.log(a.a);
console.log(a.at(5));
