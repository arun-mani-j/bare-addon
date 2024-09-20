const binding = require('./binding')
const path = require('bare-path')
const fs = require('bare-fs')

function hello () {
  const filename = path.join(__dirname, 'data', 'foo')
  console.log('Using filename:', filename)
  console.log('Reading via bare-fs', fs.readFileSync(filename).toString())
  console.log('Calling native function')
  console.log(binding.hello(filename))
}

function write (message, filename) {
  if (filename === undefined) { filename = path.join(__dirname, 'data', 'foo') }

  const dir = path.dirname(filename)
  if (!fs.existsSync(dir)) { fs.mkdirSync(dir, { recursive: true }) }

  fs.writeFileSync(filename, message, { flush: true })
  console.log('Written', message, 'to', filename)
}

exports.hello = hello
exports.write = write
