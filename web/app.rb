require 'sinatra'
require 'sinatra/reloader'
require 'haml'
require 'json'

my_requires = %w[client data_helper pagination_helper]
my_requires.each { |f| require_relative f }

configure :development do
  enable :reloader
  my_requires.each { |f| also_reload "#{f}.rb" }
end

before do
  cache_control :public, :must_revalidate, :max_age => 60
  @category = params[:category] || categories.first.first
  @query_type = params[:query_type]
  @num_results = params[:num_results] || 10
  @query_id = params[:query_id]
end

helpers DataHelper, PaginationHelper

get '/' do
  redirect to '/search'
end

get '/search' do
  haml :search
end

post '/search.?:format?' do
  response = client.send({
    type: Vis::RequestType::OFFLINE,
    category: @category,
    query_type: @query_type,
    num_results: @num_results,
    id: @query_id
  })
  @images = response[:results].map { |r| [r[:id], File.join('/data', r[:path])] }

  return JSON.dump @images if params[:format] == 'json'
  haml :search
end

get '/all' do
  list = images_for @category
  @images = paginate list, page
  @num_pages = pages list
  haml :all
end

