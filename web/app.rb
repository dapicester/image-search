require 'sinatra'
require 'sinatra/reloader'
require 'haml'

require_relative 'client'
require_relative 'data_helper'
require_relative 'pagination_helper'

configure :development do
  register Sinatra::Reloader
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
  @images = results[:results].map { |r| "data/#{r[:path]}" }
  haml :search
end

get '/all' do
  @images = paginate bags, page
  @num_pages = pages bags
  haml :all
end

