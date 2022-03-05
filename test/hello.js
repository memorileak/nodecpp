const helloworld = require('../build/Release/helloworld');
const helloasync = require('../build/Release/helloasync');
const hellonan = require('../build/Release/hellonan');

console.log(helloworld.hello());

helloasync.hello('Tung!', (greeting) => {console.log(greeting)});

console.log(hellonan.hello());

console.log('JS program completed!');
