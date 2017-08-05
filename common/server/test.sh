#!/bin/bash

ruby app.rb \
     -n 1 \
     -p 8000 \
     -j "$(cat small_map)" \
     -s '{"futures": true}' &

sleep 1

{
  echo '12:{"me":"bob"}'
  sleep 0.1
  echo '47:{"ready":0,"futures":[{"source":0,"target":2}]}'
  sleep 0.1
  echo '44:{"claim":{"punter":0,"source":0,"target":1}}'
  sleep 0.1
  echo '44:{"claim":{"punter":0,"source":2,"target":1}}'
} | telnet localhost 8000
