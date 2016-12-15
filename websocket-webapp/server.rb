
require 'sinatra'
require 'sinatra-websocket'

set :server, 'thin'
set :sockets, []
set :admin_sockets, []

get '/client' do
  erb :client
end

get '/ws/client' do
  if request.websocket? then
    request.websocket do |ws|
      ws.onopen do
        ws.send("hello")
        settings.sockets << ws
      end
      ws.onmessage do |msg|
        content = "[#{Time.now}] #{msg}"
        settings.sockets.each { |socket| socket.send(content) }
        settings.admin_sockets.each { |socket| socket.send(content) }
      end
      ws.onclose do
        warn("client has gone")
        settings.sockets.delete(ws)
      end
    end
  end
end

get '/admin' do
  erb :admin
end

get '/ws/admin' do
  if request.websocket? then
    request.websocket do |ws|
      ws.onopen do
        warn("admin is here")
        ws.send("hello")
        settings.admin_sockets << ws
      end
      ws.onmessage do |msg|
        content = "[#{Time.now}] #{msg}"
        settings.sockets.each { |socket| socket.send(content) }
        settings.admin_sockets.each { |socket| socket.send(content) }
      end
      ws.onclose do
        warn("admin has gone")
        settings.admin_sockets.delete(ws)
      end
    end
  end
end
