require 'sinatra'
require 'sinatra/reloader'
require 'haml'

my_requires = %w[client data_helper pagination_helper]
my_requires.each { |f| require_relative f }

configure :development do
  enable :reloader
  my_requires.each { |f| also_reload "#{f}.rb" }
end

before do
  cache_control :public, :must_revalidate, :max_age => 60
  @category = params[:category]
  @query_id = params[:query_id]
  @query_type = params[:query_type]
end

helpers DataHelper, PaginationHelper

get '/' do
  redirect to '/search?category=bag'
end

get '/search' do
  haml :search
end

post '/search' do
  results = client.send({
    type: Vis::RequestType::OFFLINE,
    category: @category,
    query_type: @query_type,
    num_results: 10,
    id: @query_id
  })
  @images = results[:results].map { |r| [r[:id], File.join('/data', r[:path])] }
  haml :search
end

get '/all' do
  list = images_for @category
  @images = paginate list, page
  @num_pages = pages list
  haml :all
end

