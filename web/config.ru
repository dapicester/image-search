require 'service_manager'
ServiceManager.start

require 'rack/coffee'
use Rack::Coffee, root: File.join(Dir.pwd, 'javascripts'), urls: '/'

require 'sass/plugin/rack'
use Sass::Plugin::Rack
Sass::Plugin.options[:template_location] = File.join(Dir.pwd, 'stylesheets')
Sass::Plugin.options[:css_location] = './public'

require_relative 'app'
run Sinatra::Application
