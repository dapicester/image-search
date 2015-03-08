require 'yaml'

module DataHelper

  def categories
    server_config['categories']
  end

  def query_types_for(category)
    types = categories[category]['type']
    types.map { |s|
      id = case s
      when 'color' then 0
      when 'shape' then 1
      when 'color_shape' then 2
      end
      [id, s]
    }
  end

  # Get file names for images belonging to category.
  def images_for(category)
    file = "#{settings.public_folder}/data/#{category}.txt"
    File.readlines(file).map { |s| File.join '/data', category, s.chomp }
  end

  def queries_for(category, n = 5)
    images =  images_for(category)
    queries = [1,10,20,30,40]
    queries.map { |i| [i, images[i]] }
    #queries = images_for(category).shuffle[0...5]
    #queries.map.with_index { |image, i| [i, image] }
  end

  def client
    @client ||= Vis::Client.new
  end

  def server_config
    @server_config ||= YAML.load File.read 'server-config.yml'
  end

end
