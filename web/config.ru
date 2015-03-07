require 'service_manager'
ServiceManager.start

require 'rack/coffee'
use Rack::Coffee, root: File.join(Dir.pwd, 'javascripts'), urls: '/'

require_relative 'app'
run Sinatra::Application
