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
end

helpers DataHelper, PaginationHelper

get '/' do
  redirect to '/search', 301
end

get '/search' do
  haml :search
end

post '/search' do
  results = client.send({
    type: Vis::RequestType::OFFLINE,
    category: params[:category],
    query_type: params[:query_type],
    num_results: 10,
    id: params[:'query_id']
  })
  @images = results[:results].map { |r| [r[:id], File.join('/data', r[:path])] }
  haml :search
end

get '/all' do
  @images = paginate bags, page
  @num_pages = pages bags
  haml :all
end

