require 'service_manager'

ServiceManager.define_service 'image-search' do |s|
  s.host = 'localhost'
  s.port = 4567

  s.cwd = File.expand_path '../../../build/server', __FILE__

  s.start_cmd = Proc.new { "./server -p #{port} -f config.yml" }

  s.color = 33 # 30~37

  # Service is ready when this regexp matches.
  s.loaded_cue = /Server started/
end
