Gem::Specification.new do |gem|
  gem.name = "ruby-numtheory"
  gem.homepage = "http://github.com/lomereiter/ruby-numtheory"
  gem.license = "MIT"
  gem.summary = "Ruby number theory library"
  gem.version = "0.0.8"
  gem.platform = Gem::Platform::RUBY

  gem.description = <<-EOF
    The library is written as C extension and aims to provide
    common number-theoretical functions such as powermod, jacobi symbol,
    various multiplicative ones, etc.
  EOF

  gem.email = "lomereiter@gmail.com"
  gem.authors = ["Artem Tarasov"]

  gem.extensions = FileList["ext/numtheory/extconf.rb"]
  gem.require_path = 'ext'
  gem.files = `git ls-files`.split("\n")

  gem.has_rdoc = true
  gem.extra_rdoc_files = ["ext/numtheory/numtheory.c", "LICENSE.txt"]
end
