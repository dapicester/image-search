require 'service_manager'
ServiceManager.start

require_relative 'app'
run Sinatra::Application
