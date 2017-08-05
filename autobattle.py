import threading
import subprocess
import random
import pathlib
import os
import glob
import time

port = 9999
ai_bin_path = '/home/ubuntu/LOCAL/autobattle/ai'
maps_path = '/home/ubuntu/LOCAL/autobattle/maps'
simulator_rb = '/home/ubuntu/LOCAL/autobattle/server/app.rb'
server_connector_rb = '/home/ubuntu/LOCAL/autobattle/server_connector.rb'

def find_map_files():
    return glob.glob(os.path.join(maps_path, '*.json'))

def find_ai_files():
    return glob.glob(os.path.join(ai_bin_path, '*.out'))

def choice_map(maps):
    return random.choice(maps)

def choice_participants(ais_, n):
    ais = ais_[:] #copy
    random.shuffle(ais)
    return ais[:n]

def run_server(n, battle_map):
    sim = subprocess.Popen([
      '/usr/bin/ruby',
      simulator_rb,
      "-p", str(port),
      "-n", str(n),
      "-m", battle_map
    ], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    return sim

def run():
    maps = find_map_files()
    print("=== Map List ===")
    for m in maps:
        print(m)

    ais = find_ai_files()
    ais.sort()

    print("=== AI List ===")
    for ai in ais:
        print(ai)

    n = random.randint(2, len(ais))
    battle_map = choice_map(maps)
    participants = choice_participants(ais, n)

    print("Map",  battle_map)
    print("Participants", participants)

    sim = run_server(n, battle_map)

    time.sleep(1.0)

    ai_ps = []
    logname = 'autobattle-' + '-'.join(map(os.path.basename, participants))

    for i in range(n):
        options = [
          '/usr/bin/ruby',
          server_connector_rb ,
          '-p', str(port),
          '-a', participants[i],
          '-h', 'localhost',
          '-n', logname,
        ]
        if i == 0:
            options.append('-q')
        p = subprocess.Popen(options)
        ai_ps.append(p)

    sim.wait()
    print("sim.stdout")
    print(sim.stdout.read().decode('utf-8'))
    print("sim.stderr")
    print(sim.stderr.read().decode('utf-8'))

if __name__ == '__main__':
    run()

    
