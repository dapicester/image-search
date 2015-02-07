require 'sinatra'
require 'sinatra/reloader' if development?
require 'haml'
require_relative 'client'

DATA_DIR = File.expand_path '../../data', __FILE__

set :public_folder, DATA_DIR

helpers do

  def images_for(category)
    Dir["#{DATA_DIR}/#{category}/**"].map { |p| p.gsub DATA_DIR, '' }
  end

  def bags
    @bags ||= images_for 'bag'
  end

  def paginate(array, page, size = 10)
    start = (page - 1) * size
    stop  = page * size - 1

    array[start..stop]
  end

  def page
    (params[:page] || 1).to_i
  end

  def pages(array, size = 10)
    (array.count.to_f / 10).ceil
  end

  def client
    @client ||= Vis::Client.new
  end
end

get '/' do
  redirect to '/search'
end

get '/search' do
  haml :search
end

post '/search' do
  results = client.send({
    type: Vis::RequestType::OFFLINE,
    category: 'bag',
    query_type: Vis::QueryType::COLOR,
    num_results: 10,
    id: params[:id]
  })
  @images = results[:results].map { |r| r[:path] }
  haml :search
end

get '/all' do
  @images = paginate bags, page
  @num_pages = pages bags
  haml :all
end

