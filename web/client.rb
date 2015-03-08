require 'logger'
require 'socket'
require 'timeout'

module Vis

  module RequestType
    OFFLINE = 0
    REALTIME = 1
    UPLOAD = 2
  end

  module QueryType
    COLOR = 0
    SHAPE = 1
    COLOR_SHAPE = 2
  end

  module ResponseStatus
    OK = 0
    ERROR = 1
    NO_CATEGORY = 2
    NO_QUERY_TYPE =3
  end

  module DescriptorsSize
    COLOR = 166
    SHAPE = 300
    COLOR_SHAPE = 466
  end

  class Client

    DEFAULT_ADDRESS = '0.0.0.0'
    DEFAULT_PORT = 5858

    def initialize(address = DEFAULT_ADDRESS, port = DEFAULT_PORT)
      @address = address
      @port = port
      @logger = Logger.new STDOUT
      @logger.progname = 'Vis::Client'
    end

    def probe
      @logger.info 'probing ...'
      with_timeout do
        begin
          new_socket.close
          true
        rescue Errno::ECONNREFUSED, Errno::EHOSTUNREACH
          false
        end
      end
    rescue Timeout::Error
      @logger.warn 'timeout'
      false
    end

    def send(request)
      socket ||= new_socket
      with_timeout do
        @logger.info 'sending request ' << request.to_s
        socket.puts serialize request

        received = socket.recv 1024
        socket.close

        response = deserialize received
        @logger.info 'got response ' << response.to_s

        response
      end
    end

    private

      def new_socket
        @logger.info "opening socket to %s:%i" % [@address, @port]
        TCPSocket.new @address, @port
      end

      def with_timeout(sec = 1, &blk)
        Timeout::timeout(sec) { yield blk }
      end

      def serialize(request)
        data = request
            .values_at(*%i[type category query_type num_results id descriptors])
            .compact.flatten.map(&:to_s).join ' '
        [data.size, data].pack 'Qa*'
      end

      def deserialize(response)
        data = response.unpack('Qa*').last.split ' '
        {
          status: data.shift.to_i,
          num_results: data.size/2,
          results: data.each_slice(2).map { |id, path| { id: id.to_i, path: path } }
        }
      end
  end
end
