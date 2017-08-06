import threading
import subprocess
import json
import random
import sys
import pathlib
import os
import glob
import time

port = 9999
ai_bin_path = os.path.expanduser('~/LOCAL/autobattle/ai')
maps_path = os.path.expanduser('~/LOCAL/autobattle/maps')
simulator_rb = os.path.expanduser('~/LOCAL/autobattle/server/app.rb')
server_connector_rb = os.path.expanduser('~/LOCAL/autobattle/server_connecter2.rb')
log_path = os.path.expanduser('~/LOCAL/dashboard/dashboard/static/logs')

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
      "-m", battle_map,
      "-s", '{"futures": true}',
    ], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    return sim

def prepare():
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
    return battle_map, participants

def run_offline():
    battle_map, participants = prepare()
    print("Map",  battle_map)
    print("Participants", participants)

    options = [
      '/usr/bin/ruby',
      simulator_rb,
      '--mode', 'offline'
      '-n', str(len(participants)),
      '-m', battle_map,
      '-s', '{"futures": true}',
      '--punters'
    ]

    for ai in participants:
        options.append(ai)

    sim = subprocess.Popen(options, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    sim.wait()

    print("sim.stdout")
    print(sim.stdout.read().decode('utf-8'))
    print("sim.stderr")
    print(sim.stderr.read().decode('utf-8'))

def run():
    battle_map, participants = prepare()
    print("Map",  battle_map)
    print("Participants", participants)

    print(participants)
    logname = 'autobattle-' + '-'.join(list(map(os.path.basename, participants))) + '@' + str(int(time.time())) + '.log'

    sim = run_server(n, battle_map)
    time.sleep(1.0)

    ai_ps = []

    for i in range(n):
        options = [
          '/usr/bin/ruby',
          server_connector_rb ,
          '-p', str(port),
          '-a', participants[i],
          '-h', 'localhost',
          '-n', os.path.basename(participants[i]),
        ]
        if i == 0:
            options.append('-l')
            options.append(logname)
        else:
            options.append('-q')
        p = subprocess.Popen(options)
        ai_ps.append(p)

    sim.wait()
    print("sim.stdout")
    print(sim.stdout.read().decode('utf-8'))
    print("sim.stderr")
    print(sim.stderr.read().decode('utf-8'))

    with open(logname) as f:
        lines = f.readlines()
        if 2 <= len(lines):
            scores = json.loads(lines[-1])
            if scores and 'stop' in scores:
                os.rename(logname, os.path.join(log_path, logname))

def fix_name(name):
    if '@' not in name:
        return name
    return name.split('@')[0]

def calc_rating():
    log_files = glob.glob(os.path.join(str(log_path), '*@[1-9]*.log'))
    rating = {}

    for log in log_files:
        info = None
        scores = None
        with open(log) as f:
            lines = f.readlines()
            if len(lines) < 2:
                continue
            info = json.loads(lines[1])
            scores = json.loads(lines[-1])

        if not info or not scores:
            continue
        if 'punter_names' not in info or 'stop' not in scores:
            continue

        ranking = []
        names = info['punter_names']

        for i in range(len(names)):
            names[i] = fix_name(names[i])

        for name in names:
            if name not in rating:
                rating[name] = 1500

        for kv in scores['stop']['scores']:
            name = names[kv['punter']]
            score = kv['score']
            ranking.append((score, name))

        ranking.sort(reverse=True)

        n = len(ranking)
        diff = [0] * n

        for i in range(n):
            for j in range(i + 1, n):
                if ranking[i][0] == ranking[j][0]:
                    continue
                rd = (rating[names[j]] - rating[names[i]]) * 0.04
                diff[i] += 16 + rd
                diff[j] -= 16 + rd

        for i in range(n):
            rating[names[i]] += diff[i]

    print(json.dumps(rating))

def main():
    if sys.argv[1] == 'run':
        run_offline()
    if sys.argv[1] == 'eternal-run':
        while True:
            run()
            time.sleep(1.0)
    elif sys.argv[1] == 'rating':
        calc_rating()

if __name__ == '__main__':
    main()

