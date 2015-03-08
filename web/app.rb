require 'sinatra'
require 'sinatra/reloader'
require 'haml'
require 'json'
require 'will_paginate/array'
require 'will_paginate-bootstrap'

WillPaginate.per_page = 12;

my_requires = %w[client data_helper pagination_helper]
my_requires.each { |f| require_relative f }

configure :development do
  enable :reloader
  my_requires.each { |f| also_reload "#{f}.rb" }
end

before do
  cache_control :public, :must_revalidate, :max_age => 60
end

before /(search|all)/ do
  @category = params[:category] || categories.first.first
  @query_type = params[:query_type]
  @num_results = params[:num_results] || WillPaginate.per_page
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
  @images = images_for(@category).paginate page: page
  haml :all
end

