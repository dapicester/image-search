require 'socket'

server = TCPServer.open 4567
puts "listening"
loop {
  client = server.accept
  puts "accepted"

  s = client.recv 1024
  puts "received: #{s.unpack 'Qa*'}"

  r = "0"
  client.puts [r.size, r].pack("Qa*")
  puts "responded: #{r}"

  client.close
  puts "closed"
}
