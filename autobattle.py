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
ai_bin_path = os.path.expanduser('~/LOCAL/autobattle/ai3')
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

    logname = 'AC-' + os.path.basename(battle_map) + '-' + '-'.join(list(map(os.path.basename, participants))) + '@' + str(int(time.time())) + '.log'

    options = [
      '/usr/bin/ruby',
      simulator_rb,
      '--mode', 'offline',
      '-n', str(len(participants)),
      '-m', battle_map,
      '--logfile', logname,
      '--punters', ','.join(participants)
    ]

    options.append('-s')

    settings = {}
    if bool(random.getrandbits(1)):
        settings['futures'] = True
    if bool(random.getrandbits(1)):
        settings['splurges'] = True
    if bool(random.getrandbits(1)):
        settings['options'] = True

    options.append(json.dumps(settings))

    print("=== Command ===")
    print(' '.join(options))

    subprocess.check_output(options)

    valid_log = True
    with open(logname) as f:
        lines = f.readlines()
        if 2 <= len(lines):
            info = json.loads(lines[1])
            scores = json.loads(lines[-1])
            if not info or not scores:
                valid_log = False
            if valid_log and 'punter_names' not in info or 'stop' not in scores:
                valid_log = False
            if valid_log:
                names = info['punter_names']
                if len(set(names)) != len(names):
                    valid_log = False
                    print("same name punters", names)
                    print("Participants", participants)
        else:
            print("too short log")
            valid_log = False

    print("valid_log", valid_log)
    if valid_log:
        os.chmod(logname, 0o777)
        os.rename(logname, os.path.join(log_path, logname))
    else:
        sys.exit(1)

def fix_name(name):
    if '@' not in name:
        return name
    return name.split('@')[0]

def calc_rating():
    log_files = glob.glob(os.path.join(str(log_path), 'AC-*@[1-9]*.log'))
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
                ri = rating[ranking[i][1]]
                rj = rating[ranking[j][1]]

                ei = 1.0 / (1.0 + 10 ** ((rj - ri) / 400.0))
                ej = 1.0 / (1.0 + 10 ** ((ri - rj) / 400.0))

                diff[i] += int(16 * (1 - ei))
                diff[j] += int(16 * (0 - ej))

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
            his['diff'][ranking[i][1]] = diff[i]
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

