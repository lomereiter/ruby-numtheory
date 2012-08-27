# encoding: utf-8

require 'rubygems'
require 'bundler'
begin
  Bundler.setup(:default, :development)
rescue Bundler::BundlerError => e
  $stderr.puts e.message
  $stderr.puts "Run `bundle install` to install missing gems"
  exit e.status_code
end
require 'rake'

spec = Gem::Specification.load('ruby-numtheory.gemspec')
  
require 'rspec/core'
require 'rspec/core/rake_task'
RSpec::Core::RakeTask.new(:spec) do |spec|
  spec.pattern = FileList["spec/**/*_spec.rb"]
end

task :test => :spec
task :default => :test

require 'rubygems/package_task'
Gem::PackageTask.new(spec) do |pkg|
end

task :build => :gem

require 'rake/extensiontask'
Rake::ExtensionTask.new('numtheory', spec) do |ext|
  ext.cross_compile = true
end
