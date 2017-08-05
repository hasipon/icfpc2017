#!/bin/bash

set -eu

function usage(){
    echo match.sh PORT MAP-NAME AI-NAME1 AI-NAME2
}
PORT=$1
MAP=$2
AI1=$3
AI2=$4

ruby ./common/server/app.rb -n 2 -p $PORT -m ./maps/$MAP.json &
sleep 1

pushd ./AIs/$AI1/
make
popd

pushd ./AIs/$AI2/
make
popd

cd ./dashboard/static
ruby ../../common/script/server_connecter.rb -p $PORT -a ../../AIs/$AI1/a.out -h localhost -n $AI1 &
ruby ../../common/script/server_connecter.rb -p $PORT -a ../../AIs/$AI2/a.out -h localhost -n $AI2 &
