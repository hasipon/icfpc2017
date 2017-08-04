import json
import time
import glob
import os
import subprocess
import pathlib
from flask import Flask, request, render_template

static_path = pathlib.Path(__file__).resolve().parent / 'static'
repo_path = pathlib.Path(__file__).resolve().parent.parent

app = Flask(__name__, static_folder = str(static_path), static_url_path='')

"""
@route('/problems/<file_name>')
def route_problems(file_name):
    return static_file(file_name, problems)

@route('/visualizer/<file_name>')
def route_problems(file_name):
    return static_file(file_name, visualizer)
"""

@app.route('/')
def index():
    json_data = ''
    with open(str(repo_path / 'problems.json')) as f:
        json_data = f.read()
    data = json.loads(json_data)
    probs = data['problems']
    return render_template('index.html', problems=probs)

@app.route('/gitstatus')
def git_status():
    output = ""
    try:
        output += subprocess.check_output(["git", "status"]).decode('utf-8').strip()
    except subprocess.CalledProcessError as e:
        output += "Error:" + str(e)
    return render_template('output.html', output=output)

@app.route('/pull')
def pull():
    try:
        output = subprocess.check_output(["git", "pull", "origin", "master"]).decode('utf-8').strip()
    except subprocess.CalledProcessError as e:
        output += "Error:" + str(e)
    return render_template('output', output=output)

@app.route('/submit-solution')
def solution_submit_get():
    return render_template('submit')

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
    app.run(port=8080, debug=True)

