import json
import time
import glob
import os
import subprocess
import pathlib
import threading
from flask import Flask, request, render_template, abort
from werkzeug.utils import secure_filename

static_path = pathlib.Path(__file__).resolve().parent / 'static'
repo_path = pathlib.Path(__file__).resolve().parent.parent
app = Flask(__name__, static_folder = str(static_path), static_url_path='')
app.config['SEND_FILE_MAX_AGE_DEFAULT'] = 0

@app.after_request
def add_header(response):
    if 'Expires' in response.headers:
        del response.headers['Expires']
    response.headers['Cache-Control'] = 'no-store'
    return response

@app.route('/')
def index():
    logpath = str(static_path / 'logs')
    logfiles = glob.glob(os.path.join(logpath, '*@[0-9]*.log'))
    logfiles = [os.path.relpath(x, logpath) for x in logfiles]
    logfiles.sort(key=lambda x: x.split('@')[1], reverse=True)
    return render_template('index.html', logfiles=logfiles)

@app.route('/gitstatus')
def git_status():
    output = ""
    try:
        output += subprocess.check_output(["git", "status"], stderr=subprocess.STDOUT).decode('utf-8').strip()
    except subprocess.CalledProcessError as e:
        output += "Error:" + str(e)
    return render_template('output.html', output=output)

@app.route('/update')
def git_pull():
    output = ""
    try:
        output += subprocess.check_output(["git", "pull", "origin", "master"], stderr=subprocess.STDOUT).decode('utf-8').strip()
    except subprocess.CalledProcessError as e:
        output += "Error:" + str(e)
    return render_template('output.html', output=output)

@app.route('/uploadlog', methods=['POST'])
def upload_log():
    # check if the post request has the file part
    if 'file' not in request.files:
        return abort(403)
    f = request.files['file']
    if f.filename == '':
        return abort(403)
    if f:
        filename = secure_filename(f.filename)
        f.save(str(static_path / 'logs' / f.filename))
        return ('', 204)

@app.route('/search')
def get_search():
    return render_template('search.html')

@app.route('/autobattle')
def get_autobattle():
    search = request.args.get("search")
    punter_min = request.args.get("punter-min")
    punter_max = request.args.get("punter-max")
    site_min = request.args.get("site-min")
    site_max = request.args.get("site-max")
    river_min = request.args.get("river-min")
    river_max = request.args.get("river-max")
    mine_min = request.args.get("mine-min")
    mine_max = request.args.get("mine-max")
    futures_on = request.args.get("futures-on")
    futures_off = request.args.get("futures-off")
    splurges_on = request.args.get("splurges-on")
    splurges_off = request.args.get("splurges-off")
    options_on = request.args.get("options-on")
    options_off = request.args.get("options-off")

    def log_filter(info):
        punter_count = info['punters']
        site_count = len(info['map']['sites'])
        river_count = len(info['map']['rivers'])
        mine_count = len(info['map']['mines'])

        futures_enabled = False
        splurges_enabled = False
        options_enabled = False

        if 'settings' in info:
            if 'futures' in info['settings'] and info['settings']['futures']:
                futures_enabled = True
            if 'splurges' in info['settings'] and info['settings']['splurges']:
                splurges_enabled = True
            if 'options' in info['settings'] and info['settings']['options']:
                options_enabled = True

        if punter_min and int(punter_min) > punter_count:
            return False
        if punter_max and int(punter_max) < punter_count:
            return False
        if site_min and int(site_min) > site_count:
            return False
        if site_max and int(site_max) < site_count:
            return False
        if river_min and int(river_min) > river_count:
            return False
        if river_max and int(river_max) < river_count:
            return False
        if mine_min and int(mine_min) > mine_count:
            return False
        if mine_max and int(mine_max) < mine_count:
            return False
        if futures_enabled and not futures_on:
            return False
        if not futures_enabled and not futures_off:
            return False
        if splurges_enabled and not splurges_on:
            return False
        if not splurges_enabled and not splurges_off:
            return False
        if options_enabled and not options_on:
            return False
        if not options_enabled and not options_off:
            return False
        return True

    rating_json = calc_rating(log_filter) if search else calc_rating(None)

    rating_csv = []
    if rating_json:
        # add header
        header = ['#']
        for name in rating_json['ranking']:
            header.append(name)
        rating_csv.append("%r,\n" % header)

        rate = {}
        for name in rating_json['ranking']:
            rate[name] = 1500

        # add initial rating
        line = []
        line.append(str(len(rating_csv)))
        for name in rating_json['ranking']:
            line.append(rate[name])
        rating_csv.append("%r,\n" % line)

        # add dynamic rating
        for data in reversed(rating_json['history']):
            line = []
            line.append(str(len(rating_csv)))
            for ranking in data['ranking']:
                name = ranking[1]
                rate[name] += data['diff'][name]
            for name in rating_json['ranking']:
                line.append(rate[name])
            rating_csv.append("%r,\n" % line)
    return render_template('autobattle.html', json=rating_json, csv=rating_csv)

def invoke_server(port, num_of_punters, map_json):
    print("invoke server")
    subprocess.run([
        "ruby",
        "../common/server/app.rb",
        "-p", port,
        "-n", num_of_punters,
        "-j", map_json
    ])

@app.route('/run_server', methods=['GET', 'POST'])
def run_server():
    if request.method == 'GET':
        return render_template('run_server.html')
    else:
        port = request.form.get('port')
        num_of_punters = request.form.get('num_of_punters')
        map_json = request.form.get('map_json')

        args = (port, num_of_punters, map_json)

        subprocess.Popen([
            "ruby",
            "../common/server/app.rb",
            "-p", port,
            "-n", num_of_punters,
            "-j", map_json
        ])

        message = "server started with port = {0}, num_of_punters = {1}, map_json = {2}".format(port, num_of_punters, map_json)

        # threading.Thread(target=invoke_server, args=args)
        
        return render_template('run_server.html', message=message)

##
## Rating Calculation
##
def fix_name(name):
    if '@' not in name:
        return name
    return name.split('@')[0]

def test_log_filter(info):
    punter_count = info['punters']
    site_count = len(info['map']['sites'])
    river_count = len(info['map']['rivers'])
    mine_count = len(info['map']['mines'])

    futures = False
    splurges = False
    options = False

    if 'settings' in info:
        if 'futures' in info['settings'] and info['settings']['futures']:
            futures = True
        if 'splurges' in info['settings'] and info['settings']['splurges']:
            splurges = True
        if 'options' in info['settings'] and info['settings']['options']:
            options = True

    return(4 <= info['punters'])

def calc_rating(log_filter):
    log_path = str(static_path / 'logs')
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

        if log_filter and not log_filter(info):
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
    return output

if __name__ == "__main__":
    app.run(host='0.0.0.0', port=5000, threaded=True, debug=True)

