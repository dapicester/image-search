module DataHelper

  CATEGORIES = %w[ bag ]

  # TODO dynamically generate the following method using CATEGORIES

  def bags
    @bags ||= images_for 'bag'
  end

  # Get file names for images belonging to category.
  def images_for(category)
    file = "#{settings.public_folder}/data/#{category}.txt"
    File.readlines(file).map { |s| File.join '/data', category, s.chomp }
  end

  QUERY_TYPES = {
    Vis::QueryType::COLOR => 'color',
    #Vis::QueryType::SHAPE => 'shape',
    #Vis::QueryType::COLOR_SHAPE => 'combined'
  }

  def queries_for(category)
    #TODO eval "@#{category}".values_at 1,9,15,27
    list = bags
    queries = [302,298,1,9,15,27]
    Hash[queries.map { |i| [i, list[i]] }]
  end

  def client
    @client ||= Vis::Client.new
  end

end
