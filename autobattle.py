import threading
import subprocess
import json
import random
import sys
import pathlib
import os
import glob
import time
from subprocess import CalledProcessError

port = 9999
ai_bin_path = os.path.expanduser('~/LOCAL/autobattle/ai')
maps_path = os.path.expanduser('~/LOCAL/autobattle/maps')
simulator_rb = os.path.expanduser('~/LOCAL/autobattle/server/app.rb')
server_connector_rb = os.path.expanduser('~/LOCAL/autobattle/server_connecter2.rb')
log_path = os.path.expanduser('~/LOCAL/dashboard/dashboard/static/logs')
static_path = os.path.expanduser('~/LOCAL/dashboard/dashboard/static')

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
    print("=== Config ===")
    print("Map",  battle_map)
    print("Participants", participants)

    logname = 'AB-' + '-'.join(list(map(os.path.basename, participants))) + '@' + str(int(time.time())) + '.log'

    options = [
      '/usr/bin/ruby',
      simulator_rb,
      '--mode', 'offline',
      '-n', str(len(participants)),
      '-m', battle_map,
      '-s', '{"futures": true}',
      '--logfile', logname,
      '--punters', ','.join(participants)
    ]

    print("=== Command ===")
    print(' '.join(options))

    with subprocess.Popen(options, stdout=subprocess.PIPE, stderr=subprocess.PIPE) as sim:
        print("=== sim.stdout ===")
        for line in sim.stdout:
            print(line.decode('utf-8'), end='', flush=True)

        print("=== sim.stderr ===")
        for line in sim.stderr:
            print(line.decode('utf-8'), end='')

        sim.wait()

        print("sim.returncode", sim.returncode)
        if sim.returncode != 0:
            sys.exit(sim.returncode)

        valid_log = False
        with open(logname) as f:
            lines = f.readlines()
            if 2 <= len(lines):
                scores = json.loads(lines[-1])
                if scores and 'stop' in scores:
                    valid_log = True
                else:
                    print("scores.stop is not exist")
            else:
                print("too short log")

        print("valid_log", valid_log)
        if valid_log:
            os.chmod(logname, 0o777)
            os.rename(logname, os.path.join(log_path, logname))
        else:
            sys.exit(1)

def run_old():
    battle_map, participants = prepare()
    print("Map",  battle_map)
    print("Participants", participants)

    print(participants)
    logname = 'AB-' + '-'.join(list(map(os.path.basename, participants))) + '@' + str(int(time.time())) + '.log'

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
    log_files = glob.glob(os.path.join(str(log_path), 'AB-*@[1-9]*.log'))
    log_files.sort(key=lambda x: x.split('@')[1]) 
    users = {}
    rating = {}
    history = []

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
                users[name] = {}
                users[name]['battle'] = 0
                users[name]['win'] = 0

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
                ri = rating[names[i]]
                rj = rating[names[j]]

                ei = 1.0 / (1.0 + 10 ** ((rj - ri) / 400.0))
                ej = 1.0 / (1.0 + 10 ** ((ri - rj) / 400.0))

                diff[i] += 16 * (1 - ei)
                diff[j] += 16 * (0 - ej)

        for i in range(n):
            name = ranking[i][1]
            rating[name] += diff[i]

        for i in range(n):
            users[names[i]]['battle'] += 1

        users[ranking[0][1]]['win'] += 1
        his = {}
        his['log'] = os.path.basename(log)
        his['ranking'] = ranking
        his['diff'] = {}

        for i in range(n):
            his['diff'][names[i]] = diff[i]
        history.append(his)

    output = {}
    output['ranking'] = []

    rank = []
    for name in users.keys():
        rank.append((rating[name], name))
    rank.sort(reverse=True)
    for r in rank:
        output['ranking'].append(r[1])

    output['users'] = users
    for name in users.keys():
        users[name]['rating'] = rating[name]

    history.reverse()
    output['history'] = history

    output['update'] = str(int(time.time()))

    rating_json = 'rating.json'
    print(json.dumps(output))
    with open(rating_json, 'w') as outfile:
        json.dump(output, outfile)
    os.chmod(rating_json, 0o777)
    os.rename(rating_json, os.path.join(static_path, rating_json))

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

