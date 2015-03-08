require 'test_helper'
require 'client.rb'

class ClientTest < Minitest::Test

  def setup
    @client = Vis::Client.new
  end

  def test_probe
    assert @client.probe
  end

  def test_offline_request
    category = 'bag'
    num_results = 10

    request = {
      type: Vis::RequestType::OFFLINE,
      category: category,
      query_type: Vis::QueryType::COLOR,
      num_results: num_results,
      id: 42
    }
    response = @client.send request

    check request, response
  end

  def test_realtime_request
    category = 'shoe'
    num_results = 10

    request = {
      type: Vis::RequestType::REALTIME,
      category: category,
      query_type: Vis::QueryType::SHAPE,
      num_results: num_results,
      descriptors: Array.new(Vis::DescriptorsSize::SHAPE, 0.0)
    }
    response = @client.send request

    check request, response
  end

  private

    def check(request, response)
      assert_equal Vis::ResponseStatus::OK, response[:status]
      assert_equal request[:num_results], response[:num_results]
      assert_equal request[:num_results], response[:results].size
      response[:results].each { |r|
        file = DATA_DIR + r[:path]
        assert File.exists?(file), "File #{file} does not exist"
      }
    end

end
