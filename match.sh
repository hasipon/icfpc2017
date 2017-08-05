#!/bin/bash

set -eu

function usage(){
    echo match.sh PORT MAP-NAME AI-NAME1 AI-NAME2 NUM_PENTER
}
PORT=$1
MAP=$2
AI1=$3
AI2=$4
NUM_PENTER=$5

ruby ./common/server/app.rb -n $5 -p $PORT -m ./maps/$MAP.json &
sleep 1

pushd ./AIs/$AI1/
make
popd

pushd ./AIs/$AI2/
make
popd

pushd ./dashboard/static
ruby ../../common/script/server_connecter.rb -p $PORT -a ../../AIs/$AI1/a.out -h localhost -n $AI1 &
ruby ../../common/script/server_connecter.rb -p $PORT -a ../../AIs/$AI2/a.out -h localhost -n $AI2 &
popd

RANDOM_AI=shiota_ai
pushd ./AIs/$RANDOM_AI/
make
popd
pushd ./dashboard/static
for ((i=2; i < $NUM_PENTER; i++)); do
    ruby ../../common/script/server_connecter.rb -p $PORT -a ../../AIs/$RANDOM_AI/a.out -h localhost -n ${RANDOM_AI}-$i &
done
