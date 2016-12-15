websocket-server
---

# Environment

Ruby 2.3.1
Sinatra 1.4.7
sinatra-websocket 0.3.1

# Setup

requires `gem`

```
cd /path/to/websocket-server
gem install bundler
bundle install
ruby server.rb
```

# endpoint

- /client GET (client page)
- /admin GET (admin page)
- /ws GET (websocket endpoint)
  - /client GET
  - /admin GET
