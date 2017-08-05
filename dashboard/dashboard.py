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

"""
@route('/submit-solution', method='POST')
def solution_submit_post():
    problem_id = request.forms.get('problem_id')
    solution = request.forms.get('solution')
    solution.replace('\r\n','\n')
    filename = "/tmp/" + problem_id + "-" + str(time.time())
    with open(filename, 'w') as f:
        f.write(solution)
    output = subprocess.check_output([repo + "/solution-submit", problem_id, filename])
    os.remove(filename)
    return template('output', output=output)

@route('/submit-problem/<publish_time>', method='POST')
def problem_submit_post(publish_time):
    solution = request.forms.get('solution')
    solution.replace('\r\n','\n')
    filename = "/tmp/" + publish_time + "-" + str(time.time())
    with open(filename, 'w') as f:
        f.write(solution)
    output = subprocess.check_output([repo + "/problem-submit", publish_time, filename])
    os.remove(filename)
    return template('output', output=output)
"""

if __name__ == "__main__":
    app.run(host='0.0.0.0', port=5000, threaded=True, debug=True)

