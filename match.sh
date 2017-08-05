#!/bin/bash

set -eu

function usage(){
    echo match.sh PORT MAP-NAME AI-NAME1 AI-NAME2 
}
PORT=$1
MAP=$2

ARGS=("$@")
LOG_OPTION="-q"
NUM_ARGS=${#ARGS[@]}

ruby ./common/server/app.rb -n $(( $NUM_ARGS - 2 )) -p $PORT -m ./maps/$MAP.json &
sleep 1

for ((i=2; i < $NUM_ARGS; i++)); do
    AI_NAME=${ARGS[i]}
    echo $AI_NAME
    pushd ./AIs/$AI_NAME/
    make
    popd
    pushd ./dashboard/static
    ruby ../../common/script/server_connecter.rb -p $PORT -a ../../AIs/$AI_NAME/a.out -h localhost -n $AI_NAME $LOG_OPTION &
    popd
    LOG_OPTION=""
done
